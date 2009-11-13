#include "RecoMET/METAlgorithms/interface/GlobalHaloAlgo.h"
/*
  [class]:  GlobalHaloAlgo
  [authors]: R. Remington, The University of Florida
  [description]: See GlobalHaloAlgo.h
  [date]: October 15, 2009
*/
using namespace std;
using namespace edm;
using namespace reco;

int Phi_To_iPhi(float phi) 
{
  phi = phi < 0 ? phi + 2.*TMath::Pi() : phi ;
  float phi_degrees = phi * (360.) / ( 2. * TMath::Pi() ) ;
  int iPhi = (int) ( ( phi_degrees/5. ) + 1.);
   
  return iPhi < 73 ? iPhi : 73 ;
}

GlobalHaloAlgo::GlobalHaloAlgo()
{
  // Defaults are "loose"
  Ecal_R_Min = 110.;   // Tight: 200.
  Ecal_R_Max = 330.;   // Tight: 250. 
  Hcal_R_Min = 110.;   // Tight: 220.
  Hcal_R_Max = 490.;   // Tight: 350.
  
}

reco::GlobalHaloData GlobalHaloAlgo::Calculate(const CaloGeometry& TheCaloGeometry, const CSCGeometry& TheCSCGeometry, const reco::CaloMET& TheCaloMET, edm::Handle< edm::View<Candidate> >& TheCaloTowers, edm::Handle<CSCSegmentCollection>& TheCSCSegments, edm::Handle<CSCRecHit2DCollection>& TheCSCRecHits,  const CSCHaloData& TheCSCHaloData, const EcalHaloData& TheEcalHaloData, const HcalHaloData& TheHcalHaloData)
{
  
  GlobalHaloData TheGlobalHaloData;
  float METOverSumEt = TheCaloMET.sumEt() ? TheCaloMET.pt() / TheCaloMET.sumEt() : 0 ;
  TheGlobalHaloData.SetMETOverSumEt(METOverSumEt);

  int EcalOverlapping_CSCRecHits[73];
  int EcalOverlapping_CSCSegments[73];
  int HcalOverlapping_CSCRecHits[73];
  int HcalOverlapping_CSCSegments[73];

  if( TheCSCSegments.isValid() )
    {
      for(CSCSegmentCollection::const_iterator iSegment = TheCSCSegments->begin(); iSegment != TheCSCSegments->end(); iSegment++) 
	{
	  bool EcalOverlap[73];
	  bool HcalOverlap[73];
	  for( int i = 0 ; i < 73 ; i++ ) 
	    {
	      EcalOverlap[i] = false;
	      HcalOverlap[i] = false;
	    }
	  
	  std::vector<CSCRecHit2D> Hits = iSegment->specificRecHits() ;
	  for(std::vector<CSCRecHit2D>::iterator iHit = Hits.begin() ; iHit != Hits.end(); iHit++ )
	    {
	      DetId TheDetUnitId(iHit->geographicalId());
	      if( TheDetUnitId.det() != DetId::Muon ) continue;
	      if( TheDetUnitId.subdetId() != MuonSubdetId::CSC ) continue;

	      const GeomDetUnit *TheUnit = TheCSCGeometry.idToDetUnit(TheDetUnitId);
	      LocalPoint TheLocalPosition = iHit->localPosition();  
	      const BoundPlane& TheSurface = TheUnit->surface();
	      const GlobalPoint TheGlobalPosition = TheSurface.toGlobal(TheLocalPosition);

	      int iphi = Phi_To_iPhi( TheGlobalPosition.phi() ) ;
	      float x = TheGlobalPosition.x(); 
	      float y = TheGlobalPosition.y();
	      
	      float r = TMath::Sqrt( x*x + y*y);
	      
	      if( r < Ecal_R_Max && r > Ecal_R_Min )
		EcalOverlap[iphi] = true;
	      if( r < Hcal_R_Max && r > Hcal_R_Max ) 
		HcalOverlap[iphi] = true;
	    }
	  for( int i = 0 ; i < 73 ; i++ ) 
	    {
	      if( EcalOverlap[i] )  EcalOverlapping_CSCSegments[i]++;
	      if( HcalOverlap[i] )  HcalOverlapping_CSCSegments[i]++;
	    }
	} 
    }
  if( TheCSCRecHits.isValid() )
    {
      for(CSCRecHit2DCollection::const_iterator iCSCRecHit = TheCSCRecHits->begin();   iCSCRecHit != TheCSCRecHits->end(); iCSCRecHit++ )
	{
	  
	  DetId TheDetUnitId(iCSCRecHit->geographicalId());
	  if( TheDetUnitId.det() != DetId::Muon ) continue;
	  if( TheDetUnitId.subdetId() != MuonSubdetId::CSC ) continue;
	  
	  const GeomDetUnit *TheUnit = TheCSCGeometry.idToDetUnit(TheDetUnitId);
	  LocalPoint TheLocalPosition = iCSCRecHit->localPosition();  
	  const BoundPlane& TheSurface = TheUnit->surface();
	  const GlobalPoint TheGlobalPosition = TheSurface.toGlobal(TheLocalPosition);
	  
	  int iphi = Phi_To_iPhi( TheGlobalPosition.phi() ) ;
	  float x = TheGlobalPosition.x(); 
	  float y = TheGlobalPosition.y();
	  
	  float r = TMath::Sqrt(x*x + y*y);
	  
	  if( r < Ecal_R_Max && r > Ecal_R_Min )
	    EcalOverlapping_CSCRecHits[iphi] ++;
	  if( r < Hcal_R_Max && r > Hcal_R_Max ) 
	    HcalOverlapping_CSCRecHits[iphi] ++ ;
	}
    }  

  // In development....
  // Get Ecal Wedges
  std::vector<PhiWedge> EcalWedges = TheEcalHaloData.GetPhiWedges();
  
  // Get Hcal Wedges
  std::vector<PhiWedge> HcalWedges = TheHcalHaloData.GetPhiWedges();
  
  //Get Ref to CSC Tracks
  //edm::RefVector<reco::TrackCollection> TheCSCTracks = TheCSCHaloData.GetTracks();
  //for(unsigned int i = 0 ; i < TheCSCTracks.size() ; i++ )
  //edm::Ref<reco::TrackCollection> iTrack( TheCSCTracks, i );
  
  // Get global positions of central most rechit of CSC Halo tracks
  std::vector<GlobalPoint> TheGlobalPositions = TheCSCHaloData.GetCSCTrackImpactPositions();

  // Container to store Ecal/Hcal iPhi values matched to impact point of CSC tracks
  std::vector<int> EcaliPhi, HcaliPhi;

  // Keep track of number of calo pointing CSC halo tracks that do not match to Phi wedges
  int N_Unmatched_Tracks = 0;  
  
  for( std::vector<GlobalPoint>::iterator Pos = TheGlobalPositions.begin() ; Pos != TheGlobalPositions.end() ; Pos ++ ) 
    {
      // Calculate global phi coordinate for central most rechit in the track
      float global_phi = Pos->phi();
      float global_r = TMath::Sqrt(Pos->x()*Pos->x() + Pos->y()*Pos->y());
      
      // Convert global phi to iPhi
      global_phi = global_phi < 0 ? global_phi + 2.*TMath::Pi() : global_phi ;
      float global_phi_degrees = global_phi * (360.) / ( 2. * TMath::Pi() ) ;
      int global_iPhi = (int) ( ( global_phi_degrees / 5. ) + 1.);
      
      bool MATCHED = false;
      
      //Loop over Ecal Phi Wedges 
      for( std::vector<PhiWedge>::iterator iWedge = EcalWedges.begin() ; iWedge != EcalWedges.end() ; iWedge++ )
	{
	  if( (TMath::Abs( global_iPhi - iWedge->iPhi() ) <= 2 ) && (global_r >  Ecal_R_Min && global_r < Ecal_R_Max ) )
	    {
	      bool StoreWedge = true;
	      for( unsigned int i = 0 ; i< EcaliPhi.size() ; i++ ) if ( EcaliPhi[i] == iWedge->iPhi() ) StoreWedge = false;
	      
	      if( StoreWedge ) 
		{
		  PhiWedge NewWedge(*iWedge);
		  NewWedge.SetOverlappingCSCSegments( EcalOverlapping_CSCSegments[iWedge->iPhi()] );
		  NewWedge.SetOverlappingCSCRecHits( EcalOverlapping_CSCRecHits[iWedge->iPhi()] );
		  EcaliPhi.push_back( iWedge->iPhi() );
		  TheGlobalHaloData.GetMatchedEcalPhiWedges().push_back( NewWedge );
		}
	      MATCHED = true;
	    }
	}
      //Loop over Hcal Phi Wedges 
      for( std::vector<PhiWedge>::iterator iWedge = HcalWedges.begin() ; iWedge != HcalWedges.end() ; iWedge++ )
	{
	  if(  (TMath::Abs( global_iPhi - iWedge->iPhi() ) <=  2 ) && (global_r > Hcal_R_Min && global_r < Hcal_R_Max ) )
	    {
	      bool StoreWedge  = true;
	      for( unsigned int i = 0 ; i < HcaliPhi.size() ; i++ ) if(  HcaliPhi[i] == iWedge->iPhi() ) StoreWedge = false;
	      
	      if( StoreWedge ) 
		{
		  HcaliPhi.push_back( iWedge->iPhi() ) ;
		  PhiWedge NewWedge(*iWedge);
		  NewWedge.SetOverlappingCSCSegments( HcalOverlapping_CSCSegments[iWedge->iPhi()] );
		  NewWedge.SetOverlappingCSCRecHits( HcalOverlapping_CSCRecHits[iWedge->iPhi()] );		  
		  PhiWedge wedge(*iWedge);
		  TheGlobalHaloData.GetMatchedHcalPhiWedges().push_back( NewWedge ) ; 
		}
	      MATCHED = true;
	    }
	}
      if( !MATCHED ) N_Unmatched_Tracks ++;
    }
  
  // Corrections to MEx, MEy
  float dMEx = 0.; 
  float dMEy = 0.;
  // Loop over calotowers and correct the MET for the towers that lie in the trajectory of the CSC Halo Tracks
  for( edm::View<Candidate>::const_iterator iCandidate = TheCaloTowers->begin() ; iCandidate != TheCaloTowers->end() ; iCandidate++ )
    {
      const Candidate* c = &(*iCandidate);
      if ( c )
	{
	  const CaloTower* iTower = dynamic_cast<const CaloTower*> (c);
	  if( iTower->et() < TowerEtThreshold ) continue;
	  if( abs(iTower->ieta()) > 24 )  continue;   // not in barrel/endcap
	  int iphi = iTower->iphi();
	  for( unsigned int x = 0 ; x < EcaliPhi.size() ; x++ )
	    {
	      if( iphi == EcaliPhi[x] ) 
		{
		  dMEx += ( TMath::Cos(iTower->phi())*iTower->emEt() );
		  dMEy += ( TMath::Sin(iTower->phi())*iTower->emEt() );
		}
	    }
	  for( unsigned int x = 0 ; x < HcaliPhi.size() ; x++ )
	    {
	      if( iphi == HcaliPhi[x] ) 
		{
		  dMEx += ( TMath::Cos(iTower->phi() )*iTower->hadEt() ) ;
		  dMEy += ( TMath::Sin(iTower->phi() )*iTower->hadEt() ) ;
		}
	    }
	}
    }
  
  TheGlobalHaloData.SetMETCorrections(dMEx, dMEy);
  return TheGlobalHaloData;
}
