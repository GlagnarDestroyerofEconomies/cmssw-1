/*
 * \file DQMEventInfo.cc
 * \author M. Zanetti - CERN PH
 * Last Update:
 * $Date: 2008/03/04 22:29:06 $
 * $Revision: 1.16 $
 * $Author: ameyer $
 *
 */

#include "DQMEventInfo.h"
#include <TSystem.h>

// Framework


#include <stdio.h>
#include <sstream>
#include <math.h>

using namespace edm;
using namespace std;

DQMEventInfo::DQMEventInfo(const ParameterSet& ps){
  
  parameters_ = ps;
  pEvent_ = 0;
  evtRateCount_ = 0;
  gettimeofday(&currentTime_,NULL);
  lastAvgTime_ = currentTime_;
  
  dbe_ = edm::Service<DQMStore>().operator->();

  string eventinfofolder = parameters_.getUntrackedParameter<string>("eventInfoFolder", "EventInfo") ;
  string subsystemname = parameters_.getUntrackedParameter<string>("subSystemFolder", "YourSubsystem") ;
  string currentfolder = subsystemname + "/" +  eventinfofolder ;
  cout << "currentfolder " << currentfolder << endl;

  evtRateWindow_ = parameters_.getUntrackedParameter<double>("eventRateWindow", 0.5);
  if(evtRateWindow_<=0.15) evtRateWindow_=0.15;
  cout << "Event Rate averaged over " << evtRateWindow_ << " minutes" << endl;

  dbe_->setCurrentFolder(currentfolder) ;

  //Event specific contents
  runId_     = dbe_->bookInt("iRun");
  lumisecId_ = dbe_->bookInt("iLumiSection");
  eventId_   = dbe_->bookInt("iEvent");
  eventTimeStamp_ = dbe_->bookFloat("eventTimeStamp");
  errSummary_ = dbe_->bookFloat("errorSummary");
  errSummary_->Fill(-1);
  errSummaryEtaPhi_ =
  dbe_->book2D("errorSummaryEtaPhi","errorSummaryEtaPhi",60,-3.,3.,64,-3.2,3.2);
  string suberrfolder = currentfolder + "/errorSummarySegments" ;
  dbe_->setCurrentFolder(suberrfolder);
  for (int i=0 ; i<10 ; i++) {
    char suberr[20];
    sprintf (suberr,"Segment0%d",i);
    errSummarySegment_[i] = dbe_->bookFloat(suberr);
  }
  
  dbe_->setCurrentFolder(currentfolder) ;
  //Process specific contents
  processTimeStamp_ = dbe_->bookFloat("processTimeStamp");
  processTimeStamp_->Fill(getUTCtime(&currentTime_));
  processLatency_ = dbe_->bookFloat("processLatency");
  processTimeStamp_->Fill(-1);
  processEvents_ = dbe_->bookInt("processedEvents");
  processEvents_->Fill(pEvent_);
  processEventRate_ = dbe_->bookFloat("processEventRate");
  processEventRate_->Fill(-1); 
  nUpdates_= dbe_->bookInt("processUpdates");
  nUpdates_->Fill(-1);
  

  //Static Contents
  processId_= dbe_->bookInt("processID"); 
  processId_->Fill(gSystem->GetPid());
  hostName_= dbe_->bookString("hostName",gSystem->HostName());
  processName_= dbe_->bookString("processName",subsystemname);
  workingDir_= dbe_->bookString("workingDir",gSystem->pwd());
  cmsswVer_= dbe_->bookString("CMSSW_Version",edm::getReleaseVersion());
  dqmPatch_= dbe_->bookString("DQM_Patch",dbe_->getDQMPatchVersion());
    
}

DQMEventInfo::~DQMEventInfo(){

  cout<<"[DQMEventInfo]: destructor"<<endl;

}

void DQMEventInfo::analyze(const Event& e, const EventSetup& c){

  runId_->Fill(e.id().run());
  lumisecId_->Fill(e.luminosityBlock());
  eventId_->Fill(e.id().event());
//  unsigned int ts_lo = (e.time().value()&0xFFFFFFFF);
  unsigned int ts_lo = (e.time().value()/(double)0xffffffff);
  eventTimeStamp_->Fill(ts_lo);

  pEvent_++;
  evtRateCount_++;
  processEvents_->Fill(pEvent_);

  lastUpdateTime_=currentTime_;
  gettimeofday(&currentTime_,NULL);  
  processTimeStamp_->Fill(getUTCtime(&currentTime_));
  processLatency_->Fill(getUTCtime(&lastUpdateTime_,&currentTime_));

  float time = getUTCtime(&lastAvgTime_,&currentTime_);
  if(time>=(evtRateWindow_*60.0)){
    processEventRate_->Fill((float)evtRateCount_/time);
    evtRateCount_ = 0;
    lastAvgTime_ = currentTime_;    
  }

  return;
}

double DQMEventInfo::getUTCtime(timeval* a, timeval* b){
  double deltaT=(*a).tv_sec*1000.0+(*a).tv_usec/1000.0;
  if(b!=NULL) deltaT=(*b).tv_sec*1000.0+(*b).tv_usec/1000.0 - deltaT;
  return deltaT/1000.0;
}
