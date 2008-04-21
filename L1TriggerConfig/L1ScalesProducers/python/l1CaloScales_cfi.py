import FWCore.ParameterSet.Config as cms

l1CaloScales = cms.ESProducer("L1ScalesTrivialProducer",
    L1CaloEmEtScaleLSB = cms.double(0.5),
    L1CaloEmThresholds = cms.vdouble(0.0, 1.0, 2.0, 3.0, 4.0, 
        5.0, 6.0, 7.0, 8.0, 9.0, 
        10.0, 11.0, 12.0, 13.0, 14.0, 
        15.0, 16.0, 17.0, 18.0, 19.0, 
        20.0, 21.0, 22.0, 23.0, 24.0, 
        25.0, 26.0, 27.0, 28.0, 29.0, 
        30.0, 31.0, 32.0, 33.0, 34.0, 
        35.0, 36.0, 37.0, 38.0, 39.0, 
        40.0, 41.0, 42.0, 43.0, 44.0, 
        45.0, 46.0, 47.0, 48.0, 49.0, 
        50.0, 51.0, 52.0, 53.0, 54.0, 
        55.0, 56.0, 57.0, 58.0, 59.0, 
        60.0, 61.0, 62.0, 63.0),
    L1CaloJetThresholds = cms.vdouble(0.0, 10.0, 12.0, 14.0, 15.0, 
        18.0, 20.0, 22.0, 24.0, 25.0, 
        28.0, 30.0, 32.0, 35.0, 37.0, 
        40.0, 45.0, 50.0, 55.0, 60.0, 
        65.0, 70.0, 75.0, 80.0, 85.0, 
        90.0, 100.0, 110.0, 120.0, 125.0, 
        130.0, 140.0, 150.0, 160.0, 170.0, 
        175.0, 180.0, 190.0, 200.0, 215.0, 
        225.0, 235.0, 250.0, 275.0, 300.0, 
        325.0, 350.0, 375.0, 400.0, 425.0, 
        450.0, 475.0, 500.0, 525.0, 550.0, 
        575.0, 600.0, 625.0, 650.0, 675.0, 
        700.0, 725.0, 750.0, 775.0),
    L1CaloRegionEtScaleLSB = cms.double(0.5)
)


