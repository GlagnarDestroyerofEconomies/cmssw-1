#ifndef PhotonMCTruth_h
#define PhotonMCTruth_h

#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "SimDataFormats/Track/interface/SimTrack.h"
#include <CLHEP/Matrix/Vector.h>
#include <CLHEP/Vector/LorentzVector.h>
#include <vector>

/** \class PhotonMCTruth
 *       
 *  This class stores all the MC truth information needed about the
 *  conversion
 * 
 *  $Date: 2007/06/08 10:53:08 $
 *  $Revision: 1.7 $
 *  \author N. Marinelli  University of Notre Dame
 *
 */



class ElectronMCTruth;
class PhotonMCTruth {
public:
  PhotonMCTruth() : isAConversion_(0),thePhoton_(0.,0.,0.), 
                       theConvVertex_(0.,0.,0.) {};

  PhotonMCTruth(HepLorentzVector v) : thePhoton_(v) {};


  PhotonMCTruth(int isAConversion,
		HepLorentzVector v,
                int vertIndex,
                int trackId,
                int motherId,
		HepLorentzVector mothMom, 
		HepLorentzVector mothVtx, 
		HepLorentzVector convVertex, 
		HepLorentzVector pV, 
		std::vector<ElectronMCTruth>& electrons );
  

 HepLorentzVector primaryVertex() const {return thePrimaryVertex_;}
 int isAConversion() const { return isAConversion_;}
 HepLorentzVector fourMomentum() const {return thePhoton_;}
 int vertexInd() const {return theVertexIndex_;}
 HepLorentzVector vertex() const {return theConvVertex_;}
 std::vector<ElectronMCTruth> electrons() const {return theElectrons_;} 
 int trackId() const {return theTrackId_;}  
 int motherType() const {return theMotherId_;}
 HepLorentzVector motherMomentum() const {return theMotherMom_;}
 HepLorentzVector motherVtx() const {return theMotherVtx_;}

 private:

  int isAConversion_;
  HepLorentzVector thePhoton_;
  int theVertexIndex_;
  int theTrackId_;
  int theMotherId_;
  HepLorentzVector theMotherMom_;
  HepLorentzVector theMotherVtx_;
  HepLorentzVector theConvVertex_;
  HepLorentzVector thePrimaryVertex_;
  std::vector<ElectronMCTruth> theElectrons_;

};

#endif

