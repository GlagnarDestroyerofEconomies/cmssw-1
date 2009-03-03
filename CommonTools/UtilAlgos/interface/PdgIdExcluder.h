#ifndef CommonTools_UtilAlgos_PdgIdSelector_h
#define CommonTools_UtilAlgos_PdgIdSelector_h
#include "CommonTools/UtilAlgos/interface/ParameterAdapter.h"
#include "CommonTools/Utils/interface/PdgIdExcluder.h"

namespace reco {
  namespace modules {
    
    template<>
      struct ParameterAdapter<PdgIdExcluder> {
	static PdgIdExcluder make( const edm::ParameterSet & cfg ) {
	  return PdgIdExcluder( cfg.getParameter<std::vector<int> >( "pdgId" ) );
	}
      };

  }
}

#endif
