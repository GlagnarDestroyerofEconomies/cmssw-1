import FWCore.ParameterSet.Config as cms

ecalEndcapDcsInfoTask = cms.EDAnalyzer("EEDcsInfoTask",
    prefixME = cms.untracked.string('Ecal'),
    enableCleanup = cms.untracked.bool(False),
    mergeRuns = cms.untracked.bool(False)
)
