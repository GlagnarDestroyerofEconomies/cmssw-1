import FWCore.ParameterSet.Config as cms

process = cms.Process("ICALIB")
process.load("DQMServices.Core.DQMStore_cfg")

process.MessageLogger = cms.Service("MessageLogger",
    debugModules = cms.untracked.vstring('*'),
    cout = cms.untracked.PSet(
        threshold = cms.untracked.string('INFO')
    ),
    destinations = cms.untracked.vstring('cout')
)

process.source = cms.Source("EmptyIOVSource",
    firstValue = cms.uint64(150590),
    lastValue = cms.uint64(150590),
    timetype = cms.string('runnumber'),
    interval = cms.uint64(1)
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
)

process.load("Configuration.StandardSequences.Geometry_cff")

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = "GR10_E_V5::All" #GR09_R_34X_V2

#to read information of o2o and cabling
process.BadComponentsOnline = cms.ESSource("PoolDBESSource",
    appendToDataLabel = cms.string('online'),
    BlobStreamerName = cms.untracked.string('TBufferBlobStreamingService'),
    DBParameters = cms.PSet(
        authenticationPath = cms.untracked.string('/afs/cern.ch/cms/DB/conddb')
    ),
    timetype = cms.string('runnumber'),
    toGet = cms.VPSet(cms.PSet(
        record = cms.string('SiStripFedCablingRcd'),
        tag = cms.string('SiStripFedCabling_GR10_v1_hlt')
        ),
                      cms.PSet(
        record = cms.string('SiStripBadChannelRcd'),
        tag = cms.string('SiStripBadChannel_FromOnline_GR10_v1_hlt')
        )),
    connect = cms.string('oracle://cms_orcoff_prod/CMS_COND_31X_STRIP')
)

process.sistripconn = cms.ESProducer("SiStripConnectivity")

#to read information of RunInfo
process.poolDBESSourceRunInfo = cms.ESSource("PoolDBESSource",
   appendToDataLabel = cms.string('online2'),
   BlobStreamerName = cms.untracked.string('TBufferBlobStreamingService'),
   DBParameters = cms.PSet(
        messageLevel = cms.untracked.int32(2),
        authenticationPath = cms.untracked.string('/afs/cern.ch/cms/DB/conddb')
    ),
    timetype = cms.untracked.string('runnumber'),
    connect = cms.string('frontier://PromptProd/CMS_COND_31X_RUN_INFO'),
    toGet = cms.VPSet(cms.PSet(
        record = cms.string('RunInfoRcd'),
        tag = cms.string('runinfo_start_31X_hlt')
        )               
    )
)

#to produce ESetup based on o2o, cabling and RunInfo
process.MySSQ = cms.ESProducer("SiStripQualityESProducer",
    PrintDebug = cms.untracked.bool(True),
    PrintDebugOutput = cms.bool(False),
    UseEmptyRunInfo = cms.bool(False),
    appendToDataLabel = cms.string('OnlineMasking'),
    ReduceGranularity = cms.bool(True),
    ThresholdForReducedGranularity = cms.double(0.3),
    ListOfRecordToMerge = cms.VPSet(
    cms.PSet(
       record = cms.string('SiStripBadChannelRcd'),
       tag = cms.string('online')
    ),
    cms.PSet(
       record = cms.string('SiStripDetCablingRcd'),
       tag = cms.string('')
    ),
    cms.PSet(
       record = cms.string('RunInfoRcd'),
       tag = cms.string('online2')
   )
    )
)

process.PoolDBOutputService = cms.Service("PoolDBOutputService",
    BlobStreamerName = cms.untracked.string('TBufferBlobStreamingService'),
    DBParameters = cms.PSet(
        authenticationPath = cms.untracked.string('/afs/cern.ch/cms/DB/conddb')
    ),
    timetype = cms.untracked.string('runnumber'),
    connect = cms.string('sqlite_file:dbfile.db'),
    toPut = cms.VPSet(cms.PSet(
        record = cms.string('SiStripBadStrip'),
        tag = cms.string('SiStripHotComponents_merged')
    ))
)

process.statisticsFilter = cms.EDFilter("StatisticsFilter",
    rootFilename = cms.untracked.string('/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/data/OfflineData/HIRun2010/StreamExpress/0001505xx/DQM_V0014_R000150590__StreamExpress__HIRun2010-Express-v3__DQM.root'),
    histoDirPath = cms.untracked.string('Run 150590/AlCaReco'),
    minNumberOfEvents = cms.untracked.int32(10000)
)

process.prod = cms.EDAnalyzer("SiStripQualityHotStripIdentifierRoot",
    OccupancyRootFile = cms.untracked.string('BadAPVandStripOccupancy_150590.root'),
    WriteOccupancyRootFile = cms.untracked.bool(True), # Ouput File has a size of ~100MB. To suppress writing set parameter to 'False'
    UseInputDB = cms.untracked.bool(True),
    dataLabel=cms.untracked.string('OnlineMasking'),
    OccupancyH_Xmax = cms.untracked.double(1.0),
    AlgoParameters = cms.PSet(
        AlgoName = cms.string('SiStripBadAPVandHotStripAlgorithmFromClusterOccupancy'),
        OccupancyHisto = cms.untracked.string('ClusterDigiPosition__det__'),
        LowOccupancyThreshold  = cms.untracked.double(5),
        HighOccupancyThreshold = cms.untracked.double(5),
        AbsoluteLowThreshold   = cms.untracked.double(10),
        NumberIterations = cms.untracked.uint32(3),
        OccupancyThreshold = cms.untracked.double(0.002), #0.0001
        NumberOfEvents = cms.untracked.uint32(0),
        ProbabilityThreshold = cms.untracked.double(1e-07),
        MinNumEntriesPerStrip = cms.untracked.uint32(0),
        MinNumEntries = cms.untracked.uint32(0),
        UseInputDB = cms.untracked.bool(True)
    ),
    SinceAppendMode = cms.bool(True),
    verbosity = cms.untracked.uint32(0),
    OccupancyH_Xmin = cms.untracked.double(-0.0005),
    IOVMode = cms.string('Run'),
    Record = cms.string('SiStripBadStrip'),
    rootDirPath = cms.untracked.string('Run 150590/AlCaReco'),
    rootFilename = cms.untracked.string('/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/data/OfflineData/HIRun2010/StreamExpress/0001505xx/DQM_V0014_R000150590__StreamExpress__HIRun2010-Express-v3__DQM.root'),
    doStoreOnDB = cms.bool(True),
    OccupancyH_Nbin = cms.untracked.uint32(1001)
)

process.APVValidation = cms.EDAnalyzer('APVValidationPlots',
                              inputFilename  = cms.untracked.string('BadAPVandStripOccupancy_150590.root'),
                              outputFilename = cms.untracked.string('ChannelStatusPlots.root')
)

process.StripValidation = cms.EDAnalyzer('StripValidationPlots',
                              inputFilename  = cms.untracked.string('BadAPVandStripOccupancy_150590.root'),
                              outputFilename = cms.untracked.string('ChannelStatusPlots.root')
)

process.out = cms.OutputModule("AsciiOutputModule")

process.p = cms.Path(process.statisticsFilter*process.prod*process.APVValidation*process.StripValidation)
process.ep = cms.EndPath(process.out)
