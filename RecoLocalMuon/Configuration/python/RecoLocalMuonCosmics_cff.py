import FWCore.ParameterSet.Config as cms

# Magnetic Field
# Geometry
# DT Geometry
# CSC Geometry
# RPC Geometry
#------------------------------------ DT ------------------------------------------------
# 1D RecHits
#from RecoLocalMuon.DTRecHit.dt1DRecHits_LinearDrift_CosmicData_cfi import *
from RecoLocalMuon.DTRecHit.dt1DRecHits_LinearDriftFromDB_CosmicData_cfi import *
# 2D Segments
#from RecoLocalMuon.DTSegment.dt2DSegments_CombPatternReco2D_LinearDrift_CosmicData_cfi import *
from RecoLocalMuon.DTSegment.dt2DSegments_CombPatternReco2D_LinearDriftFromDB_CosmicData_cfi import *
# 4D Segments
<<<<<<< RecoLocalMuonCosmics_cff.py
#from RecoLocalMuon.DTSegment.dt4DSegments_CombPatternReco4D_LinearDrift_CosmicData_cfi import *
from RecoLocalMuon.DTSegment.dt4DSegments_CombPatternReco4D_LinearDriftFromDB_CosmicData_cfi import *
=======
#from RecoLocalMuon.DTSegment.dt4DSegments_CombPatternReco4D_LinearDrift_CosmicData_cfi import *
from RecoLocalMuon.DTSegment.dt4DSegments_CombPatternReco4D_LinearDriftFromDB_CosmicData_cfi import *

# No drift algo
>>>>>>> 1.6
import RecoLocalMuon.DTRecHit.dt1DRecHits_NoDrift_CosmicData_cfi
dt1DRecHitsNoDrift = RecoLocalMuon.DTRecHit.dt1DRecHits_NoDrift_CosmicData_cfi.dt1DRecHits.clone()
import RecoLocalMuon.DTSegment.dt4DSegments_CombPatternReco4D_NoDrift_CosmicData_cfi
dt4DSegmentsNoDrift = RecoLocalMuon.DTSegment.dt4DSegments_CombPatternReco4D_NoDrift_CosmicData_cfi.dt4DSegments.clone()
#------------------------------------ CSC -----------------------------------------------
# 2D RecHit	
from RecoLocalMuon.CSCRecHitD.cscRecHitD_cfi import *
# Segments
from RecoLocalMuon.CSCSegment.cscSegments_cfi import *
#------------------------------------ RPC -----------------------------------------------
# 1D RecHits
from RecoLocalMuon.RPCRecHit.rpcRecHits_cfi import *
dtlocalrecoNoDrift = cms.Sequence(dt1DRecHitsNoDrift*dt4DSegmentsNoDrift)
#----------------------------------------------------------------------------------------
# DT sequence for the standard reconstruction chain 
# The reconstruction of the 2D segments are not required for the 4D segments reconstruction, they are used
# only for debuging purpose and for specific studies
dtlocalreco = cms.Sequence(dt1DRecHits*dt4DSegments)
# DT sequence with the 2D segment reconstruction
dtlocalreco_with_2DSegments = cms.Sequence(dt1DRecHits*dt2DSegments*dt4DSegments)
# CSC sequence
csclocalreco = cms.Sequence(csc2DRecHits*cscSegments)
# DT, CSC and RPC together
muonlocalreco_with_2DSegments = cms.Sequence(dtlocalreco_with_2DSegments+csclocalreco+rpcRecHits)
# DT, CSC and RPC together (correct sequence for the standard path)
muonlocalreco = cms.Sequence(dtlocalreco+csclocalreco+rpcRecHits)
# DT, CSC and RPC together (correct sequence for the standard path)
muonlocalrecoNoDrift = cms.Sequence(dtlocalrecoNoDrift+csclocalreco+rpcRecHits)
muonLocalRecoGR = cms.Sequence(muonlocalreco+muonlocalrecoNoDrift)
#DTLinearDriftAlgo_CosmicData.recAlgoConfig.hitResolution = 0.05
DTLinearDriftFromDBAlgo_CosmicData.recAlgoConfig.tTrigModeConfig.kFactor = -1.00
#dt1DRecHits.dtDigiLabel = 'muonDTDigis'
DTCombinatorialPatternReco2DAlgo_LinearDriftFromDB_CosmicData.Reco2DAlgoConfig.segmCleanerMode = 2
DTCombinatorialPatternReco2DAlgo_LinearDriftFromDB_CosmicData.Reco2DAlgoConfig.MaxAllowedHits = 30
DTCombinatorialPatternReco4DAlgo_LinearDriftFromDB_CosmicData.Reco4DAlgoConfig.segmCleanerMode = 2
#dt1DRecHitsNoDrift.dtDigiLabel = 'muonDTDigis'
