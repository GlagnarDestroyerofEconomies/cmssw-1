import FWCore.ParameterSet.Config as cms
process = cms.Process("TEST")

process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.GlobalTag.globaltag = 'IDEAL_30X::All'

process.load("RecoMuon.MuonIdentification.links_cfi")
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)
process.MessageLogger = cms.Service("MessageLogger",
    destinations = cms.untracked.vstring('cout'),
    categories = cms.untracked.vstring('MuonIdentification','TrackAssociator'),
    cout = cms.untracked.PSet(
        # threshold = cms.untracked.string('INFO')
        threshold = cms.untracked.string('DEBUG'),
	# noTimeStamps = cms.untracked.bool(True),
	# noLineBreaks = cms.untracked.bool(True)
	DEBUG = cms.untracked.PSet(
           limit = cms.untracked.int32(0)
	   ),
	MuonIdentification = cms.untracked.PSet(
	   limit = cms.untracked.int32(-1)
	),
	TrackAssociator = cms.untracked.PSet(
	   limit = cms.untracked.int32(-1)
	),
    ),
    debugModules = cms.untracked.vstring("muons")
)

import FWCore.Framework.test.cmsExceptionsFatalOption_cff
options = cms.untracked.PSet(
    Rethrow = FWCore.Framework.test.cmsExceptionsFatalOption_cff.Rethrow
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
         'file:/tmp/dmytro/step2_a_b.root'
    )
)

process.out = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string('/tmp/mc-muonid3.root'),
    outputCommands = cms.untracked.vstring("drop *",
        "keep *_genParticleCandidates_*_*",
	"keep recoTracks_*_*_*",
	"keep recoTrackExtras_*_*_*",
#	"keep recoMuons_*_*_*",
	"keep *_cscSegments_*_*",
	"keep *_dt4DSegments_*_*",
	"keep *_towerMaker_*_*",
	"keep *_*_*_TEST")
)

process.p = cms.Path(process.globalMuonLinks*process.muons)

process.muons.inputCollectionLabels = cms.VInputTag(cms.InputTag("generalTracks"),
						    cms.InputTag("globalMuonLinks"), 
						    cms.InputTag("standAloneMuons","UpdatedAtVtx"))
process.muons.inputCollectionTypes = cms.vstring('inner tracks', 
						 'links', 
						 'outer tracks')

process.muons.fillIsolation = False
process.muons.minPt = 0.
process.muons.minP = 0.

process.e = cms.EndPath(process.out)
