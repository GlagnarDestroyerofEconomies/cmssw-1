//
// $Id: LeptonVertexSignificance.h,v 1.1 2008/01/15 13:20:54 lowette Exp $
//

#ifndef PhysicsTools_PatUtils_LeptonVertexSignificance_h
#define PhysicsTools_PatUtils_LeptonVertexSignificance_h

/**
  \class    LeptonVertexSignificance LeptonVertexSignificance.h "PhysicsTools/PatUtils/interface/LeptonVertexSignificance.h"
  \brief    Calculates a lepton's vertex association significance

   LeptonVertexSignificance calculates the significance of the association
   of the lepton to a given vertex, as defined in CMS Note 2006/024

  \author   Steven Lowette
  \version  $Id: LeptonVertexSignificance.h,v 1.1 2008/01/15 13:20:54 lowette Exp $
*/

class TransientTrackBuilder;

namespace reco {
  class Track;
}

namespace edm {
  class Event;
  class EventSetup;
}

namespace pat {
  class Electron;
  class Muon;

  class LeptonVertexSignificance {
  public:
    LeptonVertexSignificance();
    LeptonVertexSignificance(const edm::EventSetup & iSetup);
    ~LeptonVertexSignificance();
    
    float calculate(const Electron & anElectron, const edm::Event & iEvent);
    float calculate(const Muon & aMuon, const edm::Event & iEvent);
    
  private:
    float calculate(const reco::Track & track, const edm::Event & iEvent);
    TransientTrackBuilder * theTrackBuilder_;
  };

}

#endif

