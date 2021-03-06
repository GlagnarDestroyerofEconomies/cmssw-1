#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/PluginManager/interface/ModuleDef.h"

#include "ElectroWeakAnalysis/EWKTau/interface/ETauAnalyzer.h"
#include "ElectroWeakAnalysis/EWKTau/interface/MuonHistManager.h"
#include "ElectroWeakAnalysis/EWKTau/interface/TauHistManager.h"
//#include "ElectroWeakAnalysis/EWKTau/interface/WToTauNuFilter.h"
#include "ElectroWeakAnalysis/EWKTau/interface/DiscriminationByLdgTrackProd.h"
#include "ElectroWeakAnalysis/EWKTau/interface/DiscriminationByMassWithElec.h"
#include "ElectroWeakAnalysis/EWKTau/interface/TauVetoFilter.h"
#include "ElectroWeakAnalysis/EWKTau/interface/PATLeptonPATMetDelPhiSelector.h"
#include "ElectroWeakAnalysis/EWKTau/interface/PATLeptonPFParticleIsolationSelector.h"
#include "ElectroWeakAnalysis/EWKTau/interface/PATLeptonSVAssociatorSelector.h"
#include "ElectroWeakAnalysis/EWKTau/interface/PATLeptonPATTauSeperationSelector.h"
#include "ElectroWeakAnalysis/EWKTau/interface/EWKTauIDAnalyzer.h"

DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_MODULE(ETauAnalyzer);
DEFINE_ANOTHER_FWK_MODULE(MuonHistManager);
DEFINE_ANOTHER_FWK_MODULE(TauHistManager);
DEFINE_ANOTHER_FWK_MODULE(DiscriminationByLdgTrackProd);
DEFINE_ANOTHER_FWK_MODULE(DiscriminationByMassWithElec);
DEFINE_ANOTHER_FWK_MODULE(TauVetoFilter);
DEFINE_ANOTHER_FWK_MODULE(PATLeptonPATMetDelPhiSelector);
DEFINE_ANOTHER_FWK_MODULE(PATLeptonPFParticleIsolationSelector);
DEFINE_ANOTHER_FWK_MODULE(PATLeptonSVAssociatorSelector);
DEFINE_ANOTHER_FWK_MODULE(PATLeptonPATTauSeperationSelector);
DEFINE_ANOTHER_FWK_MODULE(EWKTauIDAnalyzer);
//DEFINE_ANOTHER_FWK_MODULE(WToTauNuFilter);
