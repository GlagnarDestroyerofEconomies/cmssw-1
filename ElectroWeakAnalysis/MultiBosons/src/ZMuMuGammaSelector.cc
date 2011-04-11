#include "ElectroWeakAnalysis/MultiBosons/interface/ZMuMuGammaSelector.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "CommonTools/UtilAlgos/interface/DeltaR.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

ZMuMuGammaSelector::ZMuMuGammaSelector( const edm::ParameterSet& conf ) {

  std::string version(conf.getParameter<std::string>("version"));

  if ( version == "Fsr2011Apr11" )
    version_ = Fsr2011Apr11;
  else
    throw cms::Exception("InvalidInput")
      << "Received version `" << version << "'" << std::endl
      << "The only supported \'version\' is Fsr2011Apr11." << std::endl;

  switch(version_) {
    case Fsr2011Apr11:
      init_Fsr2011Apr11(
        // 1. maximum near muon HCAL isolation
        conf.getParameter<double>("maxNearMuonHcalIso"),
        // 2. maximum far muon ECAL isolation
        conf.getParameter<double>("maxFarMuonEcalIso"),
        // 3. maximum Delta R distance between the photon and the near muon
        conf.getParameter<double>("maxDeltaRNear"),
        // 4. minimum far muon transverse momentum
        conf.getParameter<double>("minFarMuonPt"),
        // 5. minimum invariant mass
        conf.getParameter<double>("minMass"),
        // 6. maximum invariant mass
        conf.getParameter<double>("maxMass")
      );
      break;
    case N_VERSIONS:
    default:
      throw cms::Exception("IllegalEnumValue")
        << "This should have been thrown previously!!!" << std::endl;
  } // end of switch(version_)

  if( conf.exists("cutToIgnore") )
    setIgnoredCuts( conf.getParameter<std::vector<std::string> >("cutsToIgnore") );

  retInternal_ = getBitTemplate();
}

void ZMuMuGammaSelector::init_Fsr2011Apr11(
  // 1. maximum near muon HCAL isolation
  const double& maxNearMuonHcalIso,
  // 2. maximum far muon ECAL isolation
  const double& maxFarMuonEcalIso,
  // 3. maximum Delta R distance between the photon and the near muon
  const double& maxDeltaRNear,
  // 4. minimum far muon transverse momentum
  const double& minFarMuonPt,
  // 5. minimum invariant mass
  const double& minMass,
  // 6. maximum invariant mass
  const double& maxMass
) {
  push_back("inclusive");
  push_back("maxNearMuonHcalIso", maxNearMuonHcalIso);
  push_back("maxFarMuonEcalIso", maxFarMuonEcalIso);
  push_back("maxDeltaRNear", maxDeltaRNear);
  push_back("minFarMuonPt", minFarMuonPt);
  push_back("minMass", minMass);
  push_back("maxMass", maxMass);

  set("inclusive");
  set("maxNearMuonHcalIso");
  set("maxFarMuonEcalIso");
  set("maxDeltaRNear");
  set("minFarMuonPt");
  set("minMass");
  set("maxMass");
} // end of ZMuMuGammaSelector::init_Fsr2011Apr11(...)

bool ZMuMuGammaSelector::operator()(
  const reco::CompositeCandidate & mmgCand,
  pat::strbitset &  ret
  )
{
  switch (version_) {
  case Fsr2011Apr11: return passes_Fsr2011Apr11(mmgCand, ret);
  case N_VERSIONS:
  default:
    throw cms::Exception("IllegalEnumValue")
      << "This should have been thrown previously!!!" << std::endl;
  }
  return false;
}

bool ZMuMuGammaSelector::passes_Fsr2011Apr11(
  const reco::CompositeCandidate& mmgCand,
  pat::strbitset & ret
  )
{
  ret.set(false);
  setIgnored(ret);

  // 0. all mmgCands
  passCut(ret, "inclusive");

  // Extract the daughters
  const pat::Photon * photon;
  const reco::CompositeCandidate * dimuon;
  const pat::Muon * muon1;
  const pat::Muon * muon2;
  const pat::Muon * nearMuon;
  const pat::Muon * farMuon;

  // TODO add some checks whether Ptrs are valid
  photon = (const pat::Photon*) mmgCand.daughter("photon")->masterClonePtr().get();
  dimuon = (const reco::CompositeCandidate*) mmgCand.daughter("dimuon");
  muon1 = (const pat::Muon*) dimuon->daughter("muon1")->masterClonePtr().get();
  muon2 = (const pat::Muon*) dimuon->daughter("muon2")->masterClonePtr().get();

  // Decide which muon is near and which is far
  DeltaR<pat::Muon, pat::Photon> deltaR;
  double dr1 = deltaR(*muon1, *photon);
  double dr2 = deltaR(*muon2, *photon);
  double drNear = dr1;

  if (dr1 < dr2) {
    nearMuon = muon1; farMuon  = muon2; drNear = dr1;
  } else {
    nearMuon = muon2; farMuon  = muon1; drNear = dr2;
  }

  // Apply cut 1. maximum near muon HCAL isolation
  LogDebug("HcalIso") << "nearMuon->hcalIso: " << nearMuon->hcalIso();
  LogDebug("HcalIso") << "nearMuon->isolationR03().hadEt: " << nearMuon->isolationR03().hadEt;
//   if (nearMuon->hcalIso() < cut("maxNearMuonHcalIso", double()) ||
  if (nearMuon->isolationR03().hadEt < cut("maxNearMuonHcalIso", double()) ||
      ignoreCut("maxNearMuonHcalIso")
      )
    passCut(ret, "maxNearMuonHcalIso");
  else return false;

  // Apply cut 2. maximum far muon ECAL isolation
  if (farMuon->ecalIso() < cut("maxFarMuonEcalIso", double()) ||
      ignoreCut("maxFarMuonEcalIso")
      )
    passCut(ret, "maxFarMuonEcalIso");
  else return false;

  // Apply cut 3. maximum Delta R distance between the photon and the near muon
  if (drNear < cut("maxDeltaRNear", double()) ||
      ignoreCut("maxDeltaRNear")
      )
    passCut(ret, "maxDeltaRNear");
  else return false;

  // Apply cut 4. minimum far muon transverse momentum
  if (farMuon->pt() > cut("minFarMuonPt", double()) ||
      ignoreCut("minFarMuonPt")
      )
    passCut(ret, "minFarMuonPt");
  else return false;

  // Apply cut 5. minimum invariant mass
  if (mmgCand.mass() > cut("minMass", double()) ||
      ignoreCut("minMass")
      )
    passCut(ret, "minMass");
  else return false;

  // Apply cut 6. maximum invariant mass
  if (mmgCand.mass() < cut("maxMass", double()) ||
      ignoreCut("maxMass")
      )
    passCut(ret, "maxMass");
  else return false;

  return  (bool) ret;
} // ZMuMuGammaSelector::passes_Fsr2011Apr11Cuts(...)
