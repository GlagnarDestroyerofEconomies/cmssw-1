import FWCore.ParameterSet.Config as cms

process = cms.Process("SiStrpDQMTier0Test")

process.MessageLogger = cms.Service("MessageLogger",
    debugModules = cms.untracked.vstring('siStripDigis', 
                                         'siStripClusters', 
                                         'siStripZeroSuppression', 
                                         'SiStripClusterizer'),
    cout = cms.untracked.PSet(threshold = cms.untracked.string('ERROR')),
    destinations = cms.untracked.vstring('cout')
)

#----------------------------
# Event Source
#-----------------------------
process.source = cms.Source("PoolSource",
     fileNames = cms.untracked.vstring(

#    '/store/data/Commissioning08/Calo/RAW/v1/000/067/647/00247884-48A3-DD11-A10C-001D09F24498.root'
    '/store/data/Commissioning08/Calo/RAW/v1/000/067/647/0029FCB8-85A3-DD11-9395-000423D94E1C.root'

     )
)                            
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(2000))

#----------------------------
# DQM Environment
#-----------------------------
process.load("DQMServices.Core.DQM_cfg")
#process.DQMStore.referenceFileName = '/home/dqmdevlocal/reference/sistrip_reference.root'

process.load("DQMServices.Components.DQMEnvironment_cfi")

#----------------------------
# DQM Playback Environment
#-----------------------------
process.load("DQM.Integration.test.environment_playback_cfi")
process.dqmEnv.subSystemFolder    = "SiStrip"

#-----------------------------
# Magnetic Field
#-----------------------------
process.load("Configuration.StandardSequences.MagneticField_0T_cff")
process.prefer("VolumeBasedMagneticFieldESProducer")

#-------------------------------------------------
# GEOMETRY
#-------------------------------------------------
process.load("Configuration.StandardSequences.Geometry_cff")

#--------------------------
# Calibration
#--------------------------
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.connect = "frontier://FrontierProd/CMS_COND_21X_GLOBALTAG"
process.GlobalTag.globaltag = "CRAFT_ALL_V3::All"
process.es_prefer_GlobalTag = cms.ESPrefer('PoolDBESSource','GlobalTag')

#-----------------------
#  Reconstruction Modules
#-----------------------
# Real data raw to digi
process.load("EventFilter.SiStripRawToDigi.SiStripDigis_cfi")
process.siStripDigis.ProductLabel = 'source'
process.load("EventFilter.SiPixelRawToDigi.SiPixelRawToDigi_cfi")
process.siPixelDigis.InputLabel = 'source'

# Local and Track Reconstruction
process.load("RecoLocalTracker.Configuration.RecoLocalTracker_Cosmics_cff")
process.load("RecoTracker.Configuration.RecoTrackerP5_cff")

# offline beam spot
process.load("RecoVertex.BeamSpotProducer.BeamSpot_cff")

#--------------------------
# Strip DQM Sources and DQ
#--------------------------
process.load("DQM.SiStripMonitorClient.SiStripDQMTier0GlobalRun_cff")

#----------------------- 
# output module
#----------------------
process.myOut = cms.OutputModule("PoolOutputModule",
                                  fileName = cms.untracked.string('sistrip_reco2.root'),
                                  outputCommands = cms.untracked.vstring('drop *', 'keep *_MEtoEDMConverter_*_*')
                                )
#--------------------------
# Scheduling
#--------------------------
process.RecoForDQM = cms.Sequence(process.siPixelDigis*process.siStripDigis*process.offlineBeamSpot*process.trackerlocalreco*process.ctftracksP5)
process.p = cms.Path(process.RecoForDQM*process.SiStripDQMTest)

process.outpath = cms.EndPath(process.myOut)
