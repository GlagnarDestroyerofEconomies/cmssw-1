import FWCore.ParameterSet.Config as cms

process = cms.Process("RAWFilterTest")

process.load("DPGAnalysis.SiStripTools.filtertest.config_69800_reco_cff")
#------------------------------------------------------------------
# filters
#------------------------------------------------------------------
process.load("DPGAnalysis.SiStripTools.filtertest.alcareco_filter_tests_cff")
#------------------------------------------------------------------


process.TFileService = cms.Service('TFileService',
                                   fileName = cms.string('alca_filter_test_69800_reco.root')
                                   )

process.p0 = cms.Path(process.sinit + process.eventtimedistribution)
process.p1 = cms.Path(process.sinit + process.alcas1)
process.p2 = cms.Path(process.sinit + process.alcas2)
process.p3 = cms.Path(process.sinit + process.alcas3)
process.p4 = cms.Path(process.sinit + process.alcas4)

