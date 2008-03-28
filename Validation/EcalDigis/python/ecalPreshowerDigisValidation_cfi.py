import FWCore.ParameterSet.Config as cms

ecalPreshowerDigisValidation = cms.EDFilter("EcalPreshowerDigisValidation",
    ESdigiCollection = cms.InputTag("ecalPreshowerDigis"),
    verbose = cms.untracked.bool(True)
)


