import FWCore.ParameterSet.Config as cms
import copy

# import config for event selection, event print-out and analysis sequence
from TauAnalysis.Configuration.analyzeAHtoElecTau_cfi import *

# define auxiliary service
# for handling of systematic uncertainties
from TauAnalysis.CandidateTools.sysErrDefinitions_cfi import *
SysUncertaintyService = cms.Service("SysUncertaintyService",
    weights = getSysUncertaintyParameterSets(
		[ theorySystematics ]
    ),
	sources = cms.PSet(
		isRecAHtoElecTau = cms.vstring(
			"sysElectron*", "",
            "sysTau*", "",
			"sysZllRecoilCorrection*", "",
			"sysJet*", ""
		)
	)
)

analyzeAHtoElecTauEvents = cms.EDAnalyzer("GenericAnalyzer",
  
    name = cms.string('ahElecTauAnalyzer'), 
                            
    filters = cms.VPSet(
        # generator level phase-space selection
        evtSelGenPhaseSpace,
    
        # generator level selection of Z --> e + tau-jet events
        # passing basic acceptance and kinematic cuts
        # (NOTE: to be used for efficiency studies only !!)
		#genElectronCut,
        #genTauCut,
    
        # trigger selection
        evtSelTrigger,
        
        # primary event vertex selection
        evtSelPrimaryEventVertex,
        evtSelPrimaryEventVertexQuality,
        evtSelPrimaryEventVertexPosition,
        
        # electron candidate selection
        evtSelElectronId,
        evtSelElectronAntiCrack,
        evtSelElectronEta,
        evtSelElectronPt,
		#evtSelElectronTrkIso,
		#evtSelElectronEcalIso,
        evtSelElectronIso,
        evtSelElectronConversionVeto,
        evtSelElectronTrkIP,

        # tau candidate selection
        evtSelTauAntiOverlapWithElectronsVeto,
        evtSelTauEta,
        evtSelTauPt,
        evtSelTauLeadTrk,
        evtSelTauLeadTrkPt,
		evtSelTauTaNCdiscr,
        evtSelTauTrkIso,
        evtSelTauEcalIso,
        evtSelTauProng,
        evtSelTauCharge,
        evtSelTauElectronVeto,
        evtSelTauEcalCrackVeto,
        evtSelTauMuonVeto,
        
        # di-tau candidate selection
        evtSelDiTauCandidateForElecTauAntiOverlapVeto,
        evtSelDiTauCandidateForElecTauZeroCharge,
        evtSelDiTauCandidateForElecTauAcoplanarity12,
        evtSelDiTauCandidateForElecTauMt1MET,
        evtSelDiTauCandidateForElecTauPzetaDiff,

        # veto events compatible with Z --> e+ e- hypothesis
        # (based on reconstructed (visible) invariant mass of e + tau-jet pair)
		#evtSelElecTauPairZeeHypothesisVeto

		# veto events compatible with Z --> e+ e- hypothesis
		# (based on the precense of an opposite-sign, loosely isolated electron	 
		evtSelDiElecPairZeeHypothesisVetoByLooseIsolation
    ),
  
    analyzers = cms.VPSet(
        genPhaseSpaceEventInfoHistManager,
        eventWeightHistManager,
        electronHistManager,
        tauHistManager,
        diTauCandidateHistManagerForElecTau,
        diTauCandidateSVfitHistManagerForElecTau,                                     
        diTauCandidateZeeHypothesisHistManagerForElecTau,
        jetHistManager,
        caloMEtHistManager,
        pfMEtHistManager,
        particleMultiplicityHistManager,
        vertexHistManager,
        triggerHistManagerForElecTau
    ),

    analyzers_systematic = cms.VPSet(
		sysUncertaintyHistManagerForElecTau
	),                                     

    eventDumps = cms.VPSet(
        elecTauEventDump
    ),
   
    analysisSequence = elecTauAnalysisSequence,

    estimateSysUncertainties = cms.bool(False), 
    systematics = cms.vstring(
        getSysUncertaintyNames(
            [ electronSystematics,
              tauSystematics,
			  jetSystematics,
			  elecTauPairSystematics,
              theorySystematics ]
        )
    )                                         
)

