#!/usr/bin/env python

from TauAnalysis.Configuration.submitToBatch import submitToBatch
from TauAnalysis.BgEstimationTools.makeReplacementsBgEstSkim import makeReplacementsBgEstSkim

# name of the directory (either on afs area or castor)
# to which all .root files produced by the cmsRun job will be copied
outputFilePath = "/castor/cern.ch/user/v/veelken/CMSSW_3_1_2/bgEstSkim/ZtoMuTau/10TeV/"

#--------------------------------------------------------------------------------
#
# Monte Carlo samples from Summer'09 production
# reprocessed with CMSSW_3_1_2, skimmed by Letizia and Manuel
#
# NOTE: The jobs get submitted to the '1nd' queue,
#       which allows for an execution time of the cmsRun jobs of up to 24 hours
#       (the queues are {'1nh' (1 hour), '1nd' (24 hours) and '1nw' (1 week execution time limit);
#        see https://twiki.cern.ch/twiki/bin/view/CMS/CMSUKCMSSWBatch for details about the CERN batch system)           
#
#--------------------------------------------------------------------------------

# Z --> tau tau jobs
for i in range(15):
    submitToBatch(configFile = "skimZtoMuTau_cfg.py", channel = "ZtoMuTau",
                  sample = "Ztautau_10TeV_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsBgEstSkim, replacements = "maxEvents = -1",
                  job = "bgEstSkim", queue = "1nd", outputFilePath = outputFilePath)

# Z --> mu mu jobs
for i in range(17):
    submitToBatch(configFile = "skimZtoMuTau_cfg.py", channel = "ZtoMuTau",
                  sample = "Zmumu_10TeV_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsBgEstSkim, replacements = "maxEvents = -1",
                  job = "bgEstSkim", queue = "1nd", outputFilePath = outputFilePath)

# pp --> mu X QCD jobs
for i in range(21):
    submitToBatch(configFile = "skimZtoMuTau_cfg.py", channel = "ZtoMuTau",
                  sample = "InclusivePPmuX_10TeV_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsBgEstSkim, replacements = "maxEvents = -1",
                  job = "bgEstSkim", queue = "1nd", outputFilePath = outputFilePath)

for i in range(68):
    submitToBatch(configFile = "skimZtoMuTau_cfg.py", channel = "ZtoMuTau",
                  sample = "PPmuXptGt20_10TeV_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsBgEstSkim, replacements = "maxEvents = -1",
                  job = "bgEstSkim", queue = "1nd", outputFilePath = outputFilePath)

# W + jets jobs
for i in range(26):
    submitToBatch(configFile = "skimZtoMuTau_cfg.py", channel = "ZtoMuTau",
                  sample = "WplusJets_10TeV_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsBgEstSkim, replacements = "maxEvents = -1",
                  job = "bgEstSkim", queue = "1nd", outputFilePath = outputFilePath)

# ttbar + jets jobs
for i in range(17):
    submitToBatch(configFile = "skimZtoMuTau_cfg.py", channel = "ZtoMuTau",
                  sample = "TTplusJets_10TeV_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsBgEstSkim, replacements = "maxEvents = -1",
                  job = "bgEstSkim", queue = "1nd", outputFilePath = outputFilePath)
