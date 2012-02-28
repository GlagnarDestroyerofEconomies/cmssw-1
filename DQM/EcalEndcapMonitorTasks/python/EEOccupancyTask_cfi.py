import FWCore.ParameterSet.Config as cms

ecalEndcapOccupancyTask = cms.EDAnalyzer("EEOccupancyTask",
    prefixME = cms.untracked.string('Ecal'),
    enableCleanup = cms.untracked.bool(False),
    mergeRuns = cms.untracked.bool(False),    
    EcalRawDataCollection = cms.InputTag("ecalEBunpacker"),
    EEDigiCollection = cms.InputTag("ecalEBunpacker","eeDigis"),
    EcalTrigPrimDigiCollection = cms.InputTag("ecalEBunpacker","EcalTriggerPrimitives"),
    EcalPnDiodeDigiCollection = cms.InputTag("ecalEBunpacker"),
    EcalRecHitCollection = cms.InputTag("ecalRecHit","EcalRecHitsEE")
)

