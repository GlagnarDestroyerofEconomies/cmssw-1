#include "DataFormats/Common/interface/Wrapper.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonExtra.h"
#include <vector>

namespace {
  namespace {
    std::vector<reco::MuonExtra> v2;
    edm::Wrapper<std::vector<reco::MuonExtra> > c2;
    edm::Ref<std::vector<reco::MuonExtra> > r2;
    edm::RefVector<std::vector<reco::MuonExtra> > rv2;

    reco::TrackExtension<reco::MuonExtraRef> te1;

    std::vector<reco::Muon> v1;
    edm::Wrapper<std::vector<reco::Muon> > c1;
    edm::Ref<std::vector<reco::Muon> > r1;
    edm::RefVector<std::vector<reco::Muon> > rv1;
  }
}
