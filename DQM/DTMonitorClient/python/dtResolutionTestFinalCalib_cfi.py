import FWCore.ParameterSet.Config as cms

resolutionTest = cms.EDAnalyzer("DTResolutionAnalysisTest",
                                 diagnosticPrescale = cms.untracked.int32(1),
                                 permittedMeanRange = cms.untracked.double(0.02),
                                 permittedSigmaRange = cms.untracked.double(0.12),
                                 # top folder for the histograms in DQMStore
                                 topHistoFolder = cms.untracked.string("DT/CalibrationSummary"),
                                 doCalibAnalysis = cms.untracked.bool(True)
                                )
