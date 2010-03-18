#ifndef __PFBlockElementTrack__
#define __PFBlockElementTrack__

#include <iostream>

#include "DataFormats/Math/interface/Point3D.h"

#include "DataFormats/ParticleFlowReco/interface/PFBlockElement.h"
#include "DataFormats/ParticleFlowReco/interface/PFRecTrackFwd.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

namespace reco {
  
  /// \brief Track Element.
  /// 
  /// this class contains a reference to a PFRecTrack 
  class PFBlockElementTrack : public PFBlockElement {
  public:

    PFBlockElementTrack() {} 

    PFBlockElementTrack(const PFRecTrackRef& ref);

    PFBlockElement* clone() const { return new PFBlockElementTrack(*this); }
    
    void Dump(std::ostream& out = std::cout, 
              const char* tab = " " ) const;

    /// \return tracktype
    virtual bool trackType(TrackType trType) const { 
      return (trackType_>>trType) & 1; 
    }

    /// \set the trackType
    virtual void setTrackType(TrackType trType, bool value) {
      if(value)  trackType_ = trackType_ | (1<<trType);
      else trackType_ = trackType_ ^ (1<<trType);
    }


    /// set position at ECAL entrance
    void setPositionAtECALEntrance(float x, float y, float z) {
      positionAtECALEntrance_.SetCoordinates(x, y, z);
    }
 

    /// \return position at ECAL entrance
    const math::XYZPointF& positionAtECALEntrance() const {
      return positionAtECALEntrance_;
    }
    
    /// \return reference to the corresponding PFRecTrack
    /// please do not use this function after the block production stage!
    PFRecTrackRef trackRefPF() const { return trackRefPF_; }
    
    /// \return reference to the corresponding Track
    reco::TrackRef trackRef() const { return trackRef_; }

    /// check if the track is secondary
    bool isSecondary() const { 
      return 
	trackType(T_FROM_NUCL) || 
	trackType(T_FROM_GAMMACONV) || 
	trackType(T_FROM_V0); 
    }

    bool isPrimary() const{
      return trackType(T_TO_NUCL); 
    }

    /// \return the nuclear interaction associated
    PFDisplacedTrackerVertexRef displacedVertexRef(TrackType trType) const {
      if (trType == T_TO_NUCL)
	return displacedVertexDaughterRef_;
      else if (trType == T_FROM_NUCL)
	return displacedVertexMotherRef_;
      else return PFDisplacedTrackerVertexRef();
    }

    /// \set the ref to the nuclear interaction
    void setDisplacedVertexRef(const PFDisplacedTrackerVertexRef& niref, TrackType trType) { 

      if (trType == T_TO_NUCL) {
	displacedVertexDaughterRef_ = niref; setTrackType(trType,true);}
      else if (trType == T_FROM_NUCL) {
      	displacedVertexMotherRef_ = niref; setTrackType(trType,true);}
    } 
    
    /// \return reference to the corresponding Muon
    reco::MuonRef muonRef() const { return muonRef_; }

    /// \set reference to the Muon
    void setMuonRef(const MuonRef& muref) { 
      muonRef_=muref; setTrackType(MUON,true); 
    }

    /// \return ref to original recoConversion
    ConversionRef convRef() const {return convRef_;} 

    /// \set the ref to  gamma conversion
    void setConversionRef(const ConversionRef& convRef, TrackType trType) { 
      convRef_ = convRef; setTrackType(trType,true); 
    } 

    /// \return ref to original V0
    VertexCompositeCandidateRef V0Ref() const {return v0Ref_;} 

    /// \set the ref to  V0
    void setV0Ref(const VertexCompositeCandidateRef& V0Ref, TrackType trType) { 
      v0Ref_ = V0Ref; setTrackType(trType,true); 
    } 


    
  private:

    /// reference to the corresponding track (transient)
    PFRecTrackRef  trackRefPF_;

    /// reference to the corresponding track 
    reco::TrackRef trackRef_;

    unsigned int  trackType_;

    /// position at ECAL entrance
    math::XYZPointF        positionAtECALEntrance_;
    
    /// reference to the corresponding pf nuclear interaction
    PFDisplacedTrackerVertexRef  displacedVertexMotherRef_;

    PFDisplacedTrackerVertexRef  displacedVertexDaughterRef_;
                                 
    /// reference to the corresponding muon
    reco::MuonRef muonRef_;

    /// reference to reco conversion
    ConversionRef convRef_;      

    /// reference to V0
    VertexCompositeCandidateRef v0Ref_;
  };
}

#endif

