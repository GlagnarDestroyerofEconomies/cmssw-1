import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.Configuration.plotterProcessDefinitions_cfi import *
from TauAnalysis.Configuration.recoSampleDefinitionsZtoElecTau_cfi import *

#--------------------------------------------------------------------------------
# define for Z --> e + tau-jet analysis names of .root files containing histograms
#--------------------------------------------------------------------------------

processZtoElecTau_Ztautau = copy.deepcopy(process_Ztautau)
processZtoElecTau_Ztautau.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsZtoElecTau_Ztautau_part01.root',
    'plotsZtoElecTau_Ztautau_part02.root',
    'plotsZtoElecTau_Ztautau_part03.root',
    'plotsZtoElecTau_Ztautau_part04.root',
    'plotsZtoElecTau_Ztautau_part05.root',
    'plotsZtoElecTau_Ztautau_part06.root',
    'plotsZtoElecTau_Ztautau_part07.root',
    'plotsZtoElecTau_Ztautau_part08.root',
    'plotsZtoElecTau_Ztautau_part09.root',
    'plotsZtoElecTau_Ztautau_part10.root'
)
processZtoElecTau_Ztautau.config_dqmFileLoader.scaleFactor = cms.double(corrFactorZtoElecTau_Ztautau*intLumiZtoElecTau_Data/intLumiZtoElecTau_Ztautau)

processZtoElecTau_ZtautauSum = copy.deepcopy(process_Ztautau)
processZtoElecTau_ZtautauSum.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsZtoElecTau_ZtautauSum.root'
)
processZtoElecTau_ZtautauSum.config_dqmFileLoader.dqmDirectory_store = cms.string('harvested')
processZtoElecTau_ZtautauSum.config_dqmFileLoader.scaleFactor = cms.double(1.)

#--------------------------------------------------------------------------------

processZtoElecTau_Zee = copy.deepcopy(process_Zee)
processZtoElecTau_Zee.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsZtoElecTau_Zee_part01.root',
    'plotsZtoElecTau_Zee_part02.root',
    'plotsZtoElecTau_Zee_part03.root',
    'plotsZtoElecTau_Zee_part04.root',
    'plotsZtoElecTau_Zee_part05.root',
    'plotsZtoElecTau_Zee_part06.root',
    'plotsZtoElecTau_Zee_part07.root',
    'plotsZtoElecTau_Zee_part08.root',
    'plotsZtoElecTau_Zee_part09.root',
    'plotsZtoElecTau_Zee_part10.root',
    'plotsZtoElecTau_Zee_part11.root',
    'plotsZtoElecTau_Zee_part12.root',
    'plotsZtoElecTau_Zee_part13.root',
    'plotsZtoElecTau_Zee_part14.root',
    'plotsZtoElecTau_Zee_part15.root',
    'plotsZtoElecTau_Zee_part16.root',
    'plotsZtoElecTau_Zee_part17.root',
    'plotsZtoElecTau_Zee_part18.root',
    'plotsZtoElecTau_Zee_part19.root',
    'plotsZtoElecTau_Zee_part20.root',
    'plotsZtoElecTau_Zee_part21.root',
    'plotsZtoElecTau_Zee_part22.root',
    'plotsZtoElecTau_Zee_part23.root',
    'plotsZtoElecTau_Zee_part24.root'
)
processZtoElecTau_Zee.config_dqmFileLoader.scaleFactor = cms.double(corrFactorZtoElecTau_Zee*intLumiZtoElecTau_Data/intLumiZtoElecTau_Zee)

processZtoElecTau_ZeeSum = copy.deepcopy(process_Zee)
processZtoElecTau_ZeeSum.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsZtoElecTau_ZeeSum.root'
)
processZtoElecTau_ZeeSum.config_dqmFileLoader.dqmDirectory_store = cms.string('harvested')
processZtoElecTau_ZeeSum.config_dqmFileLoader.scaleFactor = cms.double(1.)

#--------------------------------------------------------------------------------

processZtoElecTau_gammaPlusJets_Pt15to20 = copy.deepcopy(process_gammaPlusJets)
processZtoElecTau_gammaPlusJets_Pt15to20.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsZtoElecTau_PhotonJets_Pt15to20.root'
)
processZtoElecTau_gammaPlusJets_Pt15to20.config_dqmFileLoader.dqmDirectory_store = cms.string('harvested/gammaPlusJets_Pt15to20')
processZtoElecTau_gammaPlusJets_Pt15to20.config_dqmFileLoader.scaleFactor = cms.double(corrFactorZtoElecTau_PhotonJets_Pt15to20*intLumiZtoElecTau_Data/intLumiZtoElecTau_PhotonJets_Pt15to20)

processZtoElecTau_gammaPlusJets_Pt20to25 = copy.deepcopy(process_gammaPlusJets)
processZtoElecTau_gammaPlusJets_Pt20to25.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsZtoElecTau_PhotonJets_Pt20to25.root'
)
processZtoElecTau_gammaPlusJets_Pt20to25.config_dqmFileLoader.dqmDirectory_store = cms.string('harvested/gammaPlusJets_Pt20to25')
processZtoElecTau_gammaPlusJets_Pt20to25.config_dqmFileLoader.scaleFactor = cms.double(corrFactorZtoElecTau_PhotonJets_Pt20to25*intLumiZtoElecTau_Data/intLumiZtoElecTau_PhotonJets_Pt20to25)

processZtoElecTau_gammaPlusJets_Pt25to30 = copy.deepcopy(process_gammaPlusJets)
processZtoElecTau_gammaPlusJets_Pt25to30.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsZtoElecTau_PhotonJets_Pt25to30.root'
)
processZtoElecTau_gammaPlusJets_Pt25to30.config_dqmFileLoader.dqmDirectory_store = cms.string('harvested/gammaPlusJets_Pt25to30')
processZtoElecTau_gammaPlusJets_Pt25to30.config_dqmFileLoader.scaleFactor = cms.double(corrFactorZtoElecTau_PhotonJets_Pt25to30*intLumiZtoElecTau_Data/intLumiZtoElecTau_PhotonJets_Pt25to30)

processZtoElecTau_gammaPlusJets_Pt30to35 = copy.deepcopy(process_gammaPlusJets)
processZtoElecTau_gammaPlusJets_Pt30to35.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsZtoElecTau_PhotonJets_Pt30to35.root'
)
processZtoElecTau_gammaPlusJets_Pt30to35.config_dqmFileLoader.dqmDirectory_store = cms.string('harvested/gammaPlusJets_Pt30to35')
processZtoElecTau_gammaPlusJets_Pt30to35.config_dqmFileLoader.scaleFactor = cms.double(corrFactorZtoElecTau_PhotonJets_Pt30to35*intLumiZtoElecTau_Data/intLumiZtoElecTau_PhotonJets_Pt30to35)

processZtoElecTau_gammaPlusJets_PtGt35 = copy.deepcopy(process_gammaPlusJets)
processZtoElecTau_gammaPlusJets_PtGt35.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsZtoElecTau_PhotonJets_PtGt35.root'
)
processZtoElecTau_gammaPlusJets_PtGt35.config_dqmFileLoader.dqmDirectory_store = cms.string('harvested/gammaPlusJets_PtGt35')
processZtoElecTau_gammaPlusJets_PtGt35.config_dqmFileLoader.scaleFactor = cms.double(corrFactorZtoElecTau_PhotonJets_PtGt35*intLumiZtoElecTau_Data/intLumiZtoElecTau_PhotonJets_PtGt35)

#--------------------------------------------------------------------------------

processZtoElecTau_QCD_BCtoE_Pt20to30 = copy.deepcopy(process_QCD_BCtoE_Pt20to30)
processZtoElecTau_QCD_BCtoE_Pt20to30.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsZtoElecTau_QCD_BCtoE_Pt20to30_part01.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt20to30_part02.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt20to30_part03.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt20to30_part04.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt20to30_part05.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt20to30_part06.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt20to30_part07.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt20to30_part08.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt20to30_part09.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt20to30_part10.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt20to30_part11.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt20to30_part12.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt20to30_part13.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt20to30_part14.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt20to30_part15.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt20to30_part16.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt20to30_part17.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt20to30_part18.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt20to30_part19.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt20to30_part20.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt20to30_part21.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt20to30_part22.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt20to30_part23.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt20to30_part24.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt20to30_part25.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt20to30_part26.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt20to30_part27.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt20to30_part28.root'
)
processZtoElecTau_QCD_BCtoE_Pt20to30.config_dqmFileLoader.scaleFactor = cms.double(corrFactorZtoElecTau_QCD_BCtoE_Pt20to30*intLumiZtoElecTau_Data/intLumiZtoElecTau_QCD_BCtoE_Pt20to30)

processZtoElecTau_QCD_BCtoE_Pt20to30Sum = copy.deepcopy(process_QCD_BCtoE_Pt20to30)
processZtoElecTau_QCD_BCtoE_Pt20to30Sum.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsZtoElecTau_QCD_BCtoE_Pt20to30Sum.root'
)
processZtoElecTau_QCD_BCtoE_Pt20to30Sum.config_dqmFileLoader.dqmDirectory_store = cms.string('harvested')
processZtoElecTau_QCD_BCtoE_Pt20to30Sum.config_dqmFileLoader.scaleFactor = cms.double(1.)

#--------------------------------------------------------------------------------

processZtoElecTau_QCD_BCtoE_Pt30to80 = copy.deepcopy(process_QCD_BCtoE_Pt30to80)
processZtoElecTau_QCD_BCtoE_Pt30to80.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsZtoElecTau_QCD_BCtoE_Pt30to80_part01.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt30to80_part02.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt30to80_part03.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt30to80_part04.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt30to80_part05.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt30to80_part06.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt30to80_part07.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt30to80_part08.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt30to80_part09.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt30to80_part10.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt30to80_part11.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt30to80_part12.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt30to80_part13.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt30to80_part14.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt30to80_part15.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt30to80_part16.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt30to80_part17.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt30to80_part18.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt30to80_part19.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt30to80_part20.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt30to80_part21.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt30to80_part22.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt30to80_part23.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt30to80_part24.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt30to80_part25.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt30to80_part26.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt30to80_part27.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt30to80_part28.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt30to80_part29.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt30to80_part30.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt30to80_part31.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt30to80_part32.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt30to80_part33.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt30to80_part34.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt30to80_part35.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt30to80_part36.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt30to80_part37.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt30to80_part38.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt30to80_part39.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt30to80_part40.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt30to80_part41.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt30to80_part42.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt30to80_part43.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt30to80_part44.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt30to80_part45.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt30to80_part46.root'
)
processZtoElecTau_QCD_BCtoE_Pt30to80.config_dqmFileLoader.scaleFactor = cms.double(corrFactorZtoElecTau_QCD_BCtoE_Pt30to80*intLumiZtoElecTau_Data/intLumiZtoElecTau_QCD_BCtoE_Pt30to80)

processZtoElecTau_QCD_BCtoE_Pt30to80Sum = copy.deepcopy(process_QCD_BCtoE_Pt30to80)
processZtoElecTau_QCD_BCtoE_Pt30to80Sum.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsZtoElecTau_QCD_BCtoE_Pt30to80Sum.root'
)
processZtoElecTau_QCD_BCtoE_Pt30to80Sum.config_dqmFileLoader.dqmDirectory_store = cms.string('harvested')
processZtoElecTau_QCD_BCtoE_Pt30to80Sum.config_dqmFileLoader.scaleFactor = cms.double(1.)

#--------------------------------------------------------------------------------

processZtoElecTau_QCD_BCtoE_Pt80to170 = copy.deepcopy(process_QCD_BCtoE_Pt80to170)
processZtoElecTau_QCD_BCtoE_Pt80to170.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsZtoElecTau_QCD_BCtoE_Pt80to170_part01.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt80to170_part02.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt80to170_part03.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt80to170_part04.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt80to170_part05.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt80to170_part06.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt80to170_part07.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt80to170_part08.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt80to170_part09.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt80to170_part10.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt80to170_part11.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt80to170_part12.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt80to170_part13.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt80to170_part14.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt80to170_part15.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt80to170_part16.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt80to170_part17.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt80to170_part18.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt80to170_part19.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt80to170_part20.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt80to170_part21.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt80to170_part22.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt80to170_part23.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt80to170_part24.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt80to170_part25.root',
    'plotsZtoElecTau_QCD_BCtoE_Pt80to170_part26.root'
)
processZtoElecTau_QCD_BCtoE_Pt80to170.config_dqmFileLoader.scaleFactor = cms.double(corrFactorZtoElecTau_QCD_BCtoE_Pt80to170*intLumiZtoElecTau_Data/intLumiZtoElecTau_QCD_BCtoE_Pt80to170)

processZtoElecTau_QCD_BCtoE_Pt80to170Sum = copy.deepcopy(process_QCD_BCtoE_Pt80to170)
processZtoElecTau_QCD_BCtoE_Pt80to170Sum.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsZtoElecTau_QCD_BCtoE_Pt80to170Sum.root'
)
processZtoElecTau_QCD_BCtoE_Pt80to170Sum.config_dqmFileLoader.dqmDirectory_store = cms.string('harvested')
processZtoElecTau_QCD_BCtoE_Pt80to170Sum.config_dqmFileLoader.scaleFactor = cms.double(1.)

#--------------------------------------------------------------------------------

processZtoElecTau_QCD_EMenriched_Pt20to30 = copy.deepcopy(process_QCD_EMenriched_Pt20to30)
processZtoElecTau_QCD_EMenriched_Pt20to30.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsZtoElecTau_QCD_EMenriched_Pt20to30_part01.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt20to30_part02.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt20to30_part03.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt20to30_part04.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt20to30_part05.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt20to30_part06.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt20to30_part07.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt20to30_part08.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt20to30_part09.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt20to30_part10.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt20to30_part11.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt20to30_part12.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt20to30_part13.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt20to30_part14.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt20to30_part15.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt20to30_part16.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt20to30_part17.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt20to30_part18.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt20to30_part19.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt20to30_part20.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt20to30_part21.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt20to30_part22.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt20to30_part23.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt20to30_part24.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt20to30_part25.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt20to30_part26.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt20to30_part27.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt20to30_part28.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt20to30_part29.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt20to30_part30.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt20to30_part31.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt20to30_part32.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt20to30_part33.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt20to30_part34.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt20to30_part35.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt20to30_part36.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt20to30_part37.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt20to30_part38.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt20to30_part39.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt20to30_part40.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt20to30_part41.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt20to30_part42.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt20to30_part43.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt20to30_part44.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt20to30_part45.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt20to30_part46.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt20to30_part47.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt20to30_part48.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt20to30_part49.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt20to30_part50.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt20to30_part51.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt20to30_part52.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt20to30_part53.root'
)
processZtoElecTau_QCD_EMenriched_Pt20to30.config_dqmFileLoader.scaleFactor = cms.double(corrFactorZtoElecTau_QCD_EMenriched_Pt20to30*intLumiZtoElecTau_Data/intLumiZtoElecTau_QCD_EMenriched_Pt20to30)

processZtoElecTau_QCD_EMenriched_Pt20to30Sum = copy.deepcopy(process_QCD_EMenriched_Pt20to30)
processZtoElecTau_QCD_EMenriched_Pt20to30Sum.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsZtoElecTau_QCD_EMenriched_Pt20to30Sum.root'
)
processZtoElecTau_QCD_EMenriched_Pt20to30Sum.config_dqmFileLoader.dqmDirectory_store = cms.string('harvested')
processZtoElecTau_QCD_EMenriched_Pt20to30Sum.config_dqmFileLoader.scaleFactor = cms.double(1.)

#--------------------------------------------------------------------------------


processZtoElecTau_QCD_EMenriched_Pt30to80_part01 = copy.deepcopy(process_QCD_EMenriched_Pt30to80)
processZtoElecTau_QCD_EMenriched_Pt30to80_part01.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part01.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part02.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part03.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part04.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part05.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part06.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part07.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part08.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part09.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part10.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part11.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part12.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part13.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part14.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part15.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part16.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part17.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part18.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part19.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part20.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part21.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part22.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part23.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part24.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part25.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part26.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part27.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part28.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part29.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part30.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part31.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part32.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part33.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part34.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part35.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part36.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part37.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part38.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part39.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part40.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part41.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part42.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part43.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part44.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part45.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part46.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part47.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part48.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part49.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part50.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part51.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part52.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part53.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part54.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part55.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part56.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part57.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part58.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part59.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part60.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part61.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part62.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part63.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part64.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part65.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part66.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part67.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part68.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part69.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part70.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part71.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part72.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part73.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part74.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part75.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part76.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part77.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part78.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part79.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part80.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part81.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part82.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part83.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part84.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part85.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part86.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part87.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part88.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part89.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part90.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part91.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part92.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part93.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part94.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part95.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part96.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part97.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part98.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part99.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part100.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part101.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part102.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part103.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part104.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part105.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part106.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part107.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part108.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part109.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part110.root'
)
processZtoElecTau_QCD_EMenriched_Pt30to80_part01.config_dqmFileLoader.scaleFactor = cms.double(corrFactorZtoElecTau_QCD_EMenriched_Pt30to80*intLumiZtoElecTau_Data/intLumiZtoElecTau_QCD_EMenriched_Pt30to80)

processZtoElecTau_QCD_EMenriched_Pt30to80_part02 = copy.deepcopy(process_QCD_EMenriched_Pt30to80)
processZtoElecTau_QCD_EMenriched_Pt30to80_part02.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part111.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part112.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part113.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part114.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part115.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part116.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part117.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part118.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part119.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part120.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part121.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part122.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part123.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part124.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part125.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part126.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part127.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part128.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part129.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part130.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part131.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part132.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part133.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part134.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part135.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part136.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part137.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part138.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part139.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part140.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part141.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part142.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part143.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part144.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part145.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part146.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part147.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part148.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part149.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part150.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part151.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part152.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part153.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part154.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part155.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part156.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part157.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part158.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part159.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part160.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part161.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part162.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part163.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part164.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part165.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part166.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part167.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part168.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part169.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part170.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part171.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part172.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part173.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part174.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part175.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part176.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part177.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part178.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part179.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part180.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part181.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part182.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part183.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part184.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part185.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part186.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part187.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part188.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part189.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part190.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part191.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part192.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part193.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part194.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part195.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part196.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part197.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part198.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part199.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part200.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part201.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part202.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part203.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part204.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part205.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part206.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part207.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part208.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part209.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part210.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part211.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part212.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part213.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part214.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part215.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part216.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part217.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part218.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part219.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part220.root'
)
processZtoElecTau_QCD_EMenriched_Pt30to80_part02.config_dqmFileLoader.scaleFactor = cms.double(corrFactorZtoElecTau_QCD_EMenriched_Pt30to80*intLumiZtoElecTau_Data/intLumiZtoElecTau_QCD_EMenriched_Pt30to80)

processZtoElecTau_QCD_EMenriched_Pt30to80Sum = copy.deepcopy(process_QCD_EMenriched_Pt30to80)
processZtoElecTau_QCD_EMenriched_Pt30to80Sum.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part01Sum.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part02Sum.root'
)
processZtoElecTau_QCD_EMenriched_Pt30to80Sum.config_dqmFileLoader.dqmDirectory_store = cms.string('harvested')
processZtoElecTau_QCD_EMenriched_Pt30to80Sum.config_dqmFileLoader.scaleFactor = cms.double(1.)

#--------------------------------------------------------------------------------


processZtoElecTau_QCD_EMenriched_Pt80to170 = copy.deepcopy(process_QCD_EMenriched_Pt80to170)
processZtoElecTau_QCD_EMenriched_Pt80to170.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part01.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part02.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part03.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part04.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part05.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part06.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part07.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part08.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part09.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part10.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part11.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part12.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part13.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part14.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part15.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part16.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part17.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part18.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part19.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part20.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part21.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part22.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part23.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part24.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part25.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part26.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part27.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part28.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part29.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part30.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part31.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part32.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part33.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part34.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part35.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part36.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part37.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part38.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part39.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part40.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part41.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part42.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part43.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part44.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part45.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part46.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part47.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part48.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part49.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part50.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part51.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part52.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part53.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part54.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part55.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part56.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part57.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part58.root',
    'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part59.root'
)
processZtoElecTau_QCD_EMenriched_Pt80to170.config_dqmFileLoader.scaleFactor = cms.double(corrFactorZtoElecTau_QCD_EMenriched_Pt80to170*intLumiZtoElecTau_Data/intLumiZtoElecTau_QCD_EMenriched_Pt80to170)

processZtoElecTau_QCD_EMenriched_Pt80to170Sum = copy.deepcopy(process_QCD_EMenriched_Pt80to170)
processZtoElecTau_QCD_EMenriched_Pt80to170Sum.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsZtoElecTau_QCD_EMenriched_Pt80to170Sum.root'
)
processZtoElecTau_QCD_EMenriched_Pt80to170Sum.config_dqmFileLoader.dqmDirectory_store = cms.string('harvested')
processZtoElecTau_QCD_EMenriched_Pt80to170Sum.config_dqmFileLoader.scaleFactor = cms.double(1.)

#--------------------------------------------------------------------------------

processZtoElecTau_WplusJets = copy.deepcopy(process_WplusJets)
processZtoElecTau_WplusJets.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsZtoElecTau_WplusJets_part01.root',
    'plotsZtoElecTau_WplusJets_part02.root',
    'plotsZtoElecTau_WplusJets_part03.root',
    'plotsZtoElecTau_WplusJets_part04.root',
    'plotsZtoElecTau_WplusJets_part05.root',
    'plotsZtoElecTau_WplusJets_part06.root',
    'plotsZtoElecTau_WplusJets_part07.root',
    'plotsZtoElecTau_WplusJets_part08.root',
    'plotsZtoElecTau_WplusJets_part09.root',
    'plotsZtoElecTau_WplusJets_part10.root',
    'plotsZtoElecTau_WplusJets_part11.root',
    'plotsZtoElecTau_WplusJets_part12.root',
    'plotsZtoElecTau_WplusJets_part13.root',
    'plotsZtoElecTau_WplusJets_part14.root',
    'plotsZtoElecTau_WplusJets_part15.root',
    'plotsZtoElecTau_WplusJets_part16.root',
    'plotsZtoElecTau_WplusJets_part17.root',
    'plotsZtoElecTau_WplusJets_part18.root',
    'plotsZtoElecTau_WplusJets_part19.root',
    'plotsZtoElecTau_WplusJets_part20.root',
    'plotsZtoElecTau_WplusJets_part21.root',
    'plotsZtoElecTau_WplusJets_part22.root',
    'plotsZtoElecTau_WplusJets_part23.root',
    'plotsZtoElecTau_WplusJets_part24.root',
    'plotsZtoElecTau_WplusJets_part25.root',
    'plotsZtoElecTau_WplusJets_part26.root',
    'plotsZtoElecTau_WplusJets_part27.root',
    'plotsZtoElecTau_WplusJets_part28.root',
    'plotsZtoElecTau_WplusJets_part29.root',
    'plotsZtoElecTau_WplusJets_part30.root',
    'plotsZtoElecTau_WplusJets_part31.root',
    'plotsZtoElecTau_WplusJets_part32.root',
    'plotsZtoElecTau_WplusJets_part33.root',
    'plotsZtoElecTau_WplusJets_part34.root'
)
processZtoElecTau_WplusJets.config_dqmFileLoader.scaleFactor = cms.double(corrFactorZtoElecTau_WplusJets*intLumiZtoElecTau_Data/intLumiZtoElecTau_WplusJets)

processZtoElecTau_WplusJetsSum = copy.deepcopy(process_WplusJets)
processZtoElecTau_WplusJetsSum.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsZtoElecTau_WplusJetsSum.root'
)
processZtoElecTau_WplusJetsSum.config_dqmFileLoader.dqmDirectory_store = cms.string('harvested')
processZtoElecTau_WplusJetsSum.config_dqmFileLoader.scaleFactor = cms.double(1.)

#--------------------------------------------------------------------------------

processZtoElecTau_ZeePlusJets = copy.deepcopy(process_ZeePlusJets)
processZtoElecTau_ZeePlusJets.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsZtoElecTau_ZeePlusJets_part01.root',
    'plotsZtoElecTau_ZeePlusJets_part02.root',
    'plotsZtoElecTau_ZeePlusJets_part03.root',
    'plotsZtoElecTau_ZeePlusJets_part04.root',
    'plotsZtoElecTau_ZeePlusJets_part05.root',
    'plotsZtoElecTau_ZeePlusJets_part06.root',
    'plotsZtoElecTau_ZeePlusJets_part07.root',
    'plotsZtoElecTau_ZeePlusJets_part08.root',
    'plotsZtoElecTau_ZeePlusJets_part09.root',
    'plotsZtoElecTau_ZeePlusJets_part10.root',
    'plotsZtoElecTau_ZeePlusJets_part11.root',
    'plotsZtoElecTau_ZeePlusJets_part12.root',
    'plotsZtoElecTau_ZeePlusJets_part13.root',
    'plotsZtoElecTau_ZeePlusJets_part14.root',
    'plotsZtoElecTau_ZeePlusJets_part15.root',
    'plotsZtoElecTau_ZeePlusJets_part16.root'
)
processZtoElecTau_ZeePlusJets.config_dqmFileLoader.scaleFactor = cms.double(corrFactorZtoElecTau_ZeePlusJets*intLumiZtoElecTau_Data/intLumiZtoElecTau_ZeePlusJets)

processZtoElecTau_ZeePlusJetsSum = copy.deepcopy(process_ZeePlusJets)
processZtoElecTau_ZeePlusJetsSum.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsZtoElecTau_ZeePlusJetsSum.root'
)
processZtoElecTau_ZeePlusJetsSum.config_dqmFileLoader.dqmDirectory_store = cms.string('harvested')
processZtoElecTau_ZeePlusJetsSum.config_dqmFileLoader.scaleFactor = cms.double(1.)

processZtoElecTau_ZtautauPlusJets = copy.deepcopy(process_ZtautauPlusJets)
processZtoElecTau_ZtautauPlusJets.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsZtoElecTau_ZtautauPlusJets_part01.root',
    'plotsZtoElecTau_ZtautauPlusJets_part02.root',
    'plotsZtoElecTau_ZtautauPlusJets_part03.root',
    'plotsZtoElecTau_ZtautauPlusJets_part04.root',
    'plotsZtoElecTau_ZtautauPlusJets_part05.root',
    'plotsZtoElecTau_ZtautauPlusJets_part06.root',
    'plotsZtoElecTau_ZtautauPlusJets_part07.root',
    'plotsZtoElecTau_ZtautauPlusJets_part08.root',
    'plotsZtoElecTau_ZtautauPlusJets_part09.root',
    'plotsZtoElecTau_ZtautauPlusJets_part10.root',
    'plotsZtoElecTau_ZtautauPlusJets_part11.root',
    'plotsZtoElecTau_ZtautauPlusJets_part12.root',
    'plotsZtoElecTau_ZtautauPlusJets_part13.root',
    'plotsZtoElecTau_ZtautauPlusJets_part14.root',
    'plotsZtoElecTau_ZtautauPlusJets_part15.root',
    'plotsZtoElecTau_ZtautauPlusJets_part16.root'
)
processZtoElecTau_ZtautauPlusJets.config_dqmFileLoader.scaleFactor = cms.double(corrFactorZtoElecTau_ZtautauPlusJets*intLumiZtoElecTau_Data/intLumiZtoElecTau_ZtautauPlusJets)

processZtoElecTau_ZtautauPlusJetsSum = copy.deepcopy(process_ZtautauPlusJets)
processZtoElecTau_ZtautauPlusJetsSum.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsZtoElecTau_ZtautauPlusJetsSum.root'
)
processZtoElecTau_ZtautauPlusJetsSum.config_dqmFileLoader.dqmDirectory_store = cms.string('harvested')
processZtoElecTau_ZtautauPlusJetsSum.config_dqmFileLoader.scaleFactor = cms.double(1.)

#--------------------------------------------------------------------------------

processZtoElecTau_TTplusJets = copy.deepcopy(process_TTplusJets)
processZtoElecTau_TTplusJets.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsZtoElecTau_TTplusJets_part01.root',
    'plotsZtoElecTau_TTplusJets_part02.root',
    'plotsZtoElecTau_TTplusJets_part03.root',
    'plotsZtoElecTau_TTplusJets_part04.root',
    'plotsZtoElecTau_TTplusJets_part05.root',
    'plotsZtoElecTau_TTplusJets_part06.root',
    'plotsZtoElecTau_TTplusJets_part07.root',
    'plotsZtoElecTau_TTplusJets_part08.root',
    'plotsZtoElecTau_TTplusJets_part09.root',
    'plotsZtoElecTau_TTplusJets_part10.root',
    'plotsZtoElecTau_TTplusJets_part11.root',
    'plotsZtoElecTau_TTplusJets_part12.root',
    'plotsZtoElecTau_TTplusJets_part13.root',
    'plotsZtoElecTau_TTplusJets_part14.root',
    'plotsZtoElecTau_TTplusJets_part13.root',
    'plotsZtoElecTau_TTplusJets_part13.root',
    'plotsZtoElecTau_TTplusJets_part13.root',
    'plotsZtoElecTau_TTplusJets_part13.root',
    'plotsZtoElecTau_TTplusJets_part13.root',
    'plotsZtoElecTau_TTplusJets_part13.root',
    'plotsZtoElecTau_TTplusJets_part12.root',
    'plotsZtoElecTau_TTplusJets_part12.root',
    'plotsZtoElecTau_TTplusJets_part12.root',
    'plotsZtoElecTau_TTplusJets_part12.root',
    'plotsZtoElecTau_TTplusJets_part12.root',
    'plotsZtoElecTau_TTplusJets_part12.root',
    'plotsZtoElecTau_TTplusJets_part11.root',
    'plotsZtoElecTau_TTplusJets_part11.root',
    'plotsZtoElecTau_TTplusJets_part11.root',
    'plotsZtoElecTau_TTplusJets_part11.root',
    'plotsZtoElecTau_TTplusJets_part11.root',
    'plotsZtoElecTau_TTplusJets_part11.root'
)
processZtoElecTau_TTplusJets.config_dqmFileLoader.scaleFactor = cms.double(corrFactorZtoElecTau_TTplusJets*intLumiZtoElecTau_Data/intLumiZtoElecTau_TTplusJets)

processZtoElecTau_TTplusJetsSum = copy.deepcopy(process_TTplusJets)
processZtoElecTau_TTplusJetsSum.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsZtoElecTau_TTplusJetsSum.root'
)
processZtoElecTau_TTplusJetsSum.config_dqmFileLoader.dqmDirectory_store = cms.string('harvested')
processZtoElecTau_TTplusJetsSum.config_dqmFileLoader.scaleFactor = cms.double(1.)
