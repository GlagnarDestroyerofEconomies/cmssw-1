import FWCore.ParameterSet.Config as cms
import copy

#--------------------------------------------------------------------------------
#
# Apply "template" method for data-driven background estimation
# to Z --> mu + tau-jet channel
#
# Author: Christian Veelken, UC Davis
#
#--------------------------------------------------------------------------------

from TauAnalysis.DQMTools.plotterStyleDefinitions_cfi import *
from TauAnalysis.BgEstimationTools.templateHistDefinitions_cfi import drawJobTemplateHist
from TauAnalysis.BgEstimationTools.tools.drawTemplateHistConfigurator import drawTemplateHistConfigurator

processName = 'fitBgEstTemplateZtoMuTau'
process = cms.Process(processName)

process.DQMStore = cms.Service("DQMStore")

process.maxEvents = cms.untracked.PSet(            
    input = cms.untracked.int32(0)         
)

process.source = cms.Source("EmptySource")

#--------------------------------------------------------------------------------
# produce template histograms of visible muon + tau-jet mass distribution
#
# NOTE:
#  1.) template histogram for Ztautau signal process
#       taken from Z --> mu+ mu- events selected in (pseudo)data,
#       using MCEmbeddingTools
#  2.) distribution observed in (pseudo)data taken from plotsZtoMuTau_all.root file
#      produced by "official" analysis workflow documented at
#       https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideTauAnalysisZtoMuTau
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# define directories in which histograms are stored in DQMStore
#--------------------------------------------------------------------------------

dqmDirectory_Ztautau_finalEvtSel = 'harvested/Ztautau/zMuTauAnalyzer/afterEvtSelDiMuPairZmumuHypothesisVeto'
##dqmDirectory_Ztautau_template = 'template/harvested/Ztautau_from_selZmumu/pure/zMuTauAnalyzer/afterEvtSelDiMuPairZmumuHypothesisVeto'
##dqmDirectory_Ztautau_template = 'template/harvested/Ztautau_from_selZmumu/zMuTauAnalyzer/afterEvtSelDiTauCandidateForMuTauPzetaDiff'
dqmDirectory_Ztautau_template = 'template/harvested/Ztautau/BgEstTemplateAnalyzer_Ztautau/afterEvtSelDiMuPairZmumuHypothesisVeto'
dqmDirectory_Ztautau_templateSysTauEnUp = 'template/harvested/Ztautau/BgEstTemplateAnalyzer_Ztautau/afterEvtSelDiMuPairZmumuHypothesisVeto/sysUncertaintyHistManagerResults/sysTauJetEnUp'
dqmDirectory_Ztautau_templateSysTauEnDown = 'template/harvested/Ztautau/BgEstTemplateAnalyzer_Ztautau/afterEvtSelDiMuPairZmumuHypothesisVeto/sysUncertaintyHistManagerResults/sysTauJetEnDown'

dqmDirectory_Ztautau_systematics = processName + '/Ztautau/systematics'

dqmDirectory_Zmumu_finalEvtSel = 'harvested/Zmumu/zMuTauAnalyzer/afterEvtSelDiMuPairZmumuHypothesisVeto'
dqmDirectory_ZmumuJetMisId_correctedTemplate_pure = 'template/harvested/Zmumu/BgEstTemplateAnalyzer_ZmumuJetMisIdEnriched_reweighted/afterDiMuonPairInvMassBgEstZmumuJetMisIdEnriched'
dqmDirectory_ZmumuJetMisId_correctedTemplate_data = 'template/harvested/smSum/BgEstTemplateAnalyzer_ZmumuJetMisIdEnriched_reweighted/afterDiMuonPairInvMassBgEstZmumuJetMisIdEnriched'
dqmDirectory_ZmumuJetMisId_uncorrectedTemplate_pure = 'template/harvested/Zmumu/BgEstTemplateAnalyzer_ZmumuJetMisIdEnriched/afterDiMuonPairInvMassBgEstZmumuJetMisIdEnriched'
dqmDirectory_ZmumuJetMisId_uncorrectedTemplate_data = 'template/harvested/smSum/BgEstTemplateAnalyzer_ZmumuJetMisIdEnriched/afterDiMuonPairInvMassBgEstZmumuJetMisIdEnriched'
dqmDirectory_ZmumuMuonMisId_template_pure = 'template/harvested/Zmumu/BgEstTemplateAnalyzer_ZmumuMuonMisIdEnriched/afterDiMuonPairBgEstZmumuMuonMisIdEnriched'
dqmDirectory_ZmumuMuonMisId_template_data = 'template/harvested/smSum/BgEstTemplateAnalyzer_ZmumuMuonMisIdEnriched/afterDiMuonPairBgEstZmumuMuonMisIdEnriched'
dqmDirectory_ZmumuJetMisId_systematics = processName + '/Zmumu/systematicsJetMisId'
dqmDirectory_ZmumuMuonMisId_systematics = processName + '/Zmumu/systematicsMuonMisId'

dqmDirectory_WplusJets_finalEvtSel = 'harvested/WplusJets/zMuTauAnalyzer/afterEvtSelDiMuPairZmumuHypothesisVeto'
dqmDirectory_WplusJets_correctedTemplate_pure = 'template/harvested/WplusJets/BgEstTemplateAnalyzer_WplusJetsEnriched_reweighted/afterDiMuonVetoBgEstWplusJetsEnriched'
dqmDirectory_WplusJets_correctedTemplate_data = 'template/harvested/smSum/BgEstTemplateAnalyzer_WplusJetsEnriched_reweighted/afterDiMuonVetoBgEstWplusJetsEnriched'
dqmDirectory_WplusJets_uncorrectedTemplate_pure = 'template/harvested/WplusJets/BgEstTemplateAnalyzer_WplusJetsEnriched/afterDiMuonVetoBgEstWplusJetsEnriched'
dqmDirectory_WplusJets_uncorrectedTemplate_data = 'template/harvested/smSum/BgEstTemplateAnalyzer_WplusJetsEnriched/afterDiMuonVetoBgEstWplusJetsEnriched'
dqmDirectory_WplusJets_systematics = processName + '/WplusJets/systematics'

dqmDirectory_TTplusJets_finalEvtSel = 'harvested/TTplusJets/zMuTauAnalyzer/afterEvtSelDiMuPairZmumuHypothesisVeto'
dqmDirectory_TTplusJets_template_pure = 'template/harvested/TTplusJets/BgEstTemplateAnalyzer_TTplusJetsEnriched/afterJetEt60BgEstTTplusJetsEnriched'
dqmDirectory_TTplusJets_template_data = 'template/harvested/smSum/BgEstTemplateAnalyzer_TTplusJetsEnriched/afterJetEt60BgEstTTplusJetsEnriched'
dqmDirectory_TTplusJets_systematics = processName + '/TTplusJets/systematics'

dqmDirectory_QCD_finalEvtSel = 'harvested/qcdSum/zMuTauAnalyzer/afterEvtSelDiMuPairZmumuHypothesisVeto'
dqmDirectory_QCD_template_pure = 'template/harvested/qcdSum/BgEstTemplateAnalyzer_QCDenriched/afterDiMuonVetoBgEstQCDenriched'
dqmDirectory_QCD_template_data = 'template/harvested/smSum/BgEstTemplateAnalyzer_QCDenriched/afterDiMuonVetoBgEstQCDenriched'
dqmDirectory_QCD_systematics = processName + '/QCD/systematics'

dqmDirectory_data_finalEvtSel = 'harvested/smSum/zMuTauAnalyzer/afterEvtSelDiMuPairZmumuHypothesisVeto'

#--------------------------------------------------------------------------------
# define names of histograms used in fit
#--------------------------------------------------------------------------------

meName_diTauMvis12 = "DiTauCandidateQuantities/VisMass"
meName_diTauMvis12_norm = "DiTauCandidateQuantities/VisMassShape"

#--------------------------------------------------------------------------------
# load template histogram of visible muon + tau-jet mass distribution in Z --> tau+ tau- signal events,
# produced from by MCEmbeddingTools from Z --> mu+ mu- events selected in (pseudo)data
#--------------------------------------------------------------------------------

process.loadTemplateHistZtoMuTau_Ztautau = cms.EDAnalyzer("DQMFileLoader",
    Ztautau = cms.PSet(
        inputFileNames = cms.vstring(
            ##'rfio:/castor/cern.ch/user/v/veelken/CMSSW_3_6_x/bgEstPlots/ZtoMuTau_bgEstTemplates/7TeV/ZtoMuTau_from_selZmumu.root'
            'rfio:/castor/cern.ch/user/v/veelken/bgEstTemplates/ZtoMuTau_from_selZmumuII.root'
        ),
        scaleFactor = cms.double(1.),
        ##dqmDirectory_store = cms.string('template/Ztautau_from_selZmumu/pure')
        dqmDirectory_store = cms.string('template/harvested')
    )
)

#--------------------------------------------------------------------------------
# load template histogram of visible muon + tau-jet mass distribution
# for different types of background events
#--------------------------------------------------------------------------------

process.loadTemplateHistZtoMuTau = cms.EDAnalyzer("DQMFileLoader",
    Ztautau = cms.PSet(
        inputFileNames = cms.vstring(
            'rfio:/castor/cern.ch/user/v/veelken/CMSSW_3_6_x/bgEstPlots/ZtoMuTau_bgEstTemplates/7TeV/bgEstTemplatesZtoMuTau.root'
        ),
        scaleFactor = cms.double(1.),
        dqmDirectory_store = cms.string('template')
    )
)

#--------------------------------------------------------------------------------
# normalize to unit area distribution of visible muon + tau-jet mass for Z --> tau+ tau- signal
# and different types of background events
#--------------------------------------------------------------------------------

process.normalizeTemplateHistZtoMuTau = cms.EDAnalyzer("DQMHistNormalizer",
    config = cms.VPSet(
        cms.PSet(
            meNameInput = cms.string(dqmDirectory_Ztautau_template + '/' + meName_diTauMvis12),
            meNameOutput = cms.string(dqmDirectory_Ztautau_template + '/' + meName_diTauMvis12_norm)
        ),
        cms.PSet(
            meNameInput = cms.string(dqmDirectory_ZmumuJetMisId_correctedTemplate_pure + '/' + meName_diTauMvis12),
            meNameOutput = cms.string(dqmDirectory_ZmumuJetMisId_correctedTemplate_pure + '/' + meName_diTauMvis12_norm)
        ),
        cms.PSet(
            meNameInput = cms.string(dqmDirectory_ZmumuJetMisId_correctedTemplate_data + '/' + meName_diTauMvis12),
            meNameOutput = cms.string(dqmDirectory_ZmumuJetMisId_correctedTemplate_data + '/' + meName_diTauMvis12_norm)
        ),
        cms.PSet(
            meNameInput = cms.string(dqmDirectory_ZmumuJetMisId_uncorrectedTemplate_pure + '/' + meName_diTauMvis12),
            meNameOutput = cms.string(dqmDirectory_ZmumuJetMisId_uncorrectedTemplate_pure + '/' + meName_diTauMvis12_norm)
        ),
        cms.PSet(
            meNameInput = cms.string(dqmDirectory_ZmumuJetMisId_uncorrectedTemplate_data + '/' + meName_diTauMvis12),
            meNameOutput = cms.string(dqmDirectory_ZmumuJetMisId_uncorrectedTemplate_data + '/' + meName_diTauMvis12_norm)
        ),
        cms.PSet(
            meNameInput = cms.string(dqmDirectory_ZmumuMuonMisId_template_pure + '/' + meName_diTauMvis12),
            meNameOutput = cms.string(dqmDirectory_ZmumuMuonMisId_template_pure + '/' + meName_diTauMvis12_norm)
        ),
        cms.PSet(
            meNameInput = cms.string(dqmDirectory_ZmumuMuonMisId_template_data + '/' + meName_diTauMvis12),
            meNameOutput = cms.string(dqmDirectory_ZmumuMuonMisId_template_data + '/' + meName_diTauMvis12_norm)
        ),
        cms.PSet(
            meNameInput = cms.string(dqmDirectory_WplusJets_correctedTemplate_pure + '/' + meName_diTauMvis12),
            meNameOutput = cms.string(dqmDirectory_WplusJets_correctedTemplate_pure + '/' + meName_diTauMvis12_norm)
        ),
        cms.PSet(
            meNameInput = cms.string(dqmDirectory_WplusJets_correctedTemplate_data + '/' + meName_diTauMvis12),
            meNameOutput = cms.string(dqmDirectory_WplusJets_correctedTemplate_data + '/' + meName_diTauMvis12_norm)
        ),
        cms.PSet(
            meNameInput = cms.string(dqmDirectory_WplusJets_uncorrectedTemplate_pure + '/' + meName_diTauMvis12),
            meNameOutput = cms.string(dqmDirectory_WplusJets_uncorrectedTemplate_pure + '/' + meName_diTauMvis12_norm)
        ),
        cms.PSet(
            meNameInput = cms.string(dqmDirectory_WplusJets_uncorrectedTemplate_data + '/' + meName_diTauMvis12),
            meNameOutput = cms.string(dqmDirectory_WplusJets_uncorrectedTemplate_data + '/' + meName_diTauMvis12_norm)
        ),
        cms.PSet(
            meNameInput = cms.string(dqmDirectory_TTplusJets_template_pure + '/' + meName_diTauMvis12),
            meNameOutput = cms.string(dqmDirectory_TTplusJets_template_pure + '/' + meName_diTauMvis12_norm)
        ),
        cms.PSet(
            meNameInput = cms.string(dqmDirectory_TTplusJets_template_data + '/' + meName_diTauMvis12),
            meNameOutput = cms.string(dqmDirectory_TTplusJets_template_data + '/' + meName_diTauMvis12_norm)
        ),
        cms.PSet(
            meNameInput = cms.string(dqmDirectory_QCD_template_pure + '/' + meName_diTauMvis12),
            meNameOutput = cms.string(dqmDirectory_QCD_template_pure + '/' + meName_diTauMvis12_norm)
        ),
        cms.PSet(
            meNameInput = cms.string(dqmDirectory_QCD_template_data + '/' + meName_diTauMvis12),
            meNameOutput = cms.string(dqmDirectory_QCD_template_data + '/' + meName_diTauMvis12_norm)
        )
    ),   
    norm = cms.double(1.)
)

#--------------------------------------------------------------------------------
# load distribution of visible muon + tau-jet mass
# expected for different signal/background processes and observed in (pseudo)data
# in events passing final analysis selection criteria
#--------------------------------------------------------------------------------

process.loadAnalysisHistZtoMuTau = cms.EDAnalyzer("DQMFileLoader",
    data = cms.PSet(
        inputFileNames = cms.vstring(
            'rfio:/castor/cern.ch/user/v/veelken/CMSSW_3_6_x/plots/ZtoMuTau/7TeV/plotsZtoMuTau_all.root'
        ),
        scaleFactor = cms.double(1.),
        dqmDirectory_store = cms.string('')
    )
)

#--------------------------------------------------------------------------------
# normalize to unit area distribution of visible muon + tau-jet mass
# in simulated signal/background events passing final analysis selection criteria
#--------------------------------------------------------------------------------

process.normalizeAnalysisHistZtoMuTau = cms.EDAnalyzer("DQMHistNormalizer",
    config = cms.VPSet(
        cms.PSet(
             meNameInput = cms.string(dqmDirectory_Ztautau_finalEvtSel + '/' + meName_diTauMvis12),
             meNameOutput = cms.string(dqmDirectory_Ztautau_finalEvtSel + '/' + meName_diTauMvis12_norm)
        ),
        cms.PSet(
             meNameInput = cms.string(dqmDirectory_Zmumu_finalEvtSel + '/' + meName_diTauMvis12),
             meNameOutput = cms.string(dqmDirectory_Zmumu_finalEvtSel + '/' + meName_diTauMvis12_norm)
        ),
        cms.PSet(
             meNameInput = cms.string(dqmDirectory_WplusJets_finalEvtSel + '/' + meName_diTauMvis12),
             meNameOutput = cms.string(dqmDirectory_WplusJets_finalEvtSel + '/' + meName_diTauMvis12_norm)
        ),
        cms.PSet(
             meNameInput = cms.string(dqmDirectory_TTplusJets_finalEvtSel + '/' + meName_diTauMvis12),
             meNameOutput = cms.string(dqmDirectory_TTplusJets_finalEvtSel + '/' + meName_diTauMvis12_norm)
        ),
        cms.PSet(
             meNameInput = cms.string(dqmDirectory_QCD_finalEvtSel + '/' + meName_diTauMvis12),
             meNameOutput = cms.string(dqmDirectory_QCD_finalEvtSel + '/' + meName_diTauMvis12_norm)
        )
    ),
    norm = cms.double(1.)
)

#--------------------------------------------------------------------------------
# plot template histograms of "pure" Monte Carlo processes
# compared to the shapes determined by background enriched regions in (pseudo)Data
#--------------------------------------------------------------------------------

drawTemplateHistConfiguratorZtoMuTau = drawTemplateHistConfigurator(
    template = drawJobTemplateHist
)

#--------------------------------------------------------------------------------
# define draw jobs for Z --> tau+ tau- signal
#
# NOTE: backgrounds contributing to Z --> mu+ mu- sample from which
#       template histogram for Ztautau signal process is determined using MCEmbeddingTools
#       not included yet, so use "pure" sample as approximation for sample determined in "deta"
#       for the time being...
#--------------------------------------------------------------------------------

drawTemplateHistConfiguratorZtoMuTau.add(
    meNames = [
        dqmDirectory_Ztautau_template + '/' + meName_diTauMvis12_norm,
        dqmDirectory_Ztautau_template + '/' + meName_diTauMvis12_norm,
        dqmDirectory_Ztautau_finalEvtSel + '/' + meName_diTauMvis12_norm
    ],
    name = "Ztautau_diTauMvis12",
    title = "M_{vis}^{#mu + #tau-jet} in Z #rightarrow #tau^{+} #tau^{-} Signal" 
)

#--------------------------------------------------------------------------------
# define draw jobs for Z --> mu+ mu- background
#--------------------------------------------------------------------------------

drawTemplateHistConfiguratorZtoMuTau.add(
    meNames = [
        dqmDirectory_ZmumuJetMisId_correctedTemplate_data + '/' + meName_diTauMvis12_norm,
        dqmDirectory_ZmumuJetMisId_correctedTemplate_pure + '/' + meName_diTauMvis12_norm,
        dqmDirectory_Zmumu_finalEvtSel + '/' + meName_diTauMvis12_norm
    ],
    name = "ZmumuJetMisId_corrected_diTauMvis12",
    title = "M_{vis}^{#mu + #tau-jet} in Z #rightarrow #mu^{+} #mu^{-} Background (Jet misid.)"
)

drawTemplateHistConfiguratorZtoMuTau.add(
    meNames = [
        dqmDirectory_ZmumuJetMisId_uncorrectedTemplate_data + '/' + meName_diTauMvis12_norm,
        dqmDirectory_ZmumuJetMisId_uncorrectedTemplate_pure + '/' + meName_diTauMvis12_norm,
        dqmDirectory_Zmumu_finalEvtSel + '/' + meName_diTauMvis12_norm
    ],
    name = "ZmumuJetMisId_uncorrected_diTauMvis12",
    title = "M_{vis}^{#mu + #tau-jet} in Z #rightarrow #mu^{+} #mu^{-} Background (Jet misid.)"
)

drawTemplateHistConfiguratorZtoMuTau.add(
    meNames = [
        dqmDirectory_ZmumuMuonMisId_template_data + '/' + meName_diTauMvis12_norm,
        dqmDirectory_ZmumuMuonMisId_template_pure + '/' + meName_diTauMvis12_norm,
        dqmDirectory_Zmumu_finalEvtSel + '/' + meName_diTauMvis12_norm
    ],
    name = "ZmumuMuonMisId_diTauMvis12",
    title = "M_{vis}^{#mu + #tau-jet} in Z #rightarrow #mu^{+} #mu^{-} Background (Muon misid.)"
)

#--------------------------------------------------------------------------------
# define draw jobs for W + jets background
#--------------------------------------------------------------------------------

drawTemplateHistConfiguratorZtoMuTau.add(
    meNames = [
        dqmDirectory_WplusJets_correctedTemplate_data + '/' + meName_diTauMvis12_norm,
        dqmDirectory_WplusJets_correctedTemplate_pure + '/' + meName_diTauMvis12_norm,
        dqmDirectory_WplusJets_finalEvtSel + '/' + meName_diTauMvis12_norm
    ],
    name = "WplusJets_corrected_diTauMvis12",
    title = "M_{vis}^{#mu + #tau-jet} in W + jets Background"
)

drawTemplateHistConfiguratorZtoMuTau.add(
    meNames = [
        dqmDirectory_WplusJets_uncorrectedTemplate_data + '/' + meName_diTauMvis12_norm,
        dqmDirectory_WplusJets_uncorrectedTemplate_pure + '/' + meName_diTauMvis12_norm,
        dqmDirectory_WplusJets_finalEvtSel + '/' + meName_diTauMvis12_norm
    ],
    name = "WplusJets_uncorrected_diTauMvis12",
    title = "M_{vis}^{#mu + #tau-jet} in W + jets Background"
)

#--------------------------------------------------------------------------------
# define draw jobs for TTbar + jets background
#--------------------------------------------------------------------------------

drawTemplateHistConfiguratorZtoMuTau.add(
    meNames = [
        dqmDirectory_TTplusJets_template_data + '/' + meName_diTauMvis12_norm,
        dqmDirectory_TTplusJets_template_pure + '/' + meName_diTauMvis12_norm,
        dqmDirectory_TTplusJets_finalEvtSel + '/' + meName_diTauMvis12_norm
    ],
    name = "TTplusJets_diTauMvis12",
    title = "M_{vis}^{#mu + #tau-jet} in t#bar{t} + jets Background"
)

#--------------------------------------------------------------------------------
# define draw jobs for QCD background
#--------------------------------------------------------------------------------

drawTemplateHistConfiguratorZtoMuTau.add(
    meNames = [
        dqmDirectory_QCD_template_data + '/' + meName_diTauMvis12_norm,
        dqmDirectory_QCD_template_pure + '/' + meName_diTauMvis12_norm,
        dqmDirectory_QCD_finalEvtSel + '/' + meName_diTauMvis12_norm
    ],
    name = "QCD_diTauMvis12",
    title = "M_{vis}^{#mu + #tau-jet} in QCD Background"
)

process.plotTemplateHistZtoMuTau = cms.EDAnalyzer("DQMHistPlotter",
    processes = cms.PSet(
        bgEstData = cms.PSet(
            dqmDirectory = cms.string(''),
            legendEntry = drawJobTemplateHist.plots[0].legendEntry,
            type = cms.string('smMC')
        ),
        bgEstPure = cms.PSet(
            dqmDirectory = cms.string(''),
            legendEntry = drawJobTemplateHist.plots[1].legendEntry,
            type = cms.string('smMC')
        ),
        finalEvtSel = cms.PSet(
            dqmDirectory = cms.string(''),
            legendEntry = drawJobTemplateHist.plots[2].legendEntry,
            type = cms.string('smMC')
        )
    ),

    xAxes = cms.PSet(
        M = copy.deepcopy(xAxis_mass)
    ),

    yAxes = cms.PSet(                         
        numEntries_linear = copy.deepcopy(yAxis_numEntries_linear),
        numEntries_log = copy.deepcopy(yAxis_numEntries_log)
    ),

    legends = cms.PSet(
        regular = cms.PSet(
            posX = cms.double(0.45),            
            posY = cms.double(0.69),             
            sizeX = cms.double(0.44),        
            sizeY = cms.double(0.20),            
            header = cms.string(''),          
            option = cms.string('brNDC'),       
            borderSize = cms.int32(0),          
            fillColor = cms.int32(0)             
        )
    ),

    labels = cms.PSet(
        mcNormScale = copy.deepcopy(label_mcNormScale)
    ),

    drawOptionEntries = cms.PSet(
        bgEstData = copy.deepcopy(drawOption_green_eff),
        bgEstPure = copy.deepcopy(drawOption_lightBlue_eff),
        finalEvtSel = copy.deepcopy(drawOption_red_eff)
    ),

    drawJobs = drawTemplateHistConfiguratorZtoMuTau.configure(),

    canvasSizeX = cms.int32(800),
    canvasSizeY = cms.int32(640),                         

    outputFilePath = cms.string('./plots/'),
    #outputFileName = cms.string('plotsTemplateHistZtoMuTau.ps')
    indOutputFileName = cms.string('plotTemplateHistZtoMuTau_#PLOT#.eps')
)

#--------------------------------------------------------------------------------
# produce auxiliary histograms representing bias of visible muon + tau-jet mass distribution
# introduced by differences in event selection between final analysis and background enriched samples
#
# NOTE:
#  minuend    = contribution of (pure) signal/background process expected in final analysis
#               (estimated by Monte Carlo)
#  subtrahend = template histogram taken from background enriched sample,
#               including contributions from other signal/background processes
#               (determined by (pseudo)data)
#  difference = minuend - subtrahend
#
# --> in order to account for bias between distribution observed in final analysis
#     and the shapes of signal/background templates fitted to that distribution
#     one needs an **upward** fluctuation of the histogram representing the difference,
#     using a Gaussian of mean 0. and variance 1.
#
#--------------------------------------------------------------------------------

process.prodSysBiasHistZtoMuTau = cms.EDAnalyzer("DQMHistSubtractor",
    config = cms.VPSet(
        cms.PSet(
            meNameMinuend = cms.string(dqmDirectory_Ztautau_finalEvtSel + '/' + meName_diTauMvis12_norm),
            meNameSubtrahend = cms.string(dqmDirectory_Ztautau_template + '/' + meName_diTauMvis12_norm),
            meNameDifference = cms.string(dqmDirectory_Ztautau_systematics + '/' + 'bias' + '/' + meName_diTauMvis12_norm)
        ),
        cms.PSet(
            meNameMinuend = cms.string(dqmDirectory_Zmumu_finalEvtSel + '/' + meName_diTauMvis12_norm),
            meNameSubtrahend = cms.string(dqmDirectory_ZmumuJetMisId_correctedTemplate_data + '/' + meName_diTauMvis12_norm),
            meNameDifference = cms.string(dqmDirectory_ZmumuJetMisId_systematics + '/' + 'bias' + '/' + meName_diTauMvis12_norm)
        ),
        cms.PSet(
            meNameMinuend = cms.string(dqmDirectory_Zmumu_finalEvtSel + '/' + meName_diTauMvis12_norm),
            meNameSubtrahend = cms.string(dqmDirectory_ZmumuMuonMisId_template_data + '/' + meName_diTauMvis12_norm),
            meNameDifference = cms.string(dqmDirectory_ZmumuMuonMisId_systematics + '/' + 'bias' + '/' + meName_diTauMvis12_norm)
        ),
        cms.PSet(
            meNameMinuend = cms.string(dqmDirectory_WplusJets_finalEvtSel + '/' + meName_diTauMvis12_norm),
            meNameSubtrahend = cms.string(dqmDirectory_WplusJets_correctedTemplate_data + '/' + meName_diTauMvis12_norm),
            meNameDifference = cms.string(dqmDirectory_WplusJets_systematics + '/' + 'bias' + '/' + meName_diTauMvis12_norm)
        ),
        cms.PSet(
            meNameMinuend = cms.string(dqmDirectory_TTplusJets_finalEvtSel + '/' + meName_diTauMvis12_norm),
            meNameSubtrahend = cms.string(dqmDirectory_TTplusJets_template_data + '/' + meName_diTauMvis12_norm),
            meNameDifference = cms.string(dqmDirectory_TTplusJets_systematics + '/' + 'bias' + '/' + meName_diTauMvis12_norm)
        ),
        cms.PSet(
            meNameMinuend = cms.string(dqmDirectory_QCD_finalEvtSel + '/' + meName_diTauMvis12_norm),
            meNameSubtrahend = cms.string(dqmDirectory_QCD_template_data + '/' + meName_diTauMvis12_norm),
            meNameDifference = cms.string(dqmDirectory_QCD_systematics + '/' + 'bias' + '/' + meName_diTauMvis12_norm)
        )
    )                                                       
)

#--------------------------------------------------------------------------------
# produce auxiliary histograms representing uncertainties on shape of template histograms
# fitted to visible muon + tau-jet mass distribution
# caused by imprecise knowledge of contributions from Ztautau signal and other background processes
#--------------------------------------------------------------------------------

process.prodSysBgEnrichedSamplePurityHistZtoMuTau = cms.EDAnalyzer("DQMHistSubtractor",
    config = cms.VPSet(
        cms.PSet(
            meNameMinuend = cms.string(dqmDirectory_Ztautau_template + '/' + meName_diTauMvis12_norm),
            meNameSubtrahend = cms.string(dqmDirectory_Ztautau_template + '/' + meName_diTauMvis12_norm),
            meNameDifference = cms.string(dqmDirectory_Ztautau_systematics + '/' + 'bgEnrichedSamplePurity' + '/' + meName_diTauMvis12_norm)
        ),
        cms.PSet(
            meNameMinuend = cms.string(dqmDirectory_ZmumuJetMisId_correctedTemplate_data + '/' + meName_diTauMvis12_norm),
            meNameSubtrahend = cms.string(dqmDirectory_ZmumuJetMisId_correctedTemplate_pure + '/' + meName_diTauMvis12_norm),
            meNameDifference = cms.string(dqmDirectory_ZmumuJetMisId_systematics + '/' + 'bgEnrichedSamplePurity' + '/' + meName_diTauMvis12_norm)
        ),
        cms.PSet(
            meNameMinuend = cms.string(dqmDirectory_ZmumuMuonMisId_template_data + '/' + meName_diTauMvis12_norm),
            meNameSubtrahend = cms.string(dqmDirectory_ZmumuMuonMisId_template_pure + '/' + meName_diTauMvis12_norm),
            meNameDifference = cms.string(dqmDirectory_ZmumuMuonMisId_systematics + '/' + 'bgEnrichedSamplePurity' + '/' + meName_diTauMvis12_norm)
        ),
        cms.PSet(
            meNameMinuend = cms.string(dqmDirectory_WplusJets_correctedTemplate_data + '/' + meName_diTauMvis12_norm),
            meNameSubtrahend = cms.string(dqmDirectory_WplusJets_correctedTemplate_pure + '/' + meName_diTauMvis12_norm),
            meNameDifference = cms.string(dqmDirectory_WplusJets_systematics + '/' + 'bgEnrichedSamplePurity' + '/' + meName_diTauMvis12_norm)
        ),
        cms.PSet(
            meNameMinuend = cms.string(dqmDirectory_TTplusJets_template_data + '/' + meName_diTauMvis12_norm),
            meNameSubtrahend = cms.string(dqmDirectory_TTplusJets_template_pure + '/' + meName_diTauMvis12_norm),
            meNameDifference = cms.string(dqmDirectory_TTplusJets_systematics + '/' + 'bgEnrichedSamplePurity' + '/' + meName_diTauMvis12_norm)
        ),
        cms.PSet(
            meNameMinuend = cms.string(dqmDirectory_QCD_template_data + '/' + meName_diTauMvis12_norm),
            meNameSubtrahend = cms.string(dqmDirectory_QCD_template_pure + '/' + meName_diTauMvis12_norm),
            meNameDifference = cms.string(dqmDirectory_QCD_systematics + '/' + 'bgEnrichedSamplePurity' + '/' + meName_diTauMvis12_norm)
        )
    )                                                       
)

#--------------------------------------------------------------------------------
# scale auxiliary histograms representing systematic uncertainties
# to normalization of shape templates
# (normalization is needed in order to combine systematic uncertainties
#  with statistical uncertainties to total uncertainty on shape templates)
#--------------------------------------------------------------------------------

process.scaleSysHistZtoMuTau = cms.EDAnalyzer("DQMHistScaler",
    config = cms.VPSet(
        cms.PSet(
            meName_input = cms.string(dqmDirectory_Ztautau_systematics + '/' + 'bias' + '/' + meName_diTauMvis12_norm),
            meName_output = cms.string(dqmDirectory_Ztautau_systematics + '/' + 'bias' + '/' + meName_diTauMvis12),
            meNameScaleFactor = cms.string(dqmDirectory_Ztautau_template + '/' + meName_diTauMvis12),
            meTypeScaleFactor = cms.string("hist")
        ),
        cms.PSet(
            meName_input = cms.string(dqmDirectory_Ztautau_systematics + '/' + 'bgEnrichedSamplePurity' + '/' + meName_diTauMvis12_norm),
            meName_output = cms.string(dqmDirectory_Ztautau_systematics + '/' + 'bgEnrichedSamplePurity' + '/' + meName_diTauMvis12),
            meNameScaleFactor = cms.string(dqmDirectory_Ztautau_template + '/' + meName_diTauMvis12),
            meTypeScaleFactor = cms.string("hist")
        ),
        cms.PSet(
            meName_input = cms.string(dqmDirectory_ZmumuJetMisId_systematics + '/' + 'bias' + '/' + meName_diTauMvis12_norm),
            meName_output = cms.string(dqmDirectory_ZmumuJetMisId_systematics + '/' + 'bias' + '/' + meName_diTauMvis12),
            meNameScaleFactor = cms.string(dqmDirectory_ZmumuJetMisId_correctedTemplate_data + '/' + meName_diTauMvis12),
            meTypeScaleFactor = cms.string("hist")
        ),
        cms.PSet(
            meName_input = cms.string(dqmDirectory_ZmumuJetMisId_systematics + '/' + 'bgEnrichedSamplePurity' + '/' + meName_diTauMvis12_norm),
            meName_output = cms.string(dqmDirectory_ZmumuJetMisId_systematics + '/' + 'bgEnrichedSamplePurity' + '/' + meName_diTauMvis12),
            meNameScaleFactor = cms.string(dqmDirectory_ZmumuJetMisId_correctedTemplate_data + '/' + meName_diTauMvis12),
            meTypeScaleFactor = cms.string("hist")
        ),
        cms.PSet(
            meName_input = cms.string(dqmDirectory_ZmumuMuonMisId_systematics + '/' + 'bias' + '/' + meName_diTauMvis12_norm),
            meName_output = cms.string(dqmDirectory_ZmumuMuonMisId_systematics + '/' + 'bias' + '/' + meName_diTauMvis12),
            meNameScaleFactor = cms.string(dqmDirectory_ZmumuMuonMisId_template_data + '/' + meName_diTauMvis12),
            meTypeScaleFactor = cms.string("hist")
        ),
        cms.PSet(
            meName_input = cms.string(dqmDirectory_ZmumuMuonMisId_systematics + '/' + 'bgEnrichedSamplePurity' + '/' + meName_diTauMvis12_norm),
            meName_output = cms.string(dqmDirectory_ZmumuMuonMisId_systematics + '/' + 'bgEnrichedSamplePurity' + '/' + meName_diTauMvis12),
            meNameScaleFactor = cms.string(dqmDirectory_ZmumuMuonMisId_template_data + '/' + meName_diTauMvis12),
            meTypeScaleFactor = cms.string("hist")
        ),
        cms.PSet(
            meName_input = cms.string(dqmDirectory_WplusJets_systematics + '/' + 'bias' + '/' + meName_diTauMvis12_norm),
            meName_output = cms.string(dqmDirectory_WplusJets_systematics + '/' + 'bias' + '/' + meName_diTauMvis12),
            meNameScaleFactor = cms.string(dqmDirectory_WplusJets_correctedTemplate_data + '/' + meName_diTauMvis12),
            meTypeScaleFactor = cms.string("hist")
        ),
        cms.PSet(
            meName_input = cms.string(dqmDirectory_WplusJets_systematics + '/' + 'bgEnrichedSamplePurity' + '/' + meName_diTauMvis12_norm),
            meName_output = cms.string(dqmDirectory_WplusJets_systematics + '/' + 'bgEnrichedSamplePurity' + '/' + meName_diTauMvis12),
            meNameScaleFactor = cms.string(dqmDirectory_WplusJets_correctedTemplate_data + '/' + meName_diTauMvis12),
            meTypeScaleFactor = cms.string("hist")
        ),
        cms.PSet(
            meName_input = cms.string(dqmDirectory_TTplusJets_systematics + '/' + 'bias' + '/' + meName_diTauMvis12_norm),
            meName_output = cms.string(dqmDirectory_TTplusJets_systematics + '/' + 'bias' + '/' + meName_diTauMvis12),
            meNameScaleFactor = cms.string(dqmDirectory_TTplusJets_template_data + '/' + meName_diTauMvis12),
            meTypeScaleFactor = cms.string("hist")
        ),
        cms.PSet(
            meName_input = cms.string(dqmDirectory_TTplusJets_systematics + '/' + 'bgEnrichedSamplePurity' + '/' + meName_diTauMvis12_norm),
            meName_output = cms.string(dqmDirectory_TTplusJets_systematics + '/' + 'bgEnrichedSamplePurity' + '/' + meName_diTauMvis12),
            meNameScaleFactor = cms.string(dqmDirectory_TTplusJets_template_data + '/' + meName_diTauMvis12),
            meTypeScaleFactor = cms.string("hist")
        ),
        cms.PSet(
            meName_input = cms.string(dqmDirectory_QCD_systematics + '/' + 'bias' + '/' + meName_diTauMvis12_norm),
            meName_output = cms.string(dqmDirectory_QCD_systematics + '/' + 'bias' + '/' + meName_diTauMvis12),
            meNameScaleFactor = cms.string(dqmDirectory_QCD_template_data + '/' + meName_diTauMvis12),
            meTypeScaleFactor = cms.string("hist")
        ),
        cms.PSet(
            meName_input = cms.string(dqmDirectory_QCD_systematics + '/' + 'bgEnrichedSamplePurity' + '/' + meName_diTauMvis12_norm),
            meName_output = cms.string(dqmDirectory_QCD_systematics + '/' + 'bgEnrichedSamplePurity' + '/' + meName_diTauMvis12),
            meNameScaleFactor = cms.string(dqmDirectory_QCD_template_data + '/' + meName_diTauMvis12),
            meTypeScaleFactor = cms.string("hist")
        )
    )
)

process.dumpSysHistZtoMuTau = cms.EDAnalyzer("DQMDumpHistogram",
    meNames = cms.vstring(
        dqmDirectory_Ztautau_finalEvtSel + '/' + meName_diTauMvis12_norm,
        dqmDirectory_Ztautau_template + '/' + meName_diTauMvis12_norm,
        dqmDirectory_Ztautau_systematics + '/' + 'bias' + '/' + meName_diTauMvis12_norm,
        dqmDirectory_Ztautau_systematics + '/' + 'bias' + '/' + meName_diTauMvis12,
        dqmDirectory_Ztautau_systematics + '/' + 'bgEnrichedSamplePurity' + '/' + meName_diTauMvis12_norm,
        dqmDirectory_Ztautau_systematics + '/' + 'bgEnrichedSamplePurity' + '/' + meName_diTauMvis12,
        dqmDirectory_Zmumu_finalEvtSel + '/' + meName_diTauMvis12_norm,
        dqmDirectory_ZmumuJetMisId_correctedTemplate_data + '/' + meName_diTauMvis12_norm,
        dqmDirectory_ZmumuJetMisId_correctedTemplate_pure + '/' + meName_diTauMvis12_norm,
        dqmDirectory_ZmumuJetMisId_systematics + '/' + 'bias' + '/' + meName_diTauMvis12_norm,
        dqmDirectory_ZmumuJetMisId_systematics + '/' + 'bias' + '/' + meName_diTauMvis12,
        dqmDirectory_ZmumuJetMisId_systematics + '/' + 'bgEnrichedSamplePurity' + '/' + meName_diTauMvis12_norm,
        dqmDirectory_ZmumuJetMisId_systematics + '/' + 'bgEnrichedSamplePurity' + '/' + meName_diTauMvis12,
        dqmDirectory_ZmumuMuonMisId_template_data + '/' + meName_diTauMvis12_norm,
        dqmDirectory_ZmumuMuonMisId_template_pure + '/' + meName_diTauMvis12_norm,
        dqmDirectory_ZmumuMuonMisId_systematics + '/' + 'bias' + '/' + meName_diTauMvis12_norm,
        dqmDirectory_ZmumuMuonMisId_systematics + '/' + 'bias' + '/' + meName_diTauMvis12,
        dqmDirectory_ZmumuMuonMisId_systematics + '/' + 'bgEnrichedSamplePurity' + '/' + meName_diTauMvis12_norm,
        dqmDirectory_ZmumuMuonMisId_systematics + '/' + 'bgEnrichedSamplePurity' + '/' + meName_diTauMvis12,
        dqmDirectory_WplusJets_finalEvtSel + '/' + meName_diTauMvis12_norm,
        dqmDirectory_WplusJets_correctedTemplate_data + '/' + meName_diTauMvis12_norm,
        dqmDirectory_WplusJets_correctedTemplate_pure + '/' + meName_diTauMvis12_norm,
        dqmDirectory_WplusJets_systematics + '/' + 'bias' + '/' + meName_diTauMvis12_norm,
        dqmDirectory_WplusJets_systematics + '/' + 'bias' + '/' + meName_diTauMvis12,
        dqmDirectory_WplusJets_systematics + '/' + 'bgEnrichedSamplePurity' + '/' + meName_diTauMvis12_norm,
        dqmDirectory_WplusJets_systematics + '/' + 'bgEnrichedSamplePurity' + '/' + meName_diTauMvis12,
        dqmDirectory_TTplusJets_finalEvtSel + '/' + meName_diTauMvis12_norm,
        dqmDirectory_TTplusJets_template_data + '/' + meName_diTauMvis12_norm,
        dqmDirectory_TTplusJets_template_pure + '/' + meName_diTauMvis12_norm,
        dqmDirectory_TTplusJets_systematics + '/' + 'bias' + '/' + meName_diTauMvis12_norm,
        dqmDirectory_TTplusJets_systematics + '/' + 'bias' + '/' + meName_diTauMvis12,
        dqmDirectory_TTplusJets_systematics + '/' + 'bgEnrichedSamplePurity' + '/' + meName_diTauMvis12_norm,
        dqmDirectory_TTplusJets_systematics + '/' + 'bgEnrichedSamplePurity' + '/' + meName_diTauMvis12,
        dqmDirectory_QCD_finalEvtSel + '/' + meName_diTauMvis12_norm,
        dqmDirectory_QCD_template_data + '/' + meName_diTauMvis12_norm,
        dqmDirectory_QCD_template_pure + '/' + meName_diTauMvis12_norm,
        dqmDirectory_QCD_systematics + '/' + 'bias' + '/' + meName_diTauMvis12_norm,
        dqmDirectory_QCD_systematics + '/' + 'bias' + '/' + meName_diTauMvis12,
        dqmDirectory_QCD_systematics + '/' + 'bgEnrichedSamplePurity' + '/' + meName_diTauMvis12_norm,
        dqmDirectory_QCD_systematics + '/' + 'bgEnrichedSamplePurity' + '/' + meName_diTauMvis12,
    )
)                                             

#--------------------------------------------------------------------------------
# fit template histograms to distribution of visible muon + tau-jet mass in (pseudo)Data,
# in order to determine normalization factors of individual background processes
#--------------------------------------------------------------------------------

# fit W + jets, ttbar + jets and QCD background templates
# with Landau distribution convoluted with Gaussian
diTauMvis12_smoothing = cms.PSet(
    pluginName = cms.string("Landau_convoluted_with_Gaussian"),
    pluginType = cms.string("SmoothLandau_x_GaussPdfWrapper"), # defaults to SmoothGenericPdfWrapper
    xMin = cms.double(20.),
    xMax = cms.double(200.),
    excludeBinsX = cms.vdouble(),
    parameter = cms.PSet(
        width = cms.PSet( # width (scale) parameter of Landau density
            initial = cms.double(5.),
            min = cms.double(0.01),
            max = cms.double(50.)
        ),
        mp = cms.PSet( # most probable (MP, location) parameter of Landau density
            initial = cms.double(50.),
            min = cms.double(30.),
            max = cms.double(150.) 
        ),
        area = cms.PSet( # total area (integral from -inf to +inf, normalization constant)
            initial = cms.double(1.),
            min = cms.double(0.1),
            max = cms.double(10.)
        ),
        gsigma = cms.PSet( # width (sigma) of convoluted Gaussian function
            initial = cms.double(10.),
            min = cms.double(1.),
            max = cms.double(50.)
        )
    )
)

process.fitZtoMuTau = cms.EDAnalyzer("TemplateHistFitter",                                          
    processes = cms.PSet(
        Ztautau = cms.PSet(
            templates = cms.PSet(
                diTauMvis12 = cms.PSet(
                    meName = cms.string(dqmDirectory_Ztautau_template + '/' + meName_diTauMvis12),
                    #meName = cms.string(dqmDirectory_Ztautau_finalEvtSel + '/' + meName_diTauMvis12),
                    fitSimultaneously = cms.bool(False),                                 
                    smoothing = cms.PSet(
                        pluginName = cms.string("diTauMvis12SmoothingZtautau"),
                        pluginType = cms.string("SmoothGenericPdfWrapper"),
                        # fit Z --> tau+ tau- peak with sum of log-normal and skewed Gaussian distribution
                        formula = cms.string("par1*::ROOT::Math::lognormal_pdf(@x, TMath::Log(par4), par2, par3)"
                                             "+ (1 - par1)*TMath::Gaus(@x, par5, par6)*(1 + TMath::Erf(par7*@x))"),
                        xMin = cms.double(20.),
                        xMax = cms.double(120.),
                        parameter = cms.PSet(
                            par1 = cms.PSet(
                                initial = cms.double(0.75),
                                min = cms.double(0.),
                                max = cms.double(1.)
                            ),
                            par2 = cms.PSet(
                                initial = cms.double(0.5),
                                min = cms.double(0.),
                                max = cms.double(10.)
                            ),
                            par3 = cms.PSet(
                                initial = cms.double(40.),
                                min = cms.double(0.),
                                max = cms.double(100.)
                            ),
                            par4 = cms.PSet(
                                initial = cms.double(10.),
                                min = cms.double(0.),
                                max = cms.double(100.)
                            ),
                            par5 = cms.PSet(
                                initial = cms.double(55.)
                            ),
                            par6 = cms.PSet(
                                initial = cms.double(10.),
                                min = cms.double(0.),
                                max = cms.double(100.)
                            ),
                            par7 = cms.PSet(
                                initial = cms.double(0.0001)
                            )
                        )
                    )
                    ##interpolation = cms.PSet(
                    ##    meName_upperBound = cms.string(dqmDirectory_Ztautau_templateSysTauEnUp + '/' + meName_diTauMvis12),
                    ##    meName_lowerBound = cms.string(dqmDirectory_Ztautau_templateSysTauEnDown + '/' + meName_diTauMvis12)
                    ##)                                 
                )
            ),    
            norm = cms.PSet(
                initial = cms.double(750.)
            ),
            drawOptions = drawOption_Ztautau_separate                
        ),
        ZmumuJetMisId = cms.PSet(
            templates = cms.PSet(
                diTauMvis12 = cms.PSet(
                    meName = cms.string(dqmDirectory_ZmumuJetMisId_correctedTemplate_data + '/' + meName_diTauMvis12),
                    fitSimultaneously = cms.bool(True),                                    
                    smoothing = diTauMvis12_smoothing.clone(
                        pluginName = cms.string("diTauMvis12SmoothingZmumuJetMisId"),
                        ##xMax = cms.double(80.),
                        excludeBinsX = cms.vdouble(82.5, 87.5, 92.5, 97.5)
                    )
                )
            ),    
            norm = cms.PSet(
                initial = cms.double(25.)
            ),
            drawOptions = drawOption_Zmumu_separate 
        ),
        ZmumuMuonMisId = cms.PSet(
            templates = cms.PSet(
                diTauMvis12 = cms.PSet(
                    meName = cms.string(dqmDirectory_ZmumuMuonMisId_template_data + '/' + meName_diTauMvis12),
                    fitSimultaneously = cms.bool(True),                                   
                    smoothing = cms.PSet(
                        pluginName = cms.string("diTauMvis12SmoothingZmumuMuonMisId"),
                        pluginType = cms.string("SmoothGenericPdfWrapper"),
                        # fit Z --> mu+ mu- peak with Voigt function,
                        # the convolution of a Breit-Wigner profile with a Gaussian (smearing)
                        formula = cms.string("TMath::Voigt(@x - x0, gsigma, gamma)"),
                        xMin = cms.double(80.),
                        xMax = cms.double(100.),                        
                        parameter = cms.PSet(
                            x0 = cms.PSet(
                                initial = cms.double(90.)
                            ),
                            gsigma = cms.PSet(
                                initial = cms.double(0.1)
                            ),
                            gamma = cms.PSet(
                                initial = cms.double(2.5)
                            )
                        )
                    )
                )
            ),    
            norm = cms.PSet(
                initial = cms.double(25.)
            ),
            drawOptions = drawOption_Zmumu_separate 
        ),
        WplusJets = cms.PSet(
            templates = cms.PSet(
                diTauMvis12 = cms.PSet(
                    meName = cms.string(dqmDirectory_WplusJets_correctedTemplate_data + '/' + meName_diTauMvis12),
                    #meName = cms.string(dqmDirectory_WplusJets_finalEvtSel + '/' + meName_diTauMvis12),
                    fitSimultaneously = cms.bool(True),                                   
                    smoothing = diTauMvis12_smoothing.clone(
                        pluginName = cms.string("diTauMvis12SmoothingWplusJets")
                    )
                )
            ),    
            norm = cms.PSet(
                initial = cms.double(250.)
            ),
            drawOptions = drawOption_WplusJets_separate 
        ),
        TTplusJets = cms.PSet(
            templates = cms.PSet(
                diTauMvis12 = cms.PSet(
                    meName = cms.string(dqmDirectory_TTplusJets_template_data + '/' + meName_diTauMvis12),
                    fitSimultaneously = cms.bool(True),                                   
                    smoothing = diTauMvis12_smoothing.clone(
                        pluginName = cms.string("diTauMvis12SmoothingTTplusJets")
                    )
                )
            ),    
            norm = cms.PSet(
                initial = cms.double(25.)
            ),
            drawOptions = drawOption_TTplusJets_separate 
        ),
        QCD = cms.PSet(
            templates = cms.PSet(
                diTauMvis12 = cms.PSet(
                    meName = cms.string(dqmDirectory_QCD_template_data + '/' + meName_diTauMvis12),
                    #meName = cms.string(dqmDirectory_QCD_finalEvtSel + '/' + meName_diTauMvis12),
                    fitSimultaneously = cms.bool(True),                                    
                    smoothing = diTauMvis12_smoothing.clone(
                        pluginName = cms.string("diTauMvis12SmoothingQCD"),
                        xMax = cms.double(150.)
                    )
                )
            ),    
            norm = cms.PSet(
                initial = cms.double(250.)
            ),
            drawOptions = drawOption_QCD_separate
        )
    ),

    # use "pseudo" data-samples consisting of all Monte Carlo processes for testing                      
    data = cms.PSet(
        distributions = cms.PSet(
            diTauMvis12 = cms.PSet(
                meName = cms.string(dqmDirectory_data_finalEvtSel + '/' + meName_diTauMvis12)
            )
        )
    ),

    fit = cms.PSet(
        algorithm = cms.PSet(
            pluginName = cms.string("fitBgEstTemplateZtoMuTauAlgorithm"),
            #pluginType = cms.string("TemplateFitAdapter_TFractionFitter")
            pluginType = cms.string("TemplateFitAdapter_RooFit")
        ),
        variables = cms.PSet(
            diTauMvis12 = cms.PSet(
                name = cms.string("diTauMvis12"),
                title = cms.string("M_{vis}^{#mu + #tau-jet}"),
                min = cms.double(20.), # default:  20.
                max = cms.double(200.) # default: 200.
            )
        ),
        # constrain normalization of W + jets, ttbar + jets and QCD backgrounds
        # to Monte Carlo expectation multiplied by "k-factors" determined
        # in background enriched samples
        ##constraints = cms.PSet(
        ##    ZmumuJetMisId = cms.PSet(
        ##        norm = cms.PSet(
        ##            value = cms.double(1.*25.),
        ##            uncertainty = cms.double(25.)
        ##        )
        ##    ),
        ##    ZmumuMuonMisId = cms.PSet(
        ##        norm = cms.PSet(
        ##            value = cms.double(1.*25.),
        ##            uncertainty = cms.double(25.)
        ##        )
        ##    ),
        ##    WplusJets = cms.PSet(
        ##        norm = cms.PSet(
        ##            value = cms.double(1.*500.),
        ##            uncertainty = cms.double(250.)
        ##        )
        ##    ),
        ##    TTplusJets = cms.PSet(
        ##        norm = cms.PSet(
        ##            value = cms.double(1.*100.),
        ##            uncertainty = cms.double(100.)
        ##        )
        ##    ),
        ##    QCD = cms.PSet(
        ##        norm = cms.PSet(
        ##            value = cms.double(1.*100.),
        ##            uncertainty = cms.double(100.)
        ##        )
        ##    )
        ##),
        cutUnfittedRegion = cms.bool(False),
        #cutUnfittedRegion = cms.bool(True),
        verbosity = cms.PSet(
            printLevel = cms.int32(1),
            printWarnings = cms.bool(True)
        )
    ),

    estStatUncertainties = cms.PSet(
        numSamplings = cms.int32(10),
        chi2redMax = cms.double(10.),
        verbosity = cms.PSet(
            printLevel = cms.int32(-1),
            printWarnings = cms.bool(False)
        )
    ),

    estSysUncertainties = cms.PSet(
        fluctuations = cms.PSet(
            bias = cms.PSet(
                meNames = cms.PSet(
                    Ztautau = cms.PSet(
                        diTauMvis12 = cms.string(dqmDirectory_Ztautau_systematics + '/' + 'bias' + '/' + meName_diTauMvis12)
                    ),
                    ZmumuJetMisId = cms.PSet(
                        diTauMvis12 = cms.string(dqmDirectory_ZmumuJetMisId_systematics + '/' + 'bias' + '/' + meName_diTauMvis12)
                    ),
                    ZmumuMuonMisId = cms.PSet(
                        diTauMvis12 = cms.string(dqmDirectory_ZmumuMuonMisId_systematics + '/' + 'bias' + '/' + meName_diTauMvis12)
                    ),
                    WplusJets = cms.PSet(
                        diTauMvis12 = cms.string(dqmDirectory_WplusJets_systematics + '/' + 'bias' + '/' + meName_diTauMvis12)
                    ),
                    TTplusJets = cms.PSet(
                        diTauMvis12 = cms.string(dqmDirectory_TTplusJets_systematics + '/' + 'bias' + '/' + meName_diTauMvis12)
                    ),
                    QCD = cms.PSet(
                        diTauMvis12 = cms.string(dqmDirectory_QCD_systematics + '/' + 'bias' + '/' + meName_diTauMvis12)
                    )
                ),
                pullRMS = cms.double(1.),
                pullMin = cms.double(0.),
                pullMax = cms.double(1.),
                mode = cms.string("coherent") # coherent/incoherent
            ),
            bgSamplePurity = cms.PSet(
                meNames = cms.PSet(
                    Ztautau = cms.PSet(
                        diTauMvis12 = cms.string(dqmDirectory_Ztautau_systematics + '/' + 'bgEnrichedSamplePurity' + '/' + meName_diTauMvis12)
                    ),
                    ZmumuJetMisId = cms.PSet(
                        diTauMvis12 = cms.string(dqmDirectory_ZmumuJetMisId_systematics + '/' + 'bgEnrichedSamplePurity' + '/' + meName_diTauMvis12)
                    ),
                    ZmumuMuonMisId = cms.PSet(
                        diTauMvis12 = cms.string(dqmDirectory_ZmumuMuonMisId_systematics + '/' + 'bgEnrichedSamplePurity' + '/' + meName_diTauMvis12)
                    ),
                    WplusJets = cms.PSet(
                        diTauMvis12 = cms.string(dqmDirectory_WplusJets_systematics + '/' + 'bgEnrichedSamplePurity' + '/' + meName_diTauMvis12)
                    ),
                    TTplusJets = cms.PSet(
                        diTauMvis12 = cms.string(dqmDirectory_TTplusJets_systematics + '/' + 'bgEnrichedSamplePurity' + '/' + meName_diTauMvis12)
                    ),
                    QCD = cms.PSet(
                        diTauMvis12 = cms.string(dqmDirectory_QCD_systematics + '/' + 'bgEnrichedSamplePurity' + '/' + meName_diTauMvis12)
                    )
                ),
                pullRMS = cms.double(1.),
                pullMin = cms.double(-1.),
                pullMax = cms.double(+2.),
                mode = cms.string("coherent")
            )
        ),       
        numSamplings = cms.int32(10),
        chi2redMax = cms.double(10.),
        verbosity = cms.PSet(
            printLevel = cms.int32(-1),
            printWarnings = cms.bool(False)
        )
    ),                                     

    output = cms.PSet(
        controlPlots = cms.PSet(
            fileName = cms.string("./plots/fitBgEstTemplateZtoMuTau_#PLOT#.eps")
        ),
        fitResults = cms.PSet(
            dqmDirectory = cms.string(processName + '/' + 'fitResults')
        )
    )                                      
)                          

process.fitZtoMuTauSysTauEnUp = copy.deepcopy(process.fitZtoMuTau)
process.fitZtoMuTauSysTauEnUp.processes.Ztautau.templates.diTauMvis12.meName = cms.string(dqmDirectory_Ztautau_templateSysTauEnUp + '/' + meName_diTauMvis12)
process.fitZtoMuTauSysTauEnUp.output.controlPlots.fileName = cms.string("./plots/fitBgEstTemplateZtoMuTau_#PLOT#_sysTauEnUp.eps")

process.fitZtoMuTauSysTauEnDown = copy.deepcopy(process.fitZtoMuTau)
process.fitZtoMuTauSysTauEnDown.processes.Ztautau.templates.diTauMvis12.meName = cms.string(dqmDirectory_Ztautau_templateSysTauEnDown + '/' + meName_diTauMvis12)
process.fitZtoMuTauSysTauEnDown.output.controlPlots.fileName = cms.string("./plots/fitBgEstTemplateZtoMuTau_#PLOT#_sysTauEnDown.eps")

process.saveFitResultsZtoMuTau = cms.EDAnalyzer("DQMSimpleFileSaver",
    outputFileName = cms.string('fitBgEstTemplateZtoMuTau_results.root'),
    outputCommands = cms.vstring(
        'drop harvested/*',
        'keep template/*',
        'keep harvested/*/zMuTauAnalyzer/afterEvtSelDiMuPairZmumuHypothesisVeto/*'
    )
)

process.dumpDQMStore = cms.EDAnalyzer("DQMStoreDump")

process.p = cms.Path(
    process.loadTemplateHistZtoMuTau_Ztautau
   + process.loadTemplateHistZtoMuTau
   + process.loadAnalysisHistZtoMuTau
   #+ process.dumpDQMStore
   + process.normalizeTemplateHistZtoMuTau
   + process.normalizeAnalysisHistZtoMuTau
   + process.prodSysBiasHistZtoMuTau
   + process.prodSysBgEnrichedSamplePurityHistZtoMuTau
   + process.scaleSysHistZtoMuTau
   #+ process.dumpSysHistZtoMuTau
   #+ process.dumpDQMStore
   + process.plotTemplateHistZtoMuTau
   + process.fitZtoMuTau + process.fitZtoMuTauSysTauEnUp + process.fitZtoMuTauSysTauEnDown
   + process.saveFitResultsZtoMuTau
)

# print-out all python configuration parameter information
#print process.dumpPython()


  
