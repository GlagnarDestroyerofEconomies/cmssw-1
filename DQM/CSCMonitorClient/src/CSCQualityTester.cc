/** \file
 *
 *  Implementation of CSCQualityTester
 *
 *  $Date: 2008/01/22 18:54:03 $
 *  $Revision: 1.7 $
 *  \author Ilaria Segoni
 */
#include "DQM/CSCMonitorClient/interface/CSCQualityTester.h"
#include "DQM/CSCMonitorClient/interface/CSCQualityTestTypes.h"
#include "DQMServices/Core/interface/QTest.h"
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"

#include<iostream>

//
CSCQualityTester::CSCQualityTester()
{
  
  printout=true;
  logFile.open("CSCDQMClient.log");
  
  if(printout) logFile <<"An CSC Quality Tester Is being Created"<<std::endl;

}

//
CSCQualityTester::~CSCQualityTester(){ 
  logFile.close();
}



//
void CSCQualityTester::SetupTests(DQMOldReceiver * mui){
  if(printout) logFile<<"In CSCQualityTester::SetupTests(...)"<<std::endl;  
  this->SetupTestsFromTextFile(mui);
}


void CSCQualityTester::RunTests(DQMOldReceiver * mui){
  this->LinkTeststoME(mui);
  //this->AttachRunTests(mui);

}

//
void CSCQualityTester::SetupTestsFromTextFile(DQMOldReceiver * mui ){ 


  if(printout) logFile<<"In CSCQualityTester::SetupTestsFromTextFiles\n...\n"<<std::endl;
  FILE* TestsFile;
  char  fileName[128];
  sprintf(fileName,"QualityTests.db");
  TestsFile= fopen(fileName,"r");

  char  TestType[20];
  char  TestName[20];
  int   OnOff;
  float WarningLevel=0;
  float params[5];
  
  for(int ii=0; ii<5; ii++) params[ii]=0.0;

  int file_status=1;
 
  do{    
     file_status=fscanf(TestsFile,"%s%s%d%f%f%f%f%f%f\n",
                         &TestType,&TestName,&OnOff,&WarningLevel,
			 &params[0],&params[1],&params[2],&params[3],&params[4]);
			 
    if(file_status != EOF){			 
    
           if(printout)  logFile<<"\nReading Quality Tests Configuration: "
                          <<TestType<<" "<<TestName <<" "<<OnOff<<" "<<WarningLevel<<" "
	                  <<params[0]<<" "<<params[1]<<" "<<params[2]
	                  <<" "<<params[3]<<" "<<params[4]<< std::endl;
	   if(OnOff){
           	this->ConfigureTest(TestType,mui,TestName,WarningLevel,params);    
	   }
    }

  }while( file_status!=EOF ); 



}


void CSCQualityTester::LinkTeststoME(DQMOldReceiver * mui){ 


  if(printout) logFile<<"\nIn CSCQualityTester::LinkTeststoME"<<std::endl;
  FILE* TestsMEsFile;
  char  fileName[128];
  sprintf(fileName,"CSCTestsMEs.db");
  TestsMEsFile= fopen(fileName,"r");
  
  
  char TestName[20];
  char MEName[20];
  
  int file_status=1;
 
  DQMStore* bei = (mui)->getBEInterface(); 
  do{    
      file_status=fscanf(TestsMEsFile,"%s%s\n",&TestName,&MEName);
			 
      if(file_status != EOF){			 
            if(printout) logFile<<"\nRequesting ME: "<<MEName<<"and attaching test: " <<TestName<<std::endl;
	    // bei->subscribe(MEName);
            bei->useQTest(MEName, TestName);
	    
	    std::vector<std::string>  MEList;
	    if( qTestToMEMap.find(TestName) == qTestToMEMap.end()){
            if(printout) logFile<<"nuovo " <<TestName <<" "<<MEName << std::endl;
 	      MEList.push_back(MEName);
	      qTestToMEMap[TestName]=MEList;
	    }else{
            if(printout) logFile<<"trovato " <<TestName <<" "<<MEName << std::endl;
	      MEList=qTestToMEMap[TestName];
	      MEList.push_back(MEName);
             if(printout) logFile<<"ME's for test " <<TestName <<": "<<MEList.size() << std::endl;
	    }	    
      }	

  }while( file_status!=EOF ); 



}

void CSCQualityTester::SetContentsXRangeROOTTest(DQMOldReceiver * mui, char  TestName[20], float  WarningLevel, float  params[5]){

  if(printout) logFile<<"In CSCQualityTester::SetContentsXRangeROOTTest, configuring "<<TestName<<" test."<<std::endl;

  DQMStore* bei = (mui)->getBEInterface();	
  QCriterion * qc1 = bei->createQTest(ContentsXRange::getAlgoName(),TestName);
  ContentsXRange * me_qc1 = (ContentsXRange *) qc1;
  me_qc1->setAllowedXRange(params[0],params[1]);
  me_qc1->setWarningProb(WarningLevel);
 
}


void CSCQualityTester::SetContentsYRangeROOTTest(DQMOldReceiver * mui, char  TestName[20], float  WarningLevel, float  params[5]){

  if(printout) logFile<<"In CSCQualityTester::SetContentsYRangeROOTTest, configuring "<<TestName<<" test"<<std::endl;
	
  qTests.push_back(TestName);
  DQMStore* bei = (mui)->getBEInterface();
  QCriterion * qc1 = bei->createQTest(ContentsYRange::getAlgoName(),TestName);
  ContentsYRange * me_qc1 = (ContentsYRange *) qc1;
  me_qc1->setAllowedYRange(params[0],params[1]);
  me_qc1->setWarningProb(WarningLevel);

}


void CSCQualityTester::AttachRunTests(DQMOldReceiver * mui){

if(printout) logFile<<"\nIn CSCQualityTester::AttachRunTests(...)"<<std::endl;

for (std::map<std::string, std::vector<std::string> >::iterator testsMap=qTestToMEMap.begin(); 
			   testsMap!=qTestToMEMap.end();++testsMap)	   
    {
     
       std::string testName=testsMap->first;
       std::vector<std::string> MElist=testsMap->second;
    	DQMStore* bei = (mui)->getBEInterface();
       if(printout) logFile<<"Number of ME's for Test "<< testName <<": "<<MElist.size()<<std::endl;

      	     for(std::vector<std::string>::iterator list = MElist.begin(); list != MElist.end(); ++list){
      	    	  std::string meName = *(list);
      	    	  if(printout) logFile<<"Attaching Test "<< testName <<" to ME "<<meName<<std::endl;
      	    	  bei->useQTest(meName, testName);
      	     }
    }
}



std::pair<std::string,std::string> CSCQualityTester::CheckTestsGlobal(DQMOldReceiver * mui) 
{
  if(printout) logFile << "\nChecking Global Status of Quiality Tests" << std::endl;
  std::pair<std::string,std::string> statement;
  
	int status = 0;
	DQMStore* bei = (mui)->getBEInterface();	
	status= bei->getStatus();
        if(printout) logFile << "Possible states: successful "<< dqm::qstatus::STATUS_OK<<", error:  " 
	<< dqm::qstatus::ERROR<<
	", warning:  "<< dqm::qstatus::WARNING<<
	", Other: "<< dqm::qstatus::OTHER<<std::endl;
        if(printout) logFile << "The STATUS IS " << status<<std::endl;
	
	switch(status)
	  {
	  case dqm::qstatus::ERROR:
	    logFile << " Error(s)";
	    statement.first ="Errors detected in quality tests";
	    statement.second="red";
	    break;
	  case dqm::qstatus::WARNING:
	    logFile << " Warning(s)";
	    statement.first ="Warnings detected in quality tests";
	    statement.second="orange";
	    break;
	  case dqm::qstatus::OTHER:
	    logFile << " Some tests did not run";
	    statement.first="Some tests did not run";
	    statement.second="black";
	    break; 
	  default:
	    logFile << " No problems";
	    statement.first="No problems detected in quality tests ";
	    statement.second="green";
	  }
	    logFile << "\n";

  return statement;
}

 
void CSCQualityTester::ConfigureTest(char TestType[20], DQMOldReceiver * mui, char  TestName[20], float  WarningLevel, float  params[5]){
  
 qTests.push_back(TestName);
 
 logFile << "Setting up test "<<TestName<<std::endl;
 
 if(!std::strcmp(TestType,csc_dqm::qTestType::XRangeContent.c_str())) this->SetContentsXRangeROOTTest(mui,TestName,WarningLevel,params);    
 if(!std::strcmp(TestType,csc_dqm::qTestType::YRangeContent.c_str())) this->SetContentsYRangeROOTTest(mui,TestName,WarningLevel,params);    



}

std::map< std::string, std::vector<std::string> > CSCQualityTester::CheckTestsSingle(DQMOldReceiver * mui){
 
 detailedWarnings.clear();
 this->SearchDirectories(mui);
 return detailedWarnings;

} 

void CSCQualityTester::SearchDirectories(DQMOldReceiver * mui) 
{
  
 DQMStore* bei = (mui)->getBEInterface();
 std::vector<std::string> meNames=bei->getMEs();   
 std::vector<std::string> dirNames=bei->getSubdirs();
 int pippo=meNames.size();
 std::string currentDir=bei->pwd();
 if(printout) {
 	logFile << "Searching ME's with quality tests in " << currentDir<<"\n"
   		       << "There are " << pippo <<" monitoring elements and "
  	  	       << dirNames.size()<<" directories\n"<< std::endl;
 }


  if(meNames.size()) {

  	this->ProcessAlarms(meNames, currentDir, mui);
  }
 
  if(dirNames.size()){
  	for(std::vector<std::string>::iterator it = dirNames.begin(); 
			it != dirNames.end();++it)
    	{
      		bei->cd(*it);
      		this->SearchDirectories(mui);
    	}   
   }   	
	
  bei->goUp();
  
  return;
}


void CSCQualityTester::ProcessAlarms(std::vector<std::string> meNames, std::string currentDir, DQMOldReceiver * mui){
    logFile<<"sonoqui0, number of me's: "<<meNames.size()<<std::endl;

 for(std::vector<std::string>::iterator nameItr= meNames.begin(); nameItr!= meNames.end(); ++nameItr){
      
      std::string colour;
      char text[128];
        
	MonitorElement * me =0;
	logFile<<"sonoquinome "<<*nameItr<<std::endl;
	char fullPath[128];
	sprintf(fullPath,"%s/%s",currentDir.c_str(),(*nameItr).c_str());
	logFile<<"fullpath "<<fullPath<<std::endl;
	DQMStore* bei = (mui)->getBEInterface();
	me= bei->get(fullPath);
        logFile<<"sonoqui1 "<<me<<std::endl;
	std::vector<QReport *> report;
	if(me){
		 if (me->hasError()){
		 	colour="red";
			report= me->getQErrors();
        		logFile<<"sonoqui1 "<<std::endl;
		 } 
		 if( me->hasWarning()){ 
		 	colour="orange";
			report= me->getQWarnings();
        		logFile<<"sonoqui2 "<< std::endl;
		 }
		 if(me->hasOtherReport()){
		 	colour="black";
 			report= me->getQOthers();
       			logFile<<"sonoqui3 "<<std::endl;
		 }
	
		 for(std::vector<QReport *>::iterator itr=report.begin(); itr!=report.end();++itr ){
		        sprintf(text,"%s:%s",(*nameItr).c_str(),((*itr)->getMessage()).c_str());
			logFile<<"report "<<(*itr)->getMessage()<<std::endl;
			logFile<<"text= "<<text<<std::endl;
	    		
			std::vector<std::string> messageList;
	    		if( detailedWarnings.find(colour) == detailedWarnings.end()){
 	      			messageList.push_back(text);
	      			detailedWarnings[colour]=messageList;
	    		}else{
	      			messageList=detailedWarnings[colour];
	      			messageList.push_back(text);
	    		}	    
		 }	
	}
 }

}
