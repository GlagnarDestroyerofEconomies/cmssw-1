import FWCore.ParameterSet.Config as cms

l1tcsctf = cms.EDAnalyzer("L1TCSCTF",
    gmtProducer = cms.InputTag("gtDigis::DQM"),

    statusProducer = cms.InputTag("csctfDigis"),
    outputFile = cms.untracked.string(''),
    lctProducer = cms.InputTag("csctfDigis"),
    verbose = cms.untracked.bool(False),
    trackProducer = cms.InputTag("csctfDigis"),
    mbProducer = cms.InputTag("csctfDigis:DT"),
    DQMStore = cms.untracked.bool(True),
    disableROOToutput = cms.untracked.bool(True)
)


