# PYTHON configuration file.
# Description:  Example of applying default (L2+L3) jet corrections.
# Author: K. Kousouris
# Date:  02 - September - 2009
# Date:  22 - November - 2009: Kalanand Mishra: Modified for 3.3.X (re-Reco) corrections
# Date:  14 - January - 2011: Kalanand Mishra: Modified for 3.8.X 

import FWCore.ParameterSet.Config as cms

isMC = True
NJetsToKeep = 2


process = cms.Process("Ana")
process.load("FWCore.MessageService.MessageLogger_cfi")
#############   Set the number of events #############
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
)
#############   Define the source file ###############
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
'/store/mc/Fall10/QCD_Pt_80to120_TuneZ2_7TeV_pythia6/GEN-SIM-RECO/START38_V12-v1/0000/FEF4D100-4CCB-DF11-94CB-00E08178C12F.root'
)
)
process.source.inputCommands = cms.untracked.vstring("keep *","drop *_MEtoEDMConverter_*_*")

#############   Include the jet corrections ##########
process.load("JetMETCorrections.Configuration.DefaultJEC_cff")
from JetMETCorrections.Configuration.DefaultJEC_cff import *
ak5CaloL2Relative.useCondDB = False
ak5CaloL3Absolute.useCondDB = False
ak5CaloResidual.useCondDB = False
ak5PFL2Relative.useCondDB = False
ak5PFL3Absolute.useCondDB = False
ak5PFResidual.useCondDB = False
ak5JPTL2Relative.useCondDB = False
ak5JPTL3Absolute.useCondDB = False
ak5JPTResidual.useCondDB = False


process.ak5CaloJetsCor = ak5CaloJetsL2L3.clone()
process.ak5PFJetsCor = ak5PFJetsL2L3.clone()
process.ak5JPTJetsCor = ak5JPTJetsL2L3.clone()
if not isMC:
    process.ak5CaloJetsCor = ak5CaloJetsL2L3Residual.clone()
    process.ak5PFJetsCor = ak5PFJetsL2L3Residual.clone()
    process.ak5JPTJetsCor = ak5JPTJetsL2L3Residual.clone()

#############   Apply selection cuts ##
process.ak5CaloJetsSel = cms.EDFilter("CaloJetSelector",  
    src = cms.InputTag("ak5CaloJetsCor"),
    cut = cms.string('pt > 20.0 && eta<3.0 && eta>-3.0')
)
process.ak5PFJetsSel = cms.EDFilter("PFJetSelector",  
    src = cms.InputTag("ak5PFJetsCor"),
    cut = cms.string('pt > 20.0 && eta<3.0 && eta>-3.0')
)

process.ak5JPTJetsSel = cms.EDFilter("JPTJetSelector",  
    src = cms.InputTag("ak5JPTJetsCor"),
    cut = cms.string('pt > 20.0 && eta<3.0 && eta>-3.0')
)

 
#############   User analyzer (corrected calo jets) ##
process.correctedAK5Calo = cms.EDAnalyzer("CaloJetPlotsExample",
    JetAlgorithm  = cms.string("ak5CaloJetsSel"),
    HistoFileName = cms.string('CorJetHisto_AK5Calo.root'),
    NJets         = cms.int32(NJetsToKeep)
    )
#############   User analyzer (corrected pf jets) ##
process.correctedAK5PF = cms.EDAnalyzer("PFJetPlotsExample",
    JetAlgorithm  = cms.string("ak5PFJetsSel"),
    HistoFileName = cms.string('CorJetHisto_AK5PF.root'),
    NJets         = cms.int32(NJetsToKeep)
    )
#############   User analyzer (corrected jpt jets) #####
process.correctedAK5JPT = cms.EDAnalyzer("JPTJetPlotsExample",
    JetAlgorithm    = cms.string("ak5JPTJetsSel"),
    HistoFileName   = cms.string('CorJetHisto_AK5JPT.root'),
    NJets           = cms.int32(NJetsToKeep)
    )
#############   Path       ###########################
process.p = cms.Path( process.ak5CaloJetsCor +
                      process.ak5CaloJetsSel +
                      process.correctedAK5Calo +
                      process.ak5PFJetsCor +
                      process.ak5PFJetsSel +
                      process.correctedAK5PF  +
                      process.ak5JPTJetsCor +
                      process.ak5JPTJetsSel +
                      process.correctedAK5JPT
                      )
#############   Format MessageLogger #################
process.MessageLogger.cerr.FwkReport.reportEvery = 10

