#!/usr/bin/env python

from TauAnalysis.Configuration.recoSampleDefinitionsZtoMuTau_7TeV_grid_cfi import recoSampleDefinitionsZtoMuTau_7TeV
from TauAnalysis.Configuration.harvestAnalysisResults_grid import harvestAnalysisResults
from TauAnalysis.Configuration.userRegistry import getAnalysisFilePath, getHarvestingFilePath, getJobId, getTmpFilePath
import TauAnalysis.Configuration.userRegistry as reg

channel = 'ZtoMuTau'
#reg.overrideJobId(channel, 'Run32')

analysisFilePath = getAnalysisFilePath(channel)
harvestingFilePath = getHarvestingFilePath(channel)
jobId = getJobId(channel)
tmpFilePath = getTmpFilePath(channel)

harvestAnalysisResults(channel = channel, samples = recoSampleDefinitionsZtoMuTau_7TeV,
                       inputFilePath = analysisFilePath, outputFilePath = harvestingFilePath, jobId = jobId,
                       tmpFilePath = tmpFilePath)
