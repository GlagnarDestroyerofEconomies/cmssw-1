/*
 * \file EcalBarrelSimHitsValidation.cc
 *
 * \author C.Rovelli
 *
*/

#include "FWCore/Utilities/interface/Exception.h"
#include <DataFormats/EcalDetId/interface/EBDetId.h>
#include "Validation/EcalHits/interface/EcalBarrelSimHitsValidation.h"

using namespace cms;
using namespace edm;
using namespace std;

EcalBarrelSimHitsValidation::EcalBarrelSimHitsValidation(const edm::ParameterSet& ps):
  g4InfoLabel(ps.getParameter<std::string>("moduleLabelG4")),
  EBHitsCollection(ps.getParameter<std::string>("EBHitsCollection")),
  ValidationCollection(ps.getParameter<std::string>("ValidationCollection")){   

  // verbosity switch
  verbose_ = ps.getUntrackedParameter<bool>("verbose", false);
 
  if ( verbose_ ) {
    std::cout << " verbose switch is ON" << std::endl;
  } else {
    std::cout << " verbose switch is OFF" << std::endl;
  }

  // get hold of back-end interface
  dbe_ = 0;
  dbe_ = edm::Service<DaqMonitorBEInterface>().operator->();           
  if ( dbe_ ) {
    if ( verbose_ ) { dbe_->setVerbose(1); } 
    else            { dbe_->setVerbose(0); }
  }
                                                                               
  if ( dbe_ ) {
    if ( verbose_ ) dbe_->showDirStructure();
  }
 
  menEBHits_       = 0;
  menEBCrystals_   = 0;
  meEBOccupancy_   = 0;
  meEBLongitudinalShower_ = 0;
  meEBhitEnergy_   = 0;

  meEBe1_  = 0;  
  meEBe4_  = 0;  
  meEBe9_  = 0;  
  meEBe16_ = 0; 
  meEBe25_ = 0; 

  meEBe1oe4_   = 0;  
  meEBe1oe9_   = 0;  
  meEBe4oe9_   = 0;  
  meEBe9oe16_  = 0;
  meEBe1oe25_  = 0;  
  meEBe9oe25_  = 0; 
  meEBe16oe25_ = 0;

  myEntries = 0;
  for (int myStep = 0; myStep<26; myStep++) { eRLength[myStep] = 0.0; }

  Char_t histo[200];
   
  if ( dbe_ ) {
    dbe_->setCurrentFolder("EcalSimHitsValidation");
  
    sprintf (histo, "EB hits multiplicity" ) ;
    menEBHits_ = dbe_->book1D(histo, histo, 50, 0., 5000.) ;
 
    sprintf (histo, "EB crystals multiplicity" ) ;
    menEBCrystals_ = dbe_->book1D(histo, histo, 50, 0., 300.) ; 

    sprintf (histo, "EB occupancy" ) ;
    meEBOccupancy_ = dbe_->book2D(histo, histo, 360, 0., 360., 170, -85., 85.);
  
    sprintf (histo, "EB longitudinal shower profile" ) ;
    meEBLongitudinalShower_ = dbe_->bookProfile(histo, histo, 26, 0., 26., 100, 0., 20000.);

    sprintf (histo, "EB hits energy spectrum" );
    meEBhitEnergy_ = dbe_->book1D(histo, histo, 4000, 0., 400.);

    sprintf (histo, "EB E1" ) ;
    meEBe1_ = dbe_->book1D(histo, histo, 400, 0., 400.);

    sprintf (histo, "EB E4" ) ;
    meEBe4_ = dbe_->book1D(histo, histo, 400, 0., 400.);

    sprintf (histo, "EB E9" ) ;
    meEBe9_ = dbe_->book1D(histo, histo, 400, 0., 400.);

    sprintf (histo, "EB E16" ) ;
    meEBe16_ = dbe_->book1D(histo, histo, 400, 0., 400.);

    sprintf (histo, "EB E25" ) ;
    meEBe25_ = dbe_->book1D(histo, histo, 400, 0., 400.);

    sprintf (histo, "EB E1oE4" ) ;
    meEBe1oe4_ = dbe_->book1D(histo, histo, 100, 0.4, 1.1);

    sprintf (histo, "EB E1oE9" ) ;
    meEBe1oe9_ = dbe_->book1D(histo, histo, 100, 0.4, 1.1);

    sprintf (histo, "EB E4oE9" ) ;
    meEBe4oe9_ = dbe_->book1D(histo, histo, 100, 0.4, 1.1);

    sprintf (histo, "EB E9oE16" ) ;
    meEBe9oe16_ = dbe_->book1D(histo, histo, 100, 0.4, 1.1);

    sprintf (histo, "EB E1oE25" ) ;
    meEBe1oe25_ = dbe_->book1D(histo, histo, 100, 0.4, 1.1);

    sprintf (histo, "EB E9oE25" ) ;
    meEBe9oe25_ = dbe_->book1D(histo, histo, 100, 0.4, 1.1);

    sprintf (histo, "EB E16oE25" ) ;
    meEBe16oe25_ = dbe_->book1D(histo, histo, 100, 0.4, 1.1);
  }
}

EcalBarrelSimHitsValidation::~EcalBarrelSimHitsValidation(){
 
}

void EcalBarrelSimHitsValidation::beginJob(const edm::EventSetup& c){

}

void EcalBarrelSimHitsValidation::endJob(){
  
  for (int myStep=0; myStep<26; myStep++){
    if (meEBLongitudinalShower_) meEBLongitudinalShower_->Fill(float(myStep), eRLength[myStep]/myEntries);
  }
}

void EcalBarrelSimHitsValidation::analyze(const edm::Event& e, const edm::EventSetup& c){

  edm::LogInfo("EventInfo") << " Run = " << e.id().run() << " Event = " << e.id().event();
  
  edm::Handle<edm::PCaloHitContainer> EcalHitsEB;
  e.getByLabel(g4InfoLabel,EBHitsCollection,EcalHitsEB);

  // Do nothing if no Barrel data available
  if( ! EcalHitsEB.isValid() ) return;

  edm::Handle<PEcalValidInfo> MyPEcalValidInfo;
  e.getByLabel(g4InfoLabel,ValidationCollection,MyPEcalValidInfo);

  std::vector<PCaloHit> theEBCaloHits;
  theEBCaloHits.insert(theEBCaloHits.end(), EcalHitsEB->begin(), EcalHitsEB->end());

  myEntries++;

  double EBEnergy_ = 0.;
  std::map<unsigned int, std::vector<PCaloHit>,std::less<unsigned int> > CaloHitMap;
  
  double eb1  = 0.0;
  double eb4  = 0.0;
  double eb9  = 0.0;
  double eb16 = 0.0;
  double eb25 = 0.0;
  
  MapType ebmap;
  uint32_t nEBHits = 0;
  
  for (std::vector<PCaloHit>::iterator isim = theEBCaloHits.begin();
       isim != theEBCaloHits.end(); ++isim){
    CaloHitMap[(*isim).id()].push_back((*isim));
    
    EBDetId ebid (isim->id()) ;
    
    LogDebug("HitInfo") 
      << " CaloHit "    << isim->getName() << "\n" 
      << " DetID = "    << isim->id()   << " EBDetId = " << ebid.ieta() << " " << ebid.iphi() << "\n" 
      << " Time = "     << isim->time()    << "\n"
      << " Track Id = " << isim->geantTrackId() << "\n"
      << " Energy = "   << isim->energy();
    
    if (meEBOccupancy_) meEBOccupancy_->Fill( ebid.iphi(), ebid.ieta() );
    
    uint32_t crystid = ebid.rawId();
    ebmap[crystid] += isim->energy();
    
    EBEnergy_ += isim->energy();
    nEBHits++;
    meEBhitEnergy_->Fill(isim->energy());
    
  }

  if (menEBCrystals_) menEBCrystals_->Fill(ebmap.size());
  
  if (menEBHits_) menEBHits_->Fill(nEBHits);
  
  if (nEBHits > 0 ) {
    
    uint32_t  ebcenterid = getUnitWithMaxEnergy(ebmap);
    EBDetId myEBid(ebcenterid);
    int bx = myEBid.ietaAbs();
    int by = myEBid.iphi();
    int bz = myEBid.zside();
    eb1 =  energyInMatrixEB(1,1,bx,by,bz,ebmap);
    if (meEBe1_) meEBe1_->Fill(eb1);
    eb9 =  energyInMatrixEB(3,3,bx,by,bz,ebmap);
    if (meEBe9_) meEBe9_->Fill(eb9);
    eb25=  energyInMatrixEB(5,5,bx,by,bz,ebmap);
    if (meEBe25_) meEBe25_->Fill(eb25);
    
    MapType  newebmap;
    if( fillEBMatrix(3,3,bx,by,bz,newebmap, ebmap)){
      eb4 = eCluster2x2(newebmap);
      if (meEBe4_) meEBe4_->Fill(eb4);
    }
    if( fillEBMatrix(5,5,bx,by,bz,newebmap, ebmap)){
      eb16 = eCluster4x4(eb9,newebmap); 
      if (meEBe16_) meEBe16_->Fill(eb16);
    }
    
    if (meEBe1oe4_   && eb4  > 0.1 ) meEBe1oe4_  -> Fill(eb1/eb4);
    if (meEBe1oe9_   && eb9  > 0.1 ) meEBe1oe9_  -> Fill(eb1/eb9);
    if (meEBe4oe9_   && eb9  > 0.1 ) meEBe4oe9_  -> Fill(eb4/eb9);
    if (meEBe9oe16_  && eb16 > 0.1 ) meEBe9oe16_ -> Fill(eb9/eb16);
    if (meEBe1oe25_  && eb25 > 0.1 ) meEBe1oe25_ -> Fill(eb1/eb25);
    if (meEBe9oe25_  && eb25 > 0.1 ) meEBe9oe25_ -> Fill(eb9/eb25);
    if (meEBe16oe25_ && eb25 > 0.1 ) meEBe16oe25_-> Fill(eb16/eb25);
    
  }
  
  if( MyPEcalValidInfo.isValid() ) {
    if ( MyPEcalValidInfo->eb1x1() > 0. ) {
      std::vector<float>  BX0 = MyPEcalValidInfo->bX0();
      for (int myStep=0; myStep< 26; myStep++ ) { eRLength[myStep] += BX0[myStep]; }
    }
  }
}

float EcalBarrelSimHitsValidation::energyInMatrixEB(int nCellInEta, int nCellInPhi,
					      int centralEta, int centralPhi,
					      int centralZ, MapType& themap){

  int   ncristals   = 0;
  float totalEnergy = 0.;
  
  int goBackInEta = nCellInEta/2;
  int goBackInPhi = nCellInPhi/2;
  int startEta    = centralZ*centralEta-goBackInEta;
  int startPhi    = centralPhi-goBackInPhi;
  
  for (int ieta=startEta; ieta<startEta+nCellInEta; ieta++) {
    for (int iphi=startPhi; iphi<startPhi+nCellInPhi; iphi++) {
      
      uint32_t index ;
      if (abs(ieta) > 85 || abs(ieta)<1 ) { continue; }
      if (iphi< 1)      { index = EBDetId(ieta,iphi+360).rawId(); }
      else if(iphi>360) { index = EBDetId(ieta,iphi-360).rawId(); }
      else              { index = EBDetId(ieta,iphi).rawId();     }

      totalEnergy   += themap[index];
      ncristals     += 1;
    }
  }
  
  LogDebug("GeomInfo")
    << nCellInEta << " x " << nCellInPhi 
    << " EB matrix energy = " << totalEnergy
    << " for " << ncristals << " crystals" ;
  return totalEnergy;
  
}   

bool  EcalBarrelSimHitsValidation::fillEBMatrix(int nCellInEta, int nCellInPhi,
                                    int CentralEta, int CentralPhi,int CentralZ,
                                    MapType& fillmap, MapType&  themap)
{
  int goBackInEta = nCellInEta/2;
  int goBackInPhi = nCellInPhi/2;
  
  int startEta  =  CentralZ*CentralEta - goBackInEta;
  int startPhi  =  CentralPhi - goBackInPhi;
  
  int i = 0 ;
  for ( int ieta = startEta; ieta < startEta+nCellInEta; ieta ++ ) {
    for( int iphi = startPhi; iphi < startPhi + nCellInPhi; iphi++ ) {
      uint32_t  index;
      if (abs(ieta) > 85 || abs(ieta)<1 ) { continue; }
      if (iphi< 1)      { index = EBDetId(ieta,iphi+360).rawId(); }
      else if(iphi>360) { index = EBDetId(ieta,iphi-360).rawId(); }
      else              { index = EBDetId(ieta,iphi).rawId();     }
      fillmap[i++] = themap[index];
    }
  }
  
  uint32_t  ebcenterid = getUnitWithMaxEnergy(themap);
  
  if ( fillmap[i/2] == themap[ebcenterid] ) 
    return true;
  else 
    return false;
} 

float EcalBarrelSimHitsValidation::eCluster2x2( MapType& themap){
  float  E22=0.;
  float e012  = themap[0]+themap[1]+themap[2];
  float e036  = themap[0]+themap[3]+themap[6];
  float e678  = themap[6]+themap[7]+themap[8];
  float e258  = themap[2]+themap[5]+themap[8];

  if ( (e012>e678 || e012==e678) && (e036>e258  || e036==e258))
    return     E22=themap[0]+themap[1]+themap[3]+themap[4];
  else if ( (e012>e678 || e012==e678)  && (e036<e258 || e036==e258) )
    return    E22=themap[1]+themap[2]+themap[4]+themap[5];
  else if ( (e012<e678 || e012==e678) && (e036>e258 || e036==e258))
    return     E22=themap[3]+themap[4]+themap[6]+themap[7];
  else if ( (e012<e678|| e012==e678)  && (e036<e258|| e036==e258) )
    return    E22=themap[4]+themap[5]+themap[7]+themap[8];
  else {
    return E22;
  }
}

float EcalBarrelSimHitsValidation::eCluster4x4(float e33,  MapType&  themap){
  float E44=0.;
  float e0_4   = themap[0]+themap[1]+themap[2]+themap[3]+themap[4];
  float e0_20  = themap[0]+themap[5]+themap[10]+themap[15]+themap[20];
  float e4_24  = themap[4]+themap[9]+themap[14]+themap[19]+themap[24];
  float e0_24  = themap[20]+themap[21]+themap[22]+themap[23]+themap[24];
  
  if ((e0_4>e0_24 || e0_4==e0_24) && (e0_20>e4_24|| e0_20==e4_24))
    return E44=e33+themap[0]+themap[1]+themap[2]+themap[3]+themap[5]+themap[10]+themap[15];
  else if ((e0_4>e0_24 || e0_4==e0_24)  && (e0_20<e4_24 || e0_20==e4_24))
    return E44=e33+themap[1]+themap[2]+themap[3]+themap[4]+themap[9]+themap[14]+themap[19];
  else if ((e0_4<e0_24|| e0_4==e0_24) && (e0_20>e4_24 || e0_20==e4_24))
    return E44=e33+themap[5]+themap[10]+themap[15]+themap[20]+themap[21]+themap[22]+themap[23];
  else if ((e0_4<e0_24|| e0_4==e0_24) && (e0_20<e4_24 || e0_20==e4_24))
    return E44=e33+themap[21]+themap[22]+themap[23]+themap[24]+themap[9]+themap[14]+themap[19];
  else{
    return E44;
  }
}

uint32_t EcalBarrelSimHitsValidation::getUnitWithMaxEnergy(MapType& themap) {
  
  //look for max
  uint32_t unitWithMaxEnergy = 0;
  float    maxEnergy = 0.;
  
  MapType::iterator iter;
  for (iter = themap.begin(); iter != themap.end(); iter++) {
    
    if (maxEnergy < (*iter).second) {
      maxEnergy = (*iter).second;       
      unitWithMaxEnergy = (*iter).first;
    }                           
  }
  
  LogDebug("GeomInfo")
    << " max energy of " << maxEnergy 
    << " GeV in Unit id " << unitWithMaxEnergy;
  return unitWithMaxEnergy;
}


