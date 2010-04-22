#ifndef PhysicsTools_PatUtils_interface_MuonVPlusJetsIDSelectionFunctor_h
#define PhysicsTools_PatUtils_interface_MuonVPlusJetsIDSelectionFunctor_h

#include "DataFormats/PatCandidates/interface/Muon.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "PhysicsTools/SelectorUtils/interface/Selector.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <iostream>

class MuonVPlusJetsIDSelectionFunctor : public Selector<pat::Muon> {

 public: // interface

  enum Version_t { SUMMER08, FIRSTDATA, N_VERSIONS };


  MuonVPlusJetsIDSelectionFunctor( edm::ParameterSet const & parameters ){
    std::string versionStr = parameters.getParameter<std::string>("version");
    if ( versionStr == "SUMMER08" ) {
      initialize( SUMMER08, 
		  parameters.getParameter<double>("Chi2"),
		  parameters.getParameter<double>("D0")  ,
		  parameters.getParameter<int>   ("NHits")   ,
		  parameters.getParameter<double>("ECalVeto")   ,
		  parameters.getParameter<double>("HCalVeto")   ,
		  parameters.getParameter<double>("RelIso") );
      if ( parameters.exists("cutsToIgnore") )
	setIgnoredCuts( parameters.getParameter<std::vector<std::string> >("cutsToIgnore") );
	
    }
    else if ( versionStr == "FIRSTDATA" ) {
      initialize( FIRSTDATA, 
		  parameters.getParameter<double>("Chi2"),
		  parameters.getParameter<double>("D0")  ,
		  parameters.getParameter<double>("ED0")  ,
		  parameters.getParameter<double>("SD0")  ,
		  parameters.getParameter<int>   ("NHits")   ,
		  parameters.getParameter<double>("ECalVeto")   ,
		  parameters.getParameter<double>("HCalVeto")   ,
		  parameters.getParameter<double>("RelIso") );
      if ( parameters.exists("cutsToIgnore") )
	setIgnoredCuts( parameters.getParameter<std::vector<std::string> >("cutsToIgnore") );
	
    }
    else {
      throw cms::Exception("InvalidInput") << "Expect version to be one of SUMMER08, FIRSTDATA," << std::endl;
    }

    retInternal_ = getBitTemplate();
		
  }


  // compatibility with summer 08
  MuonVPlusJetsIDSelectionFunctor( Version_t version,
				   double chi2 = 10.0,
				   double d0 = 0.2,
				   int nhits = 11,
				   double ecalveto = 4.0,
				   double hcalveto = 6.0,
				   double reliso = 0.05
				   ) {
    if ( version != SUMMER08 ) {
      std::cout << "You are using the wrong version for MuonVPlusJetsIDSelectionFunctor!" << std::endl;
    }
    initialize( version, chi2, d0, 999.0, 999.0, nhits, ecalveto, hcalveto, reliso );
  }

  MuonVPlusJetsIDSelectionFunctor( Version_t version,
				   double chi2 = 10.0,
				   double d0 = 999.0,
				   double ed0 = 999.0,
				   double sd0 = 3.0,
				   int nhits = 11,
				   double ecalveto = 4.0,
				   double hcalveto = 6.0,
				   double reliso = 0.05
				   ) {
    initialize( version, chi2, d0, ed0, sd0, nhits, ecalveto, hcalveto, reliso );
  }
  

  void initialize( Version_t version,
		   double chi2 = 10.0,
		   double d0 = 999.0,
		   double ed0 = 999.0,
		   double sd0 = 3.0,
		   int nhits = 11,
		   double ecalveto = 4.0,
		   double hcalveto = 6.0,
		   double reliso = 0.05 )
  {
    version_ = version; 

    push_back("Chi2",      chi2   );
    push_back("D0",        d0     );
    push_back("ED0",       ed0    );
    push_back("SD0",       sd0    );
    push_back("NHits",     nhits  );
    push_back("ECalVeto",  ecalveto);
    push_back("HCalVeto",  hcalveto);
    push_back("RelIso",    reliso );

    set("Chi2");
    set("D0");
    set("ED0");
    set("SD0");
    set("NHits");
    set("ECalVeto");
    set("HCalVeto");
    set("RelIso");

    if ( version_ == FIRSTDATA ) {
      set("D0", false );
      set("ED0", false );
    } else if (version == SUMMER08 ) {
      set("SD0", false);
    }

  }

  // Allow for multiple definitions of the cuts. 
  bool operator()( const pat::Muon & muon, std::strbitset & ret ) 
  { 

    if ( version_ == SUMMER08 ) return summer08Cuts( muon, ret );
    else if ( version_ == FIRSTDATA ) return firstDataCuts( muon, ret );
    else {
      return false;
    }
  }

  using Selector<pat::Muon>::operator();

  // cuts based on craft 08 analysis. 
  bool summer08Cuts( const pat::Muon & muon, std::strbitset & ret)
  {

    ret.set(false);

    double norm_chi2 = muon.normChi2();
    double corr_d0 = muon.dB();
    int nhits = static_cast<int>( muon.numberOfValidHits() );
    
    double ecalVeto = muon.isolationR03().emVetoEt;
    double hcalVeto = muon.isolationR03().hadVetoEt;
	
    double hcalIso = muon.hcalIso();
    double ecalIso = muon.ecalIso();
    double trkIso  = muon.trackIso();
    double pt      = muon.pt() ;

    double relIso = (ecalIso + hcalIso + trkIso) / pt;

    if ( norm_chi2     <  cut("Chi2",   double()) || ignoreCut("Chi2")    ) passCut(ret, "Chi2"   );
    if ( fabs(corr_d0) <  cut("D0",     double()) || ignoreCut("D0")      ) passCut(ret, "D0"     );
    if ( nhits         >= cut("NHits",  int()   ) || ignoreCut("NHits")   ) passCut(ret, "NHits"  );
    if ( hcalVeto      <  cut("HCalVeto",double())|| ignoreCut("HCalVeto")) passCut(ret, "HCalVeto");
    if ( ecalVeto      <  cut("ECalVeto",double())|| ignoreCut("ECalVeto")) passCut(ret, "ECalVeto");
    if ( relIso        <  cut("RelIso", double()) || ignoreCut("RelIso")  ) passCut(ret, "RelIso" );

    setIgnored(ret);

    return (bool)ret;
  }



  // cuts based on craft 08 analysis. 
  bool firstDataCuts( const pat::Muon & muon, std::strbitset & ret)
  {

    ret.set(false);

    double norm_chi2 = muon.normChi2();
    double corr_d0 = muon.dB();
    double corr_ed0 = muon.edB();
    double corr_sd0 = ( corr_ed0 > 0.000000001 ) ? corr_d0 / corr_ed0 : 999.0;
    int nhits = static_cast<int>( muon.numberOfValidHits() );
    
    double ecalVeto = muon.isolationR03().emVetoEt;
    double hcalVeto = muon.isolationR03().hadVetoEt;
	
    double hcalIso = muon.hcalIso();
    double ecalIso = muon.ecalIso();
    double trkIso  = muon.trackIso();
    double pt      = muon.pt() ;

    double relIso = (ecalIso + hcalIso + trkIso) / pt;

    if ( norm_chi2     <  cut("Chi2",   double()) || ignoreCut("Chi2")    ) passCut(ret, "Chi2"   );
    if ( fabs(corr_d0) <  cut("D0",     double()) || ignoreCut("D0")      ) passCut(ret, "D0"     );
    if ( fabs(corr_ed0)<  cut("ED0",    double()) || ignoreCut("ED0")     ) passCut(ret, "ED0"    );
    if ( fabs(corr_sd0)<  cut("SD0",    double()) || ignoreCut("SD0")     ) passCut(ret, "SD0"    );
    if ( nhits         >= cut("NHits",  int()   ) || ignoreCut("NHits")   ) passCut(ret, "NHits"  );
    if ( hcalVeto      <  cut("HCalVeto",double())|| ignoreCut("HCalVeto")) passCut(ret, "HCalVeto");
    if ( ecalVeto      <  cut("ECalVeto",double())|| ignoreCut("ECalVeto")) passCut(ret, "ECalVeto");
    if ( relIso        <  cut("RelIso", double()) || ignoreCut("RelIso")  ) passCut(ret, "RelIso" );

    setIgnored(ret);
    
    return (bool)ret;
  }

  
 private: // member variables
  
  Version_t version_;
  
};

#endif
