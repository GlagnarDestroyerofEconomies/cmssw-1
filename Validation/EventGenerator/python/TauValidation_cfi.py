import FWCore.ParameterSet.Config as cms

tauValidation = cms.EDAnalyzer("TauValidation",
    hepmcCollection = cms.InputTag("generator","")
)
