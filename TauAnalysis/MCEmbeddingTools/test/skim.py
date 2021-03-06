# Auto generated configuration file
# using: 
# Revision: 1.381.2.2 
# Source: /local/reps/CMSSW/CMSSW/Configuration/PyReleaseValidation/python/ConfigBuilder.py,v 
# with command line options: TauAnalysis/MCEmbeddingTools/python/ZmumuStandaloneSelection_cff -s NONE --no_exec --conditions=FrontierConditions_GlobalTag,START53_V7A::All --fileout=skimmed.root --eventcontent=RECOSIM --python_filename=skim.py --customise=TauAnalysis/MCEmbeddingTools/ZmumuStandaloneSelectionAll -n 10
import FWCore.ParameterSet.Config as cms

process = cms.Process('NONE')

# import of standard configurations
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10)
)

# Input source
process.source = cms.Source("PoolSource",
    secondaryFileNames = cms.untracked.vstring(),
#    fileNames = cms.untracked.vstring('file:/scratch/hh/lustre/cms/user/aburgmei/testFiles/_DoubleMu_Run2012A-22Jan2013-v1_RECO/4A43BA46-AC81-E211-8184-002590593920.root')
    fileNames = cms.untracked.vstring('file:/scratch/hh/dust/naf/cms/user/aburgmei/4A43BA46-AC81-E211-8184-002590593920.root')
)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.1 $'),
    annotation = cms.untracked.string('TauAnalysis/MCEmbeddingTools/python/ZmumuStandaloneSelection_cff nevts:10'),
    name = cms.untracked.string('PyReleaseValidation')
)

# Output definition

process.RECOSIMoutput = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = process.RECOSIMEventContent.outputCommands,
    fileName = cms.untracked.string('skimmed.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('')
    )
)

# Additional output definition

# Other statements
process.GlobalTag.globaltag = 'FT_R_53_V18::All'

# Path and EndPath definitions
process.RECOSIMoutput_step = cms.EndPath(process.RECOSIMoutput)

# Schedule definition
process.schedule = cms.Schedule(process.RECOSIMoutput_step)

# customisation of the process.

# Set options
from TauAnalysis.MCEmbeddingTools.setDefaults import setDefaults
setDefaults(process)
process.customization_options.isMC = cms.bool(False)
process.customization_options.ZmumuCollection = cms.InputTag('goldenZmumuCandidatesGe0IsoMuons')

# Automatic addition of the customisation function from TauAnalysis.MCEmbeddingTools.ZmumuStandaloneSelectionAll
from TauAnalysis.MCEmbeddingTools.ZmumuStandaloneSelectionAll import customise 

#call to customisation function customise imported from TauAnalysis.MCEmbeddingTools.ZmumuStandaloneSelectionAll
process = customise(process)

# End of customisation functions
