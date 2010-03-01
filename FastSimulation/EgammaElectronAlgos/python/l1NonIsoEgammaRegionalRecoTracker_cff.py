import FWCore.ParameterSet.Config as cms

# Take all pixel tracks but the potential electrons
hltL1NonIsoEgammaRegionalCTFFinalFitWithMaterial = cms.EDProducer("FastTrackMerger",
    SaveTracksOnly = cms.untracked.bool(True),
    TrackProducers = cms.VInputTag(cms.InputTag("globalPixelWithMaterialTracksForPhotons"),
                                   cms.InputTag("globalPixelTrackCandidatesForPhotons"))
)

# The sequence
HLTL1NonIsoEgammaRegionalRecoTrackerSequence = cms.Sequence(cms.SequencePlaceholder("globalPixelTracking")+
                                                            hltL1NonIsoEgammaRegionalCTFFinalFitWithMaterial)

