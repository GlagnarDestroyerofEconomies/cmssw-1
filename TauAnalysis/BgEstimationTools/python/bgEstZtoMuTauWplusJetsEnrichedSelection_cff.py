import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.CandidateTools.tools.objSelConfigurator import *
from TauAnalysis.RecoTools.tools.eventSelFlagProdConfigurator import *

#--------------------------------------------------------------------------------
# select W + jets background enriched event sample
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------  
# produce collection of pat::Muons
#--------------------------------------------------------------------------------

from TauAnalysis.RecoTools.patMuonSelection_cfi import *
#
# Note: muonPt cut improves WplusJets/QCD ratio by about a factor five,
#       but significantly shifts the muon + tau-jet visible invariant mass distribution towards higher values.
#       In order to supress QCD background contamination (on a statistical basis),
#       could extract W + jets template shape from difference in muon + tau-jet visible invariant mass distributions
#       of opposite sign - same sign muon and tau-jet combinations.
#      (SS/OS ratio is close to one for QCD background; significant charge asymmetry expected for W + jets background)
#    
muonsBgEstWplusJetsEnrichedPt = copy.deepcopy(selectedPatMuonsPt15)
muonsBgEstWplusJetsEnrichedPt.cut = cms.string('pt > 15.')

muonsBgEstWplusJetsEnrichedVbTfId = copy.deepcopy(selectedPatMuonsVbTfId)

muonsBgEstWplusJetsEnrichedPFRelIso = copy.deepcopy(selectedPatMuonsPFRelIso)
muonsBgEstWplusJetsEnrichedPFRelIso.sumPtMax = cms.double(0.10)

muonSelConfiguratorBgEstWplusJetsEnriched = objSelConfigurator(
    [ muonsBgEstWplusJetsEnrichedPt,
      muonsBgEstWplusJetsEnrichedVbTfId,
      muonsBgEstWplusJetsEnrichedPFRelIso ],
    src = "selectedPatMuonsEta21Cumulative",
    pyModuleName = __name__,
    doSelIndividual = False
)

selectMuonsBgEstWplusJetsEnriched = muonSelConfiguratorBgEstWplusJetsEnriched.configure(pyNameSpace = locals())

#--------------------------------------------------------------------------------  
# produce collection of pat::Taus
#--------------------------------------------------------------------------------

from TauAnalysis.RecoTools.patPFTauSelection_cfi import *
from TauAnalysis.RecoTools.patPFTauSelectionForMuTau_cfi import *
#
# Note: probability for quark/gluon jets to pass tau track and ECAL isolation criteria
#       is higher for low Pt than for high Pt jets; the consequence is that muon + tau-jet visible invariant mass distribution
#       gets shifted towards higher values in case tau track and ECAL isolation criteria are not applied.
#       For this reason, either need to apply tau track and ECAL isolation criteria in selection of W + jets background enriched sample
#       or correct for template shape distortion by reweighting
#      (would gain a factor of about 2.5 in event statistics; reweighting of tauPt distribution not implemented yet, however)
#
tausBgEstWplusJetsEnrichedTaNCdiscr = copy.deepcopy(selectedPatTausTaNCdiscr)
tausBgEstWplusJetsEnrichedTaNCdiscr.cut = cms.string('tauID("byTaNCvloose") > 0.5 & tauID("byTaNCmedium") < 0.5')

tausBgEstWplusJetsEnrichedMuonVeto = copy.deepcopy(selectedPatTausMuonVeto)

tauSelConfiguratorBgEstWplusJetsEnriched = objSelConfigurator(
    [ tausBgEstWplusJetsEnrichedTaNCdiscr,
      tausBgEstWplusJetsEnrichedMuonVeto ],
    src = "selectedPatTausForMuTauLeadTrkPtCumulative",
    pyModuleName = __name__,
    doSelIndividual = False
)

selectTausBgEstWplusJetsEnriched = tauSelConfiguratorBgEstWplusJetsEnriched.configure(pyNameSpace = locals())

#--------------------------------------------------------------------------------  
# produce collection of vertices compatible with background-enriched lepton collections
#--------------------------------------------------------------------------------

from TauAnalysis.RecoTools.recoVertexSelectionForMuTau_cff import *

selectedPrimaryVertexForMuTauBgEstWplusJetsEnriched = selectedPrimaryVertexForMuTau.clone(
    srcParticles = cms.VInputTag(
        'muonsBgEstWplusJetsEnrichedPFRelIsoCumulative',
        'tausBgEstWplusJetsEnrichedMuonVetoCumulative'
    )
)
selectedPrimaryVertexQualityForMuTauBgEstWplusJetsEnriched = selectedPrimaryVertexQualityForMuTau.clone(
    src = cms.InputTag('selectedPrimaryVertexForMuTauBgEstWplusJetsEnriched')
)
selectedPrimaryVertexPositionForMuTauBgEstWplusJetsEnriched = selectedPrimaryVertexPositionForMuTau.clone(
    src = cms.InputTag('selectedPrimaryVertexQualityForMuTauBgEstWplusJetsEnriched')
)
selectedPrimaryVertexHighestPtTrackSumForMuTauBgEstWplusJetsEnriched = selectedPrimaryVertexHighestPtTrackSumForMuTau.clone(
    src = cms.InputTag('selectedPrimaryVertexPositionForMuTauBgEstWplusJetsEnriched')
)
selectPrimaryVertexForMuTauBgEstWplusJetsEnriched = cms.Sequence(
    selectedPrimaryVertexForMuTauBgEstWplusJetsEnriched
    * selectedPrimaryVertexQualityForMuTauBgEstWplusJetsEnriched
    * selectedPrimaryVertexPositionForMuTauBgEstWplusJetsEnriched
    * selectedPrimaryVertexHighestPtTrackSumForMuTauBgEstWplusJetsEnriched
)


#--------------------------------------------------------------------------------  
# produce collection of muon + tau-jet combinations
#--------------------------------------------------------------------------------

from TauAnalysis.CandidateTools.muTauPairProduction_cff import *

muTauPairsBgEstWplusJetsEnriched = allMuTauPairs.clone(
    srcLeg1 = cms.InputTag('muonsBgEstWplusJetsEnrichedPFRelIsoCumulative'),
    srcLeg2 = cms.InputTag('tausBgEstWplusJetsEnrichedMuonVetoCumulative'),
    verbosity = cms.untracked.int32(0)
)

produceMuTauPairsBgEstWplusJetsEnriched = cms.Sequence(muTauPairsBgEstWplusJetsEnriched)

muTauPairsBgEstWplusJetsEnrichedMt1MET = cms.EDFilter("PATMuTauPairSelector",
    src = cms.InputTag('muTauPairsBgEstWplusJetsEnriched'),                                                   
    cut = cms.string('mt1MET > 30.'),
    filter = cms.bool(False)
)

selectMuTauPairsBgEstWplusJetsEnriched = cms.Sequence(muTauPairsBgEstWplusJetsEnrichedMt1MET)

#--------------------------------------------------------------------------------  
# produce collection of pat::Jets used for central jet veto
# (in order to reject QCD di-jet events)
#--------------------------------------------------------------------------------

jetsBgEstWplusJetsEnrichedAntiOverlapWithLeptonsVeto = cms.EDFilter("PATJetAntiOverlapSelector",
    src = cms.InputTag("selectedPatJetsEt20Cumulative"),                                                                  
    srcNotToBeFiltered = cms.VInputTag(
        "selectedPatElectronsTrkIPcumulative",
        "muonsBgEstWplusJetsEnrichedPFRelIsoCumulative",
        "tausBgEstWplusJetsEnrichedMuonVetoCumulative"
    ),
    dRmin = cms.double(0.7),
    filter = cms.bool(False)                                           
)

jetsBgEstWplusJetsEnrichedAlpha0point1 = cms.EDFilter("PATJetAlphaSelector",
    src = cms.InputTag("jetsBgEstWplusJetsEnrichedAntiOverlapWithLeptonsVeto"),
    alphaMin = cms.double(0.1),
    filter = cms.bool(False)
)

selectJetsBgEstWplusJetsEnriched = cms.Sequence(
    jetsBgEstWplusJetsEnrichedAntiOverlapWithLeptonsVeto + jetsBgEstWplusJetsEnrichedAlpha0point1
)

#--------------------------------------------------------------------------------  
# produce boolean event selection flags
#--------------------------------------------------------------------------------

from TauAnalysis.Configuration.selectZtoMuTau_cff import *

cfgMuonPtCutBgEstWplusJetsEnriched = copy.deepcopy(cfgMuonPtCut)
cfgMuonPtCutBgEstWplusJetsEnriched.pluginName = cms.string('muonPtCutBgEstWplusJetsEnriched')
cfgMuonPtCutBgEstWplusJetsEnriched.src_cumulative = cms.InputTag('muonsBgEstWplusJetsEnrichedPtCumulative')
cfgMuonPtCutBgEstWplusJetsEnriched.systematics = cms.vstring()

cfgMuonVbTfIdCutBgEstWplusJetsEnriched = copy.deepcopy(cfgMuonVbTfIdCut)
cfgMuonVbTfIdCutBgEstWplusJetsEnriched.pluginName = cms.string('muonVbTfIdCutBgEstWplusJetsEnriched')
cfgMuonVbTfIdCutBgEstWplusJetsEnriched.src_cumulative = cms.InputTag('muonsBgEstWplusJetsEnrichedVbTfIdCumulative')
cfgMuonVbTfIdCutBgEstWplusJetsEnriched.systematics = cms.vstring()

cfgMuonPFRelIsoCutBgEstWplusJetsEnriched = copy.deepcopy(cfgMuonPFRelIsoCut)
cfgMuonPFRelIsoCutBgEstWplusJetsEnriched.pluginName = cms.string('muonPFRelIsoCutBgEstWplusJetsEnriched')
cfgMuonPFRelIsoCutBgEstWplusJetsEnriched.src_cumulative = cms.InputTag('muonsBgEstWplusJetsEnrichedPFRelIsoCumulative')
cfgMuonPFRelIsoCutBgEstWplusJetsEnriched.systematics = cms.vstring()

cfgTauTaNCdiscrCutBgEstWplusJetsEnriched = copy.deepcopy(cfgTauTaNCdiscrCut)
cfgTauTaNCdiscrCutBgEstWplusJetsEnriched.pluginName = cms.string('tauTaNCdiscrCutBgEstWplusJetsEnriched')
cfgTauTaNCdiscrCutBgEstWplusJetsEnriched.src_cumulative = cms.InputTag('tausBgEstWplusJetsEnrichedTaNCdiscrCumulative')
cfgTauTaNCdiscrCutBgEstWplusJetsEnriched.systematics = cms.vstring()

cfgTauMuonVetoBgEstWplusJetsEnriched = copy.deepcopy(cfgTauMuonVeto)
cfgTauMuonVetoBgEstWplusJetsEnriched.pluginName = cms.string('tauMuonVetoBgEstWplusJetsEnriched')
cfgTauMuonVetoBgEstWplusJetsEnriched.src_cumulative = cms.InputTag('tausBgEstWplusJetsEnrichedMuonVetoCumulative')
cfgTauMuonVetoBgEstWplusJetsEnriched.systematics = cms.vstring()

cfgMuTauPairBgEstWplusJetsEnriched = cms.PSet(
    pluginName = cms.string('muTauPairBgEstWplusJetsEnriched'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src = cms.InputTag('muTauPairsBgEstWplusJetsEnriched'),
    minNumber = cms.uint32(1)
)

cfgMuTauPairMt1MEtBgEstWplusJetsEnriched= cms.PSet(
    pluginName = cms.string('muTauPairMt1MEtBgEstWplusJetsEnriched'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src = cms.InputTag('muTauPairsBgEstWplusJetsEnrichedMt1MET'),
    minNumber = cms.uint32(1)
)

cfgCentralJetVetoBgEstWplusJetsEnriched = cms.PSet(
    pluginName = cms.string('centralJetVetoBgEstWplusJetsEnriched'),
    pluginType = cms.string('PATCandViewMaxEventSelector'),
    src = cms.InputTag('jetsBgEstWplusJetsEnrichedAlpha0point1'),
    maxNumber = cms.uint32(0)
)

cfgDiMuonVetoBgEstWplusJetsEnriched = cms.PSet(
    pluginName = cms.string('diMuonVetoBgEstWplusJetsEnriched'),
    pluginType = cms.string('PATCandViewMaxEventSelector'),
    src = cms.InputTag('selectedPatMuonsGlobalIndividual'),
    maxNumber = cms.uint32(1)
)

cfgVertexForMuTauBgEstWplusJetsEnriched = cms.PSet(
    pluginName = cms.string('vertexBgEstWplusJetsEnriched'),
    pluginType = cms.string('VertexMinEventSelector'),
    src = cms.InputTag('selectedPrimaryVertexForMuTauBgEstWplusJetsEnriched'),
    minNumber = cms.uint32(1)
)
cfgVertexQualityForMuTauBgEstWplusJetsEnriched = cms.PSet(
    pluginName = cms.string('vertexQualityBgEstWplusJetsEnriched'),
    pluginType = cms.string('VertexMinEventSelector'),
    src = cms.InputTag('selectedPrimaryVertexQualityForMuTauBgEstWplusJetsEnriched'),
    minNumber = cms.uint32(1)
)
cfgVertexPositionForMuTauBgEstWplusJetsEnriched = cms.PSet(
    pluginName = cms.string('vertexPositionBgEstWplusJetsEnriched'),
    pluginType = cms.string('VertexMinEventSelector'),
    src = cms.InputTag('selectedPrimaryVertexPositionForMuTauBgEstWplusJetsEnriched'),
    minNumber = cms.uint32(1)
)


evtSelConfiguratorBgEstWplusJetsEnriched = eventSelFlagProdConfigurator(
    [ cfgMuonPtCutBgEstWplusJetsEnriched,
      cfgMuonVbTfIdCutBgEstWplusJetsEnriched,      
      cfgMuonPFRelIsoCutBgEstWplusJetsEnriched,
      cfgTauTaNCdiscrCutBgEstWplusJetsEnriched,
      cfgTauMuonVetoBgEstWplusJetsEnriched,
      cfgVertexForMuTauBgEstWplusJetsEnriched,
      cfgVertexQualityForMuTauBgEstWplusJetsEnriched,
      cfgVertexPositionForMuTauBgEstWplusJetsEnriched,
      cfgMuTauPairBgEstWplusJetsEnriched,
      cfgMuTauPairMt1MEtBgEstWplusJetsEnriched,
      cfgCentralJetVetoBgEstWplusJetsEnriched,
      cfgDiMuonVetoBgEstWplusJetsEnriched ],
    boolEventSelFlagProducer = "BoolEventSelFlagProducer",
    pyModuleName = __name__
)

selectEventsBgEstWplusJetsEnriched = evtSelConfiguratorBgEstWplusJetsEnriched.configure()

#--------------------------------------------------------------------------------  
# apply event selection criteria; fill histograms
#--------------------------------------------------------------------------------

from TauAnalysis.Configuration.analyzeZtoMuTau_cfi import *
#from TauAnalysis.BgEstimationTools.selectZtoMuTauEventVertex_cff import *

muonHistManagerBgEstWplusJetsEnriched = copy.deepcopy(muonHistManager)
muonHistManagerBgEstWplusJetsEnriched.pluginName = cms.string('muonHistManagerBgEstWplusJetsEnriched')
muonHistManagerBgEstWplusJetsEnriched.muonSource = muTauPairsBgEstWplusJetsEnriched.srcLeg1

tauHistManagerBgEstWplusJetsEnriched = copy.deepcopy(tauHistManager)
tauHistManagerBgEstWplusJetsEnriched.pluginName = cms.string('tauHistManagerBgEstWplusJetsEnriched')
tauHistManagerBgEstWplusJetsEnriched.tauSource = muTauPairsBgEstWplusJetsEnriched.srcLeg2

diTauCandidateHistManagerBgEstWplusJetsEnriched = copy.deepcopy(diTauCandidateHistManagerForMuTau)
diTauCandidateHistManagerBgEstWplusJetsEnriched.pluginName = cms.string('diTauCandidateHistManagerBgEstWplusJetsEnriched')
diTauCandidateHistManagerBgEstWplusJetsEnriched.diTauCandidateSource = cms.InputTag('muTauPairsBgEstWplusJetsEnrichedMt1MET')
diTauCandidateHistManagerBgEstWplusJetsEnriched.visMassHypothesisSource = cms.InputTag('')

#diTauCandidateNSVfitHistManagerBgEstWplusJetsEnriched = copy.deepcopy(diTauCandidateNSVfitHistManagerForMuTau)
#diTauCandidateNSVfitHistManagerBgEstWplusJetsEnriched.pluginName = cms.string('diTauCandidateNSVfitHistManagerBgEstWplusJetsEnriched')
#diTauCandidateNSVfitHistManagerBgEstWplusJetsEnriched.diTauCandidateSource = diTauCandidateHistManagerBgEstWplusJetsEnriched.diTauCandidateSource

caloMEtHistManagerBgEstWplusJetsEnriched = copy.deepcopy(caloMEtHistManager)
caloMEtHistManagerBgEstWplusJetsEnriched.pluginName = cms.string('caloMEtHistManagerBgEstWplusJetsEnriched')
caloMEtHistManagerBgEstWplusJetsEnriched.leg1Source = muonHistManagerBgEstWplusJetsEnriched.muonSource
caloMEtHistManagerBgEstWplusJetsEnriched.leg2Source = tauHistManagerBgEstWplusJetsEnriched.tauSource
pfMEtHistManagerBgEstWplusJetsEnriched = copy.deepcopy(pfMEtHistManager)
pfMEtHistManagerBgEstWplusJetsEnriched.pluginName = cms.string('pfMEtHistManagerBgEstWplusJetsEnriched')
pfMEtHistManagerBgEstWplusJetsEnriched.leg1Source = muonHistManagerBgEstWplusJetsEnriched.muonSource
pfMEtHistManagerBgEstWplusJetsEnriched.leg2Source = tauHistManagerBgEstWplusJetsEnriched.tauSource

jetHistManagerBgEstWplusJetsEnriched = copy.deepcopy(jetHistManager)
jetHistManagerBgEstWplusJetsEnriched.pluginName = cms.string('jetHistManagerBgEstWplusJetsEnriched')
jetHistManagerBgEstWplusJetsEnriched.jetSource = cms.InputTag('jetsBgEstWplusJetsEnrichedAntiOverlapWithLeptonsVeto')

#from TauAnalysis.BgEstimationTools.tauIdEffZtoMuTauHistManager_cfi import *
#tauIdEffHistManagerBgEstWplusJetsEnriched = copy.deepcopy(tauIdEffZtoMuTauHistManager)
#tauIdEffHistManagerBgEstWplusJetsEnriched.pluginName = cms.string('tauIdEffHistManagerBgEstWplusJetsEnriched')
#tauIdEffHistManagerBgEstWplusJetsEnriched.muonSource = muTauPairsBgEstWplusJetsEnriched.srcLeg1
#tauIdEffHistManagerBgEstWplusJetsEnriched.tauSource = muTauPairsBgEstWplusJetsEnriched.srcLeg2
#tauIdEffHistManagerBgEstWplusJetsEnriched.diTauSource = diTauCandidateHistManagerBgEstWplusJetsEnriched.diTauCandidateSource
#tauIdEffHistManagerBgEstWplusJetsEnriched.diTauChargeSignExtractor.src = tauIdEffHistManagerBgEstWplusJetsEnriched.diTauSource

dataBinnerBgEstWplusJetsEnriched = copy.deepcopy(dataBinner)
dataBinnerBgEstWplusJetsEnriched.pluginName = cms.string('dataBinnerBgEstWplusJetsEnriched')

analyzeEventsBgEstWplusJetsEnriched = cms.EDAnalyzer("GenericAnalyzer",
  
    name = cms.string('BgEstTemplateAnalyzer_WplusJetsEnriched'), 
                            
    filters = cms.VPSet(
        evtSelGenPhaseSpace,
        evtSelTrigger,
        evtSelDataQuality,
        evtSelGlobalMuon,
        evtSelMuonEta,
        cms.PSet(
            pluginName = cms.string('muonPtCutBgEstWplusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('muonPtCutBgEstWplusJetsEnriched', 'cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('muonVbTfIdCutBgEstWplusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('muonVbTfIdCutBgEstWplusJetsEnriched', 'cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('muonPFRelIsoCutBgEstWplusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('muonPFRelIsoCutBgEstWplusJetsEnriched', 'cumulative')
        ),
        evtSelTauAntiOverlapWithMuonsVeto,
        evtSelTauEta,
        evtSelTauPt,
        evtSelTauLeadTrk,
        evtSelTauLeadTrkPt,
        cms.PSet(
            pluginName = cms.string('tauTaNCdiscrCutBgEstWplusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauTaNCdiscrCutBgEstWplusJetsEnriched', 'cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('tauMuonVetoBgEstWplusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauMuonVetoBgEstWplusJetsEnriched', 'cumulative')
        ),      
        cms.PSet(
            pluginName = cms.string('vertexBgEstWplusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('vertexBgEstWplusJetsEnriched')
        ),
        cms.PSet(
            pluginName = cms.string('vertexQualityBgEstWplusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('vertexQualityBgEstWplusJetsEnriched')
        ),
        cms.PSet(
            pluginName = cms.string('vertexPositionBgEstWplusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('vertexPositionBgEstWplusJetsEnriched')
        ),
        cms.PSet(
            pluginName = cms.string('muTauPairBgEstWplusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('muTauPairBgEstWplusJetsEnriched')
        ),
        cms.PSet(
            pluginName = cms.string('muTauPairMt1MEtBgEstWplusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('muTauPairMt1MEtBgEstWplusJetsEnriched')
        ),
        cms.PSet(
            pluginName = cms.string('centralJetVetoBgEstWplusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('centralJetVetoBgEstWplusJetsEnriched')
        ),
        cms.PSet(
            pluginName = cms.string('diMuonVetoBgEstWplusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('diMuonVetoBgEstWplusJetsEnriched')
        )
    ),
  
    analyzers = cms.VPSet(
        muonHistManagerBgEstWplusJetsEnriched,
        tauHistManagerBgEstWplusJetsEnriched,
        diTauCandidateHistManagerBgEstWplusJetsEnriched,
	#diTauCandidateNSVfitHistManagerBgEstWplusJetsEnriched,
        caloMEtHistManagerBgEstWplusJetsEnriched,
	pfMEtHistManagerBgEstWplusJetsEnriched,
        jetHistManagerBgEstWplusJetsEnriched,
        #tauIdEffHistManagerBgEstWplusJetsEnriched,
        dataBinnerBgEstWplusJetsEnriched
    ),

    eventDumps = cms.VPSet(),
   
    analysisSequence = cms.VPSet(
    
        # generator level phase-space selection
        # (NOTE: (1) to be used in case of Monte Carlo samples
        #            overlapping in simulated phase-space only !!
        #        (2) genPhaseSpaceCut needs to be **always** the first entry in the list of cuts
        #           - otherwise the script submitToBatch.csh for submission of cmsRun jobs
        #            to the CERN batch system will not work !!)
        cms.PSet(
            filter = cms.string('genPhaseSpaceCut'),
            title = cms.string('gen. Phase-Space')
        ),
        cms.PSet(
            filter = cms.string('evtSelTrigger'),
            title = cms.string('Muon Trigger')
        ),
        cms.PSet(
            filter = cms.string('evtSelDataQuality'),
            title = cms.string('Data quality')
        ),
        cms.PSet(
            filter = cms.string('evtSelGlobalMuon'),
            title = cms.string('global Muon')
        ),
        cms.PSet(
            filter = cms.string('evtSelMuonEta'),
            title = cms.string('-2.1 < eta(Muon) < +2.1')
        ),
        cms.PSet(
            filter = cms.string('muonPtCutBgEstWplusJetsEnriched'),
            title = cms.string('Pt(Muon) > 15 GeV')
        ),
        cms.PSet(
            filter = cms.string('evtSelTauAntiOverlapWithMuonsVeto'),
            title = cms.string('Tau not overlapping w. Muon'),
        ),
        cms.PSet(
            filter = cms.string('evtSelTauEta'),
            title = cms.string('-2.1 < eta(Tau) < +2.1'),
        ),
        cms.PSet(
            filter = cms.string('evtSelTauPt'),
            title = cms.string('Pt(Tau) > 20 GeV'),
        ),
        cms.PSet(
            filter = cms.string('muonVbTfIdCutBgEstWplusJetsEnriched'),
            title = cms.string('Muon VBTF id.'),
        ),
        cms.PSet(
            filter = cms.string('muonPFRelIsoCutBgEstWplusJetsEnriched'),
            title = cms.string('Muon iso.')
        ),
        cms.PSet(
            filter = cms.string('evtSelTauLeadTrk'),
            title = cms.string('Tau lead. Track find.'),
        ),
        cms.PSet(
            filter = cms.string('evtSelTauLeadTrkPt'),
            title = cms.string('Tau lead. Track Pt'),
        ),
        cms.PSet(
            filter = cms.string('tauTaNCdiscrCutBgEstWplusJetsEnriched'),
            title = cms.string('Tau TaNC discr.')
        ),
        cms.PSet(
            filter = cms.string('tauMuonVetoBgEstWplusJetsEnriched'),
            title = cms.string('Tau mu-Veto')
        ),
        cms.PSet(
            filter = cms.string('vertexBgEstWplusJetsEnriched'),
            title = cms.string('Vertex')
        ),
        cms.PSet(
            filter = cms.string('vertexQualityBgEstWplusJetsEnriched'),
            title = cms.string('Vertex quality')
        ),
        cms.PSet(
            filter = cms.string('vertexPositionBgEstWplusJetsEnriched'),
            title = cms.string('Vertex position')
        ),
        cms.PSet(
            filter = cms.string('muTauPairBgEstWplusJetsEnriched'),
            title = cms.string('dR(Muon-Tau) > 0.7')
        ),
        cms.PSet(
            analyzers = cms.vstring(
                'muonHistManagerBgEstWplusJetsEnriched',
                'tauHistManagerBgEstWplusJetsEnriched',
                'diTauCandidateHistManagerBgEstWplusJetsEnriched',
				#'diTauCandidateNSVfitHistManagerBgEstWplusJetsEnriched'
            ),
            replace = cms.vstring(
                'diTauCandidateHistManagerBgEstWplusJetsEnriched.diTauCandidateSource = muTauPairsBgEstWplusJetsEnriched',
				#'diTauCandidateNSVfitHistManagerBgEstWplusJetsEnriched.diTauCandidateSource = muTauPairsBgEstWplusJetsEnriched'
            )
        ),
        cms.PSet(
            filter = cms.string('muTauPairMt1MEtBgEstWplusJetsEnriched'),
            title = cms.string('M_{T}(Muon-MET) > 30 GeV')
        ),
        cms.PSet(
            analyzers = cms.vstring(
                'muonHistManagerBgEstWplusJetsEnriched',
                'tauHistManagerBgEstWplusJetsEnriched',
                'diTauCandidateHistManagerBgEstWplusJetsEnriched',
				#'diTauCandidateNSVfitHistManagerBgEstWplusJetsEnriched'
            )
        ),
        cms.PSet(
            filter = cms.string('centralJetVetoBgEstWplusJetsEnriched'),
            title = cms.string('central Jet Veto')
        ),
        cms.PSet(
            analyzers = cms.vstring(
                'muonHistManagerBgEstWplusJetsEnriched',
                'tauHistManagerBgEstWplusJetsEnriched',
                'diTauCandidateHistManagerBgEstWplusJetsEnriched'
            )
        ),
        cms.PSet(
            filter = cms.string('diMuonVetoBgEstWplusJetsEnriched'),
            title = cms.string('di-Muon Veto')
        ),
        cms.PSet(
            analyzers = cms.vstring(
                'muonHistManagerBgEstWplusJetsEnriched',
                'tauHistManagerBgEstWplusJetsEnriched',
                'diTauCandidateHistManagerBgEstWplusJetsEnriched',
		#'diTauCandidateNSVfitHistManagerBgEstWplusJetsEnriched',
                'pfMEtHistManagerBgEstWplusJetsEnriched',
                'caloMEtHistManagerBgEstWplusJetsEnriched',
                'jetHistManagerBgEstWplusJetsEnriched',
                #'tauIdEffHistManagerBgEstWplusJetsEnriched',
                'dataBinnerBgEstWplusJetsEnriched'
            )
        )
    )
)

analysisSequenceBgEstWplusJetsEnriched = cms.Sequence(analyzeEventsBgEstWplusJetsEnriched)

#--------------------------------------------------------------------------------  
# define (final) analysis sequence
#--------------------------------------------------------------------------------

bgEstWplusJetsEnrichedAnalysisSequence = cms.Sequence(
    selectMuonsBgEstWplusJetsEnriched
   + selectTausBgEstWplusJetsEnriched
   + selectPrimaryVertexForMuTauBgEstWplusJetsEnriched
   + produceMuTauPairsBgEstWplusJetsEnriched + selectMuTauPairsBgEstWplusJetsEnriched
   + selectJetsBgEstWplusJetsEnriched 
   + selectEventsBgEstWplusJetsEnriched
   + analysisSequenceBgEstWplusJetsEnriched
)
