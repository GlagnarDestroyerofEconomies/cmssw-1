import FWCore.ParameterSet.Config as cms

process = cms.Process("TEST")

process.PoolDBESSource = cms.ESSource("PoolDBESSource",
    loadAll = cms.bool(True),
    timetype = cms.string('runnumber'),
    toGet = cms.VPSet(
                cms.PSet( record = cms.string('DTTtrigRcd'),
                          tag = cms.string('tTrig_test') )
    ),
    connect = cms.string('sqlite_file:test.db'),
    DBParameters = cms.PSet(
        messageLevel = cms.untracked.int32(0),
        authenticationPath = cms.untracked.string('.')
    )
)

process.source = cms.Source("EmptySource",
    firstRun  = cms.untracked.uint32(1),
    numberEventsInRun  = cms.untracked.uint32(1)
)

process.get = cms.EDAnalyzer("DTTtrigPrint")

process.p = cms.Path(process.get)


