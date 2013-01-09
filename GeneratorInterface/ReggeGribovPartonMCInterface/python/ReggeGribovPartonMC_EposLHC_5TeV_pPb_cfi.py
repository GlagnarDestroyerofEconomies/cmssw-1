import FWCore.ParameterSet.Config as cms

source = cms.Source("EmptySource")

from GeneratorInterface.ReggeGribovPartonMCInterface.ReggeGribovPartonMC_AdvancedParameters_cfi import *
generator = cms.EDFilter("ReggeGribovPartonMCGeneratorFilter",
                    ReggeGribovInterfaceAdvancedParameters,
                    beammomentum = cms.double(1577),
                    targetmomentum = cms.double(-4000),
                    beamid = cms.int32(208),
                    targetid = cms.int32(1),
                    model = cms.int32(0),
                    )


configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.2 $'),
    name = cms.untracked.string('$Source: /local/reps/CMSSW/CMSSW/GeneratorInterface/ReggeGribovPartonMCInterface/python/ReggeGribovPartonMC_EposLHC_5TeV_pPb_cfi.py,v $'),
    annotation = cms.untracked.string('ReggeGribovMC generator')
    )





