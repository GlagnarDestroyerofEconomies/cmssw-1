#include "FWCore/Framework/interface/MakerMacros.h"
#include "CommonTools/UtilAlgos/interface/StringCutObjectSelector.h"
#include "CommonTools/CandAlgos/interface/CandCombiner.h"

namespace reco {
  namespace modules {
    
    typedef reco::modules::CandCombiner<
      StringCutObjectSelector<reco::Candidate,true>,
      AnyPairSelector,
      combiner::helpers::ShallowClonePtr
      > LazyCandViewShallowClonePtrCombiner;
    
    DEFINE_FWK_MODULE( LazyCandViewShallowClonePtrCombiner );
  }
}
