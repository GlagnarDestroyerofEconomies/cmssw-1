#include "DQM/CastorMonitor/interface/CastorEventDisplay.h"
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"

//***************************************************//
//********** CastorEventDisplay: *******************//
//********** Author: Dmytro Volyanskyy   ************//
//********** Date  : 04.05.2010 (first version) ******// 
//***************************************************//
////---- to visualize CastorRecHits 
////---- last revision: 04.05.2010

//==================================================================//
//======================= Constructor ==============================//
//==================================================================//
CastorEventDisplay::CastorEventDisplay() {
  ievt_=0;
}

//==================================================================//
//======================= Destructor ==============================//
//==================================================================//
CastorEventDisplay::~CastorEventDisplay(){
}

void CastorEventDisplay::reset(){
}


//==========================================================//
//========================= setup ==========================//
//==========================================================//

void CastorEventDisplay::setup(const edm::ParameterSet& ps, DQMStore* dbe){
  
  CastorBaseMonitor::setup(ps,dbe);
  baseFolder_ = rootFolder_+"CastorEventDisplay";
  

  offline_             = ps.getUntrackedParameter<bool>("OfflineMode", false); 

  if(fVerbosity>0) cout << "CastorEventDisplay::setup (start)" << endl;
  
  ////--- initialize these here
  ievt_=0;  allEnergyEvent=0.; maxEnergyEvent=0.;
  X_pos=0.; Y_pos=0.; Z_pos=0.;
  X_pos_maxE=0.; Y_pos_maxE=0.; Z_pos_maxE=0.;

  if ( m_dbe !=NULL ) {    
  m_dbe->setCurrentFolder(baseFolder_);

  ////---- book MonitorElements
  meEVT_ = m_dbe->bookInt("EventDisplay Event Number"); // meEVT_->Fill(ievt_);
  
  ////---- cumulative event display
  meCastor3Dhits = m_dbe->book3D("CASTOR 3D hits- cumulative", "CASTOR 3D hits - cumulative", 30, -1600, -1420, 35, -35, 35, 35, -35, 35);


  if( offline_ ){
  //-- swap z and y axis 
  TH3F* Castor3Dhits = meCastor3Dhits->getTH3F();
  Castor3Dhits->GetXaxis()->SetTitle("Z [cm]"); //-- also swap x and z
  Castor3Dhits->GetYaxis()->SetTitle("X [cm]");
  Castor3Dhits->GetZaxis()->SetTitle("Y [cm]");
  }

  ////---- event display of an event with the largest deposited energy
  meCastor3DhitsMaxEnergy = m_dbe->book3D("CASTOR 3D hits- event with the largest deposited E", "CASTOR 3D hits- event with the largest deposited E",  30, -1600, -1420, 20, -30, 30, 20, -30, 30); //-- swap z and y axis

  if( offline_ ){
  TH3F* Castor3DhitsMaxEnergy = meCastor3DhitsMaxEnergy->getTH3F();
  Castor3DhitsMaxEnergy->GetXaxis()->SetTitle("Z [cm]"); //-- also swap x and z
  Castor3DhitsMaxEnergy->GetYaxis()->SetTitle("X [cm]");
  Castor3DhitsMaxEnergy->GetZaxis()->SetTitle("Y [cm]");
  Castor3DhitsMaxEnergy->SetDrawOption("LEGO2");
  }

 } 

  else{
  if(fVerbosity>0) cout << "CastorEventDisplay::setup - NO DQMStore service" << endl; 
  }

  if(fVerbosity>0) cout << "CastorEventDisplay::setup (end)" << endl;
  
  return;
}

//==========================================================//
//================== processEvent ==========================//
//==========================================================//

void CastorEventDisplay::processEvent(const CastorRecHitCollection& castorHits, const CaloGeometry& caloGeometry ){
  
  if(fVerbosity>0) cout << "==>CastorEventDisplay::processEvent !!!"<< endl;

  ////---- fill the event number
   meEVT_->Fill(ievt_);

  if(!m_dbe) { 
    if(fVerbosity>0) cout <<"CastorEventDisplay::processEvent => DQMStore is not instantiated !!!"<<endl;  
    return; 
  }

   allEnergyEvent=0.;


  ////---- define iterator
  CastorRecHitCollection::const_iterator CASTORiter;

     if(castorHits.size()>0)
    {    
       if(fVerbosity>0) cout << "==>CastorEventDisplay::processEvent: castorHits.size()>0 !!!" << endl; 

       ////---- loop over all hits
       for (CASTORiter=castorHits.begin(); CASTORiter!=castorHits.end(); ++CASTORiter) { 
     	
         ////---- get energy
         energy = CASTORiter->energy();    

        ////---- get CASTOR ID
        HcalCastorDetId CastorID(CASTORiter->detid().rawId());

        ////---- get positions (default in cm)
	//const CaloSubdetectorGeometry* subdetectorGeometry=caloGeometry.getSubdetectorGeometry(CastorID) ; 
        //const CaloCellGeometry* cellGeometry =  subdetectorGeometry->getGeometry(CastorID) ;
        //X_pos = cellGeometry->getPosition().x() ; Y_pos = cellGeometry->getPosition().y() ; 
        //Z_pos = cellGeometry->getPosition().z() ;
       
        X_pos = caloGeometry.getSubdetectorGeometry(CastorID)->getGeometry(CastorID)->getPosition().x() ;
        Y_pos = caloGeometry.getSubdetectorGeometry(CastorID)->getGeometry(CastorID)->getPosition().y() ;
        Z_pos = caloGeometry.getSubdetectorGeometry(CastorID)->getGeometry(CastorID)->getPosition().z() ;
        
        if (energy>0){
          ////--- fill the cumulative energy in an event
         allEnergyEvent= allEnergyEvent+energy ;
         ////--- fill the cumulative distribution
         meCastor3Dhits->Fill(Z_pos,X_pos,Y_pos, energy);
	}

	if(fVerbosity>0)  cout<<"ENERGY="<< energy <<" X_pos="<< X_pos <<" Y_pos="<< Y_pos <<" Z_pos="<< Z_pos << endl;   
      } //-- end of for loop


    if(fVerbosity>0)  cout<<"TOTAL ENERGY in an event="<< allEnergyEvent << endl;



    ////---- check whether the cumulative energy is the largest 
    if (allEnergyEvent > maxEnergyEvent) { 
     maxEnergyEvent = allEnergyEvent;
     meCastor3DhitsMaxEnergy->Reset();
     if(fVerbosity>0) cout<<"LARGEST ENERGY in an event="<< maxEnergyEvent << endl;
 
    ////---- loop over all hits
     for (CASTORiter=castorHits.begin(); CASTORiter!=castorHits.end(); ++CASTORiter){
       HcalCastorDetId CastorID_maxE(CASTORiter->detid().rawId());
       X_pos_maxE = caloGeometry.getSubdetectorGeometry(CastorID_maxE)->getGeometry(CastorID_maxE)->getPosition().x() ;
       Y_pos_maxE = caloGeometry.getSubdetectorGeometry(CastorID_maxE)->getGeometry(CastorID_maxE)->getPosition().y() ;
       Z_pos_maxE = caloGeometry.getSubdetectorGeometry(CastorID_maxE)->getGeometry(CastorID_maxE)->getPosition().z() ;
       meCastor3DhitsMaxEnergy->Fill(Z_pos_maxE,X_pos_maxE,Y_pos_maxE, CASTORiter->energy() );
      }
     meCastor3DhitsMaxEnergy->getTH3F()->SetOption("BOX");
     meCastor3DhitsMaxEnergy->update();
    }
  } ////---- end of castorHits.size()>0

  else { if(fVerbosity>0) cout<<"CastorEventDisplay::processEvent NO Castor RecHits !!!"<<endl; }


  if (showTiming) { 
      cpu_timer.stop(); cout << " TIMER::CastorEventDisplay -> " << cpu_timer.cpuTime() << endl; 
      cpu_timer.reset(); cpu_timer.start();  
    }
    
  ////---- increment here
  ievt_++; 
  
  return;
}


