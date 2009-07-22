import FWCore.ParameterSet.Config as cms

PhotonIDProd = cms.EDProducer("PhotonIDProducer",
    #required inputs
    #What collection of photons do I run on?
    photonProducer = cms.string('photons'),                              
    photonLabel = cms.string(''),
    #What labels do I use for my products?
    photonCutBasedIDLooseLabel = cms.string('PhotonCutBasedIDLoose'),
    photonCutBasedIDTightLabel = cms.string('PhotonCutBasedIDTight'),
    #What rechit collection do I use for ECAL iso?                          
    doCutBased = cms.bool(True),
    #switches, turn on quality cuts for various quantities.
    RequireFiducial = cms.bool(False),
    DoHollowConeTrackIsolationCut = cms.bool(True),
    DoSolidConeTrackIsolationCut = cms.bool(False),
    DoHollowConeNTrkCut = cms.bool(False),
    DoSolidConeNTrkCut = cms.bool(False),
    DoHadOverEMCut = cms.bool(True),
    DoEtaWidthCut = cms.bool(True),
    DoHcalTowerIsolationCut = cms.bool(True),
    DoEcalRecHitIsolationCut = cms.bool(True),
    DoEcalIsoRelativeCut = cms.bool(True),
    DoR9Cut = cms.bool(False),                               
    #LoosePhoton cuts EB
    LoosePhotonEcalIsoRelativeCutSlopeEB = cms.double(0.004),
    LoosePhotonEcalIsoRelativeCutOffsetEB= cms.double(2.5),
    LoosePhotonHcalTowerIsoSlopeEB= cms.double(0.),
    LoosePhotonHcalTowerIsoOffsetEB=cms.double(3.),
    LoosePhotonHollowTrkSlopeEB=cms.double(0.),
    LoosePhotonHollowTrkOffsetEB=cms.double(9.),
    LoosePhotonSolidTrkSlopeEB=cms.double(0.),
    LoosePhotonSolidTrkOffsetEB=cms.double(999.),
    LoosePhotonSolidTrkEB=cms.double(999.),
    LoosePhotonSolidNTrkEB=cms.int32(999),
    LoosePhotonHollowNTrkEB=cms.int32(999),
    LoosePhotonEtaWidthEB=cms.double(999.),
    LoosePhotonHadOverEMEB=cms.double(0.15),
    LoosePhotonR9CutEB=cms.double(0.0),
    #TightPhoton cuts EB
    TightPhotonEcalIsoRelativeCutSlopeEB = cms.double(0.004),
    TightPhotonEcalIsoRelativeCutOffsetEB= cms.double(2.5),
    TightPhotonHcalTowerIsoSlopeEB= cms.double(0.),
    TightPhotonHcalTowerIsoOffsetEB=cms.double(3.),
    TightPhotonHollowTrkSlopeEB=cms.double(0.),
    TightPhotonHollowTrkOffsetEB=cms.double(9.),
    TightPhotonSolidTrkSlopeEB=cms.double(0.),
    TightPhotonSolidTrkOffsetEB=cms.double(999.),
    TightPhotonSolidTrkEB=cms.double(999.),
    TightPhotonSolidNTrkEB=cms.int32(999),
    TightPhotonHollowNTrkEB=cms.int32(999),
    TightPhotonEtaWidthEB=cms.double(0.013),
    TightPhotonHadOverEMEB=cms.double(0.15),
    TightPhotonR9CutEB=cms.double(0.0),
    #LoosePhoton cuts EE
    LoosePhotonEcalIsoRelativeCutSlopeEE = cms.double(0.0021),
    LoosePhotonEcalIsoRelativeCutOffsetEE= cms.double(3.),
    LoosePhotonHcalTowerIsoSlopeEE= cms.double(0.),
    LoosePhotonHcalTowerIsoOffsetEE=cms.double(3.5),
    LoosePhotonHollowTrkSlopeEE=cms.double(0.),
    LoosePhotonHollowTrkOffsetEE=cms.double(9.),
    LoosePhotonSolidTrkSlopeEE=cms.double(0.),
    LoosePhotonSolidTrkOffsetEE=cms.double(999.),
    LoosePhotonSolidTrkEE=cms.double(999.),
    LoosePhotonSolidNTrkEE=cms.int32(999),
    LoosePhotonHollowNTrkEE=cms.int32(999),
    LoosePhotonEtaWidthEE=cms.double(999.),
    LoosePhotonHadOverEMEE=cms.double(0.15),
    LoosePhotonR9CutEE=cms.double(0.0),
    #TightPhoton cuts EE
    TightPhotonEcalIsoRelativeCutSlopeEE = cms.double(0.0021),
    TightPhotonEcalIsoRelativeCutOffsetEE= cms.double(5.),
    TightPhotonHcalTowerIsoSlopeEE= cms.double(0.),
    TightPhotonHcalTowerIsoOffsetEE=cms.double(5.),
    TightPhotonHollowTrkSlopeEE=cms.double(0.),
    TightPhotonHollowTrkOffsetEE=cms.double(9.),
    TightPhotonSolidTrkSlopeEE=cms.double(0.),
    TightPhotonSolidTrkOffsetEE=cms.double(999.),
    TightPhotonSolidTrkEE=cms.double(999.),
    TightPhotonSolidNTrkEE=cms.int32(999),
    TightPhotonHollowNTrkEE=cms.int32(999),
    TightPhotonEtaWidthEE=cms.double(999),
    TightPhotonHadOverEMEE=cms.double(0.15),
    TightPhotonR9CutEE=cms.double(0.0)
)


