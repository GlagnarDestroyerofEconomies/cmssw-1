import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.CandidateTools.tools.objProdConfigurator import *
from TauAnalysis.CandidateTools.resolutions_cfi import *
from TauAnalysis.CandidateTools.svFitAlgorithm_cfi import *
from TauAnalysis.CandidateTools.nSVfitAlgorithmDiTau_cfi import *

svFitLikelihoodMuTauPairKinematicsPhaseSpace = copy.deepcopy(svFitLikelihoodDiTauKinematicsPhaseSpace)
svFitLikelihoodMuTauPairKinematicsPhaseSpace.pluginType = "SVfitLikelihoodMuTauPairKinematics"
svFitLikelihoodMuTauPairKinematicsPhaseSpace.leg1.pluginType = "SVfitMuonLikelihoodPhaseSpace"
svFitLikelihoodMuTauPairKinematicsPhaseSpace.leg2.pluginType = "SVfitTauLikelihoodPhaseSpace"

svFitLikelihoodMuTauPairKinematicsPolarized = copy.deepcopy(svFitLikelihoodDiTauKinematicsPolarized)
svFitLikelihoodMuTauPairKinematicsPolarized.pluginType = "SVfitLikelihoodMuTauPairKinematics"
svFitLikelihoodMuTauPairKinematicsPolarized.leg1.pluginType = "SVfitMuonLikelihoodPolarization"
svFitLikelihoodMuTauPairKinematicsPolarized.leg2.pluginType = "SVfitTauLikelihoodPolarization"

svFitLikelihoodMuTauPairMEt = copy.deepcopy(svFitLikelihoodDiTauMEt)
svFitLikelihoodMuTauPairMEt.pluginType = cms.string("SVfitLikelihoodMuTauPairMEt")

svFitLikelihoodMuTauPairTrackInfo = copy.deepcopy(svFitLikelihoodTrackInfo)
svFitLikelihoodMuTauPairTrackInfo.pluginType = "SVfitLikelihoodMuTauPairTrackInfo"
svFitLikelihoodMuTauPairTrackInfo.leg1.pluginType = "SVfitMuonLikelihoodTrackInfo"
svFitLikelihoodMuTauPairTrackInfo.leg2.pluginType = "SVfitTauLikelihoodTrackInfo"

svFitLikelihoodMuTauPairPtBalance = copy.deepcopy(svFitLikelihoodDiTauPtBalance)
svFitLikelihoodMuTauPairPtBalance.pluginType = cms.string("SVfitLikelihoodMuTauPairPtBalance")

svFitLikelihoodMuTauPairPtBalance2 = copy.deepcopy(svFitLikelihoodDiTauPtBalance2)
svFitLikelihoodMuTauPairPtBalance2.pluginType = cms.string("SVfitLikelihoodMuTauPairPtBalance2")

svFitLikelihoodMuTauPairZprod = copy.deepcopy(svFitLikelihoodDiTauProdZ0)
svFitLikelihoodMuTauPairZprod.pluginType = cms.string("SVfitLikelihoodMuTauPairProd")
svFitLikelihoodMuTauPairZprod.process = cms.string("Z0")

#--------------------------------------------------------------------------------
# produce combinations of muon + tau-jet pairs
#--------------------------------------------------------------------------------

svfit_with_track = cms.PSet(
    likelihoodFunctions = cms.VPSet(
        svFitLikelihoodMuTauPairKinematicsPhaseSpace,
        svFitLikelihoodMuTauPairMEt,
        svFitLikelihoodMuTauPairTrackInfo,
        svFitLikelihoodMuTauPairPtBalance
    ),
    parameterizeVertexAlongTrackLeg1 = cms.bool(True),
    parameterizeVertexAlongTrackLeg2 = cms.bool(True),
    estUncertainties = cms.PSet(
        #numSamplings = cms.int32(1000)
        numSamplings = cms.int32(-1)
    )
)

allMuTauPairs = cms.EDProducer("PATMuTauPairProducer",
    useLeadingTausOnly = cms.bool(False),
    srcLeg1 = cms.InputTag('selectedPatMuonsTrkIPcumulative'),
    srcLeg2 = cms.InputTag('selectedPatTausForMuTauElectronVetoCumulative'),
    dRmin12 = cms.double(0.3),
    srcMET = cms.InputTag('patMETs'),
    srcPrimaryVertex = cms.InputTag("offlinePrimaryVerticesWithBS"),
    srcBeamSpot = cms.InputTag("offlineBeamSpot"),
    srcGenParticles = cms.InputTag('genParticles'),
    recoMode = cms.string(""),
    doSVreco = cms.bool(True),
    svFit = cms.PSet(
        psKine = cms.PSet(
            likelihoodFunctions = cms.VPSet(
                svFitLikelihoodMuTauPairKinematicsPhaseSpace
            ),
            estUncertainties = cms.PSet(
                numSamplings = cms.int32(-1)
            )
        ),
        psKine_MEt = cms.PSet(
            likelihoodFunctions = cms.VPSet(
                svFitLikelihoodMuTauPairKinematicsPhaseSpace,
                svFitLikelihoodMuTauPairMEt
            ),
            estUncertainties = cms.PSet(
                numSamplings = cms.int32(-1)
            )
        ),
        psKine_MEt_ptBalance = cms.PSet(
            likelihoodFunctions = cms.VPSet(
                svFitLikelihoodMuTauPairKinematicsPhaseSpace,
                svFitLikelihoodMuTauPairMEt,
                svFitLikelihoodMuTauPairPtBalance
            ),
            estUncertainties = cms.PSet(
                #numSamplings = cms.int32(1000)
                numSamplings = cms.int32(-1)
            )
        ),
        psKine_MEt_ptBalance2 = cms.PSet(
            likelihoodFunctions = cms.VPSet(
                svFitLikelihoodMuTauPairKinematicsPhaseSpace,
                svFitLikelihoodMuTauPairMEt,
                svFitLikelihoodMuTauPairPtBalance2
            ),
            estUncertainties = cms.PSet(
                #numSamplings = cms.int32(1000)
                numSamplings = cms.int32(-1)
            )
        ),
        ##psKine_MEt_Track_ptBalance = cms.PSet(
        ##    likelihoodFunctions = cms.VPSet(
        ##        svFitLikelihoodMuTauPairKinematicsPhaseSpace,
        ##        svFitLikelihoodMuTauPairMEt,
        ##        svFitLikelihoodMuTauPairTrackInfo,
        ##        svFitLikelihoodMuTauPairPtBalance
        ##    ),
        ##    parameterizeVertexAlongTrack = cms.bool(True),
        ##    estUncertainties = cms.PSet(
        ##        #numSamplings = cms.int32(1000)
        ##        numSamplings = cms.int32(-1)
        ##    )
    ),
    nSVfit = cms.PSet(
        psKine_MEt_pow10ptBalance_int = cms.PSet(
            config    =  nSVfitConfig,
            algorithm = cms.PSet(
                pluginName = cms.string("nSVfitAlgorithmByIntegration"),
                pluginType = cms.string("NSVfitAlgorithmByIntegration"),                                    
                parameters = cms.PSet(
                    mass_A = cms.PSet(
                        #min = cms.double(20.),
                        min = cms.double(60.),                            
                        max = cms.double(200.),
                        stepSize = cms.double(5.),                                                            
                        replace = cms.string("leg1.x"),
                        by = cms.string("(A.p4.mass/mass_A)*(A.p4.mass/mass_A)/leg2.x")
                    )
                ),
                vegasOptions = cms.PSet(
                    numCalls = cms.uint32(10000)                             
                )
            )
        )
    ),                           
    scaleFuncImprovedCollinearApprox = cms.string('1'),
    verbosity = cms.untracked.int32(0)
)

allMuTauPairs.nSVfit.psKine_MEt_pow05ptBalance_int = copy.deepcopy(allMuTauPairs.nSVfit.psKine_MEt_pow10ptBalance_int)
allMuTauPairs.nSVfit.psKine_MEt_pow05ptBalance_int.config.event.resonances.A.likelihoodFunctions = cms.VPSet(
    nSVfitResonanceLikelihoodPtBalance.clone(
        power = cms.double(0.5)                         
    )
)
allMuTauPairs.nSVfit.psKine_MEt_pow15ptBalance_int = copy.deepcopy(allMuTauPairs.nSVfit.psKine_MEt_pow10ptBalance_int)
allMuTauPairs.nSVfit.psKine_MEt_pow15ptBalance_int.config.event.resonances.A.likelihoodFunctions = cms.VPSet(
    nSVfitResonanceLikelihoodPtBalance.clone(
        power = cms.double(1.5)                         
    )
)
allMuTauPairs.nSVfit.psKine_MEt_pow20ptBalance_int = copy.deepcopy(allMuTauPairs.nSVfit.psKine_MEt_pow10ptBalance_int)
allMuTauPairs.nSVfit.psKine_MEt_pow20ptBalance_int.config.event.resonances.A.likelihoodFunctions = cms.VPSet(
    nSVfitResonanceLikelihoodPtBalance.clone(
        power = cms.double(2.0)                         
    )
)

allMuTauPairs.nSVfit.psKine_MEt_pow10ptBalance_fit = copy.deepcopy(allMuTauPairs.nSVfit.psKine_MEt_pow10ptBalance_int)
allMuTauPairs.nSVfit.psKine_MEt_pow10ptBalance_fit.algorithm = cms.PSet(
    pluginName = cms.string("nSVfitAlgorithmByLikelihoodMaximization"),
    pluginType = cms.string("NSVfitAlgorithmByLikelihoodMaximization"),                                    
    minimizer  = cms.vstring("Minuit2", "Migrad"),
    #minimizer  = cms.vstring("GSLMultiMin", "conjugatefr"),
    #minimizer  = cms.vstring("GSLMultiMin", "SteepestDescent"),
    maxObjFunctionCalls_ = cms.uint32(5000),  
    verbosity = cms.int32(1)
)

##allMuTauPairs.nSVfit.psKine_MEt_Track_ptBalance = copy.deepcopy(allMuTauPairs.nSVfit.psKine_MEt_ptBalance)
##allMuTauPairs.nSVfit.psKine_MEt_Track_ptBalance.config.event.resonances.A.daughters.leg1.likelihoodFunctions = cms.VPSet(
##    nSVfitMuonLikelihoodPhaseSpace,
##    nSVfitMuonLikelihoodTrackInfo
##)
##allMuTauPairs.nSVfit.psKine_MEt_Track_ptBalance.config.event.resonances.A.daughters.leg2.likelihoodFunctions = cms.VPSet(
##    nSVfitTauLikelihoodPhaseSpace,
##    nSVfitTauLikelihoodTrackInfo
##)

muTauPairProdConfigurator = objProdConfigurator(
    allMuTauPairs,
    pyModuleName = __name__
)

produceMuTauPairs = muTauPairProdConfigurator.configure(pyNameSpace = locals())

# define additional collections of muon + tau-jet candidates
# with loose track and ECAL isolation applied on muon leg
# (NOTE: to be used for the purpose of factorizing efficiencies
#        of muon isolation from other event selection criteria,
#        in order to avoid problems with limited Monte Carlo statistics)

allMuTauPairsLooseMuonIsolation = allMuTauPairs.clone(
    srcLeg1 = cms.InputTag('selectedPatMuonsTrkIPlooseIsolationCumulative'),
)

muTauPairProdConfiguratorLooseMuonIsolation = objProdConfigurator(
    allMuTauPairsLooseMuonIsolation,
    pyModuleName = __name__
)

produceMuTauPairsLooseMuonIsolation = muTauPairProdConfiguratorLooseMuonIsolation.configure(pyNameSpace = locals())

produceMuTauPairsAll = cms.Sequence(produceMuTauPairs * produceMuTauPairsLooseMuonIsolation)
