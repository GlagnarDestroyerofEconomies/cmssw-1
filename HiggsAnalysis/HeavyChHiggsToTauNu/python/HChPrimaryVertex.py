import FWCore.ParameterSet.Config as cms


def addPrimaryVertexSelection(process, sequence):
    process.firstPrimaryVertex = cms.EDProducer("HPlusFirstVertexSelector",
        src = cms.InputTag("offlinePrimaryVertices")
    )
    sequence *= process.firstPrimaryVertex

    import HiggsAnalysis.HeavyChHiggsToTauNu.HChPrimaryVertex_cfi as pv
    process.selectedPrimaryVertex = pv.goodPrimaryVertices.clone(
        src = cms.InputTag("firstPrimaryVertex")
    )
    sequence *= process.selectedPrimaryVertex

    process.goodPrimaryVertices = pv.goodPrimaryVertices.clone()
    sequence *= process.goodPrimaryVertices

#    process.selectedPrimaryVertexFilter = cms.EDFilter("VertexCountFilter",
#                                                       src = cms.InputTag("selectedPrimaryVertex"),
#                                                       minNumber = cms.uint32(1),
#                                                       maxNumber = cms.uint32(999)
#                                                       )
#    sequence *= process.selectedPrimaryVertexFilter
    
