import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------
# LOCAL RECO

# Tracker
from RecoVertex.BeamSpotProducer.BeamSpot_cfi import *
from RecoLocalTracker.Configuration.RecoLocalTracker_cff import *

# Ecal
from RecoLocalCalo.Configuration.ecalLocalRecoSequence_cff import *

# Hcal
from RecoLocalCalo.Configuration.hcalLocalReco_cff import *

# Muons
from RecoLocalMuon.Configuration.RecoLocalMuon_cff import *

#--------------------------------------------------------------------------
# HIGH LEVEL RECO

# Tracking
#from RecoHI.HiTracking.HighPtTracking_PbPb_cff import *  # above 1.5 GeV
from RecoHI.HiTracking.LowPtTracking_PbPb_cff import *    # above 0.9 GeV

# Egamma
from RecoHI.HiEgammaAlgos.HiEgamma_cff import *

# Jet Reconstruction
from RecoHI.HiJetAlgos.IterativeConePu5Jets_PbPb_cff import *

# Muon Reco
from RecoLuminosity.LumiProducer.lumiProducer_cfi import *
from RecoHI.HiMuonAlgos.HiRecoMuon_cff import * 

# Heavy Ion Event Characterization
from RecoHI.HiCentralityAlgos.HiCentrality_cfi import *
from RecoHI.HiEvtPlaneAlgos.HiEvtPlane_cfi import *

#--------------------------------------------------------------------------

caloReco = cms.Sequence(ecalLocalRecoSequence*hcalLocalRecoSequence)
muonReco = cms.Sequence(trackerlocalreco+muonlocalreco+lumiProducer)
localReco = cms.Sequence(offlineBeamSpot*muonReco*caloReco)

#--------------------------------------------------------------------------
# Main Sequence

reconstruct_PbPb = cms.Sequence(localReco*heavyIonTracking*muontracking_with_TeVRefinement*hiEcalClusters*runjets*hiEgammaSequence*hiCentrality*hiEvtPlane)
reconstruct_PbPb_CaloOnly = cms.Sequence(caloReco*hiEcalClusters*runjets*hiCentrality*hiEvtPlane)
reconstruct_PbPb_MuonOnly = cms.Sequence(offlineBeamSpot*muonReco*heavyIonTracking*muontracking_with_TeVRefinement)

#--------------------------------------------------------------------------

