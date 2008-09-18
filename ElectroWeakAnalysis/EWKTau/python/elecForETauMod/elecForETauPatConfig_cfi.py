# The following comments couldn't be translated into the new config version:

# Minimum Tk Pt
# 
# Endcaps
# 
import FWCore.ParameterSet.Config as cms


allLayer0ElecForETau = cms.EDFilter("PATElectronCleaner",
     ## reco electron input source
     electronSource = cms.InputTag("pixelMatchGsfElectrons"), 
 
     # remove or flag duplicate (same track or same supercluster seed)
     removeDuplicates = cms.bool(True),
 
     # selection (e.g. ID)
     selection = cms.PSet(
         type = cms.string('none')
     )
)

allLayer1ElecForETau = cms.EDProducer("PATElectronProducer",
    # General configurables
    electronSource = cms.InputTag("allLayer0ElecForETau"),

    # Embedding of AOD items
    embedTrack        = cms.bool(False), ## whether to embed in AOD externally stored track (note: gsf electrons don't have a track)
    embedGsfTrack     = cms.bool(False), ## whether to embed in AOD externally stored gsf track
    embedSuperCluster = cms.bool(False), ## whether to embed in AOD externally stored supercluster

    # resolution configurables
    addResolutions   = cms.bool(True),
    electronResoFile = cms.string('PhysicsTools/PatUtils/data/Resolutions_electron.root'), # input root file for the resolution functions
    useNNResolutions = cms.bool(False), ## use the neural network approach?

    # Store isolation values
    isolation = cms.PSet(
        tracker = cms.PSet(
            # source IsoDeposit
            src = cms.InputTag("layer0ElectronIsolations","eleIsoDepositTk"),
            # parameters to compute isolation (Egamma POG defaults)
            deltaR = cms.double(0.3),
            vetos = cms.vstring('0.02', # inner radius veto cone (was 0.015)
                'Threshold(1.0)'),       # threshold on individual track pt
            skipDefaultVeto = cms.bool(True),
        ),
         #ecal = cms.PSet(
         # source IsoDeposit
         #src = cms.InputTag("layer0ElectronIsolations","eleIsoDepositEcalFromHits"),
         #src = cms.InputTag("layer0ElectronIsolations","eleIsoDepositEcalFromClusts"),
         # parameters to compute isolation (Egamma POG defaults)
         #deltaR = cms.double(0.4),
         #vetos = cms.vstring('EcalBarrel:0.040', 'EcalBarrel:RectangularEtaPhiVeto(-0.01,0.01,-0.5,0.5)',  # Barrel (|eta| < 1.479)
         #'EcalEndcaps:0.070','EcalEndcaps:RectangularEtaPhiVeto(-0.02,0.02,-0.5,0.5)'),
         # skipDefaultVeto = cms.bool(True),
         #),

        ## other option, using eleIsoDepositEcalSCVetoFromClust (see also recoLayer0/electronIsolation_cff.py)
        ecal = cms.PSet( 
           src    = cms.InputTag("layer0ElectronIsolations", "eleIsoDepositEcalSCVetoFromClusts"),
           deltaR = cms.double(0.4),
           vetos  = cms.vstring(),     # no veto, already done with SC
           skipDefaultVeto = cms.bool(True),
        ),
        hcal = cms.PSet(
            # source IsoDeposit
             src = cms.InputTag("layer0ElectronIsolations","eleIsoDepositHcalFromHits"),
            #src = cms.InputTag("layer0ElectronIsolations","eleIsoDepositHcalFromTowers"),
            # parameters to compute isolation (Egamma POG defaults)
            deltaR = cms.double(0.4),
            skipDefaultVeto = cms.bool(True),
        ),
        user = cms.VPSet(),
    ),
    # Store IsoDeposits
    isoDeposits = cms.PSet(
        tracker = cms.InputTag("layer0ElectronIsolations","eleIsoDepositTk"),
        #ecal    = cms.InputTag("layer0ElectronIsolations","eleIsoDepositEcalFromHits"),
        ecal    = cms.InputTag("layer0ElectronIsolations","eleIsoDepositEcalSCVetoFromClusts"),
        #hcal    = cms.InputTag("layer0ElectronIsolations","eleIsoDepositHcalFromHits"),
        hcal    = cms.InputTag("layer0ElectronIsolations","eleIsoDepositHcalFromTowers"),
    ),

    # electron ID configurables
    addElectronID = cms.bool(True),
    electronIDSources = cms.PSet(
        # configure many IDs as InputTag <someName> = <someTag>
        # you can comment out those you don't want to save some disk space
        robust     = cms.InputTag("elecIdForETauCutBasedRobust"),
        loose      = cms.InputTag("elecIdForETauCutBasedLoose"),
        tight      = cms.InputTag("elecIdForETauCutBasedTight"),        
    ),

    # Trigger matching configurables
    addTrigMatch = cms.bool(True),
    # trigger primitive sources to be used for the matching
    trigPrimMatch = cms.VInputTag(
            cms.InputTag("electronTrigMatchHLT1ElectronRelaxed"), 
            cms.InputTag("electronTrigMatchCandHLT1ElectronStartup")
    ),

    # MC matching configurables
    addGenMatch      = cms.bool(False),
    embedGenMatch    = cms.bool(False),
    genParticleMatch = cms.InputTag("electronMatch"), ## Association between electrons and generator particles

    # Efficiencies
    addEfficiencies = cms.bool(False),
    efficiencies    = cms.PSet(),

)


