import FWCore.ParameterSet.Config as cms

# Tracking particle module
from SimGeneral.TrackingAnalysis.trackingParticles_cfi import *
mergedtruth.simHitCollections = cms.PSet(
    tracker = cms.vstring("famosSimHitsTrackerHits"),
    muon = cms.vstring (
        'MuonSimHitsMuonDTHits',
        'MuonSimHitsMuonCSCHits',
        'MuonSimHitsMuonRPCHits'            
    )
)
mergedtruth.simHitLabel = 'famosSimHits'
mergedtruth.removeDeadModules = cms.bool(False)

from Validation.RecoMET.METRelValForDQM_cff import *

from Validation.TrackingMCTruth.trackingTruthValidation_cfi import *
from Validation.RecoTrack.TrackValidation_fastsim_cff import *
###must be commented for automatic RelVal running
###multiTrackValidator.outputFile='valPlots_fastsim.root'

from Validation.RecoMuon.muonValidationFastSim_cff import *
from Validation.MuonIsolation.MuIsoVal_cff import *
from Validation.MuonIdentification.muonIdVal_cff import *

from DQMOffline.RecoB.dqmAnalyzer_cff import *

#from PhysicsTools.JetMCAlgos.CaloJetsMCFlavour_cfi import * 
#from Validation.RecoB.bTagAnalysis_cfi import *
#bTagValidation.jetMCSrc = 'IC5byValAlgo'
#bTagValidation.etaRanges = cms.vdouble(0.0, 1.1, 2.4)

globalValidation = cms.Sequence(trackingParticles
                                +trackingTruthValid
                                +tracksValidation
                                +METRelValSequence
                                +recoMuonValidationFastSim
                                +muIsoVal_seq
                                +muonIdValDQMSeq
                                +bTagPlots
                               # +myPartons
                               # +iterativeCone5Flavour
                               # +bTagValidation
                                )

globalValidation_preprod = cms.Sequence(trackingParticles
                                +trackingTruthValid
                                +tracksValidation
                                +METRelValSequence
                                +recoMuonValidationFastSim
                                +muIsoVal_seq
                                +muonIdValDQMSeq
                                )
