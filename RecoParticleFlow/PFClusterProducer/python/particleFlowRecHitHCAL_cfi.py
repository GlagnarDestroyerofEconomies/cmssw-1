import FWCore.ParameterSet.Config as cms

particleFlowRecHitHCAL = cms.EDProducer("PFRecHitProducerHCAL",
    # verbosity 
    verbose = cms.untracked.bool(False),
    caloTowers = cms.InputTag("towerMakerPF"),
    hcalRecHitsHBHE = cms.InputTag(""),
    # cell threshold in barrel 
    thresh_Barrel = cms.double(0.4),
    # cell threshold in HF
    thresh_HF = cms.double(0.4),
    # cell threshold in endcap 
    thresh_Endcap = cms.double(0.4),
    # Navigation in HF: 
    # False = no real clustering in HF
    # True  = do clustering in HF
    navigation_HF = cms.bool(True),
#AUGUSTE: TO BE CHECKED:
    weight_HFem = cms.double(1.000),
    weight_HFhad = cms.double(1.000),
#   weight_HFem = cms.double(1.0),
#   weight_HFhad = cms.double(1.0)

# HCAL DPG RecHit calibration
    HCAL_Calib = cms.bool(True),
    HF_Calib = cms.bool(False),

# Cut short fibres if no long fibre energy
    ShortFibre_Cut = cms.double(60.),
    LongFibre_Fraction = cms.double(0.10),

# Compensate for ECAL dead channels                                        
    ECAL_Compensate = cms.bool(False),
    ECAL_Threshold = cms.double(10.),
    ECAL_Compensation = cms.double(0.5),
    ECAL_Dead_Code = cms.uint32(10),

# Depth correction (in cm) for hadronic and electromagnetic rechits
    EM_Depth = cms.double(22.),
    HAD_Depth = cms.double(47.)                              
                                  
)


