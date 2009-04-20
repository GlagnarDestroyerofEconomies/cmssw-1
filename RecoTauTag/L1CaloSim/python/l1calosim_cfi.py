import FWCore.ParameterSet.Config as cms

l1CaloSim = cms.EDFilter("L1CaloSim",
    TowerEEScale = cms.double(1.0),
    HcalLUT = cms.FileInPath('CalibCalorimetry/CaloTPG/data/TPGcalcDecompress2.txt'),
    TowerEMLSB = cms.double(0.5),
    JetSeedEtThreshold = cms.double(0.01),
    JetLSB = cms.double(0.5),
    DoJetCorr = cms.bool(True),
    EMSeedEnThreshold = cms.double(0.01),
    CrystalEEThreshold = cms.double(0.0),
    TowerHadLSB = cms.double(0.5),
    FGEEThreshold = cms.double(0.5),
    HcalTPInput = cms.InputTag("hcalTriggerPrimitiveDigis"),
    TowerEBScale = cms.double(1.0),
    EMActiveLevel = cms.double(3.0),
    HadActiveLevel = cms.double(3.0),
    EMActiveLevelIso = cms.double(2.0),
    HadActiveLevelIso = cms.double(2.0),
    IsolationEt = cms.double(4.0),
    TowerHBScale = cms.double(1.0),
    TowerHBThreshold = cms.double(0.0),
    GctIso = cms.bool(False),
    hOeThreshold = cms.double(0.05),
    noFGThreshold = cms.double(50.0),
    TowerInput = cms.InputTag("towerMaker"),
    CrystalEBThreshold = cms.double(0.0),
    EmInputs = cms.VInputTag(cms.InputTag("ecalRecHit","EcalRecHitsEB"), cms.InputTag("ecalRecHit","EcalRecHitsEE")),
    TowerHEScale = cms.double(1.0),
    AlgorithmSource = cms.string('TrigPrims'),
    EMLSB = cms.double(0.5),
    TowerHEThreshold = cms.double(0.0),
    TowerEEThreshold = cms.double(0.0),
    MuonNoiseLevel = cms.double(2.0),
    noTauVetoLevel = cms.double(10000.0),
    EMNoiseLevel = cms.double(2.0),
    DoEMCorr = cms.bool(True),
    FGEBThreshold = cms.double(0.8),
    QuietRegionThreshold = cms.double(3.0),
    EcalTPInput = cms.InputTag("ecalTriggerPrimitiveDigis"),
    TowerEBThreshold = cms.double(0.0),
    DoBitInfo = cms.bool(False),
    HadNoiseLevel = cms.double(2.0)
)



