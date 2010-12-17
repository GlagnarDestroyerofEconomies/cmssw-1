import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing

## setup 'analysis'  options
options = VarParsing.VarParsing ('analysis')

## setup any defaults you want
options.outputFile = "ZToMMGSkim.root"
options.maxEvents = -1 # -1 means all events
sourcePath = "/store/data/Run2010B/Mu/RECO/PromptReco-v2/000/149/291/"
sourceFiles = """
    FC6AEC33-C6E4-DF11-862E-003048F024FE.root
    F4FEDF51-E2E4-DF11-942D-001617C3B6CE.root
    F2061C09-D5E4-DF11-8C3F-001D09F2532F.root
    F08ACF28-D0E4-DF11-BBF8-001D09F23A34.root
    E61D8328-D0E4-DF11-86AC-001D09F28F0C.root
    E0BF91D0-D7E4-DF11-BB96-001D09F242EA.root
    E0BDCCE2-D2E4-DF11-9047-003048CFB40C.root
    DEF6D6AD-D5E4-DF11-8246-001D09F23174.root
    DE1F5440-D9E4-DF11-9A83-001617E30E2C.root
    DCD967F1-D9E4-DF11-AFB4-0030487C6090.root
    CE8BB813-D7E4-DF11-9913-001D09F250AF.root
    BC80D612-D7E4-DF11-9F89-001D09F2305C.root
    BAB4D9C0-D0E4-DF11-8153-0019B9F704D6.root
    B6B0BD26-D0E4-DF11-8E88-0019B9F704D6.root
    B4E2E406-D5E4-DF11-89A9-001D09F2462D.root
    B4ADEAAD-D5E4-DF11-83DC-003048D2C174.root
    AE8E2BA8-CEE4-DF11-81DC-003048F118D2.root
    A63B17B8-C9E4-DF11-AB5C-001617C3B6CC.root
    A4F73A5C-CFE4-DF11-B0F8-001D09F2512C.root
    944D5FD2-CBE4-DF11-AB73-003048F024FE.root
    926FD01C-D7E4-DF11-9CAF-001D09F24F1F.root
    92550663-D4E4-DF11-87E1-003048F1110E.root
    8EFEE9A8-CEE4-DF11-A8A2-001D09F242EF.root
    8A256BC1-D0E4-DF11-857B-0019B9F72F97.root
    88D7AE8E-D3E4-DF11-8156-003048F1C424.root
    82ED4626-D0E4-DF11-A115-0030486780B8.root
    7872AC78-C5E4-DF11-B662-0030486780EC.root
    7841F970-CAE4-DF11-9755-003048F118C4.root
    6C181436-D9E4-DF11-9AF1-001617E30F58.root
    6AB281E1-D2E4-DF11-BA67-001D09F24F1F.root
    6A200A62-D4E4-DF11-937D-000423D94E70.root
    5EC0C85A-CFE4-DF11-AB2D-003048F118AC.root
    5E84778E-D3E4-DF11-A1F5-003048F11CF0.root
    5C8E518E-D3E4-DF11-B9C2-003048F110BE.root
    56AC87E1-D2E4-DF11-AB60-003048F11114.root
    52935EA9-CEE4-DF11-9A11-001D09F24DA8.root
    500AFDE0-ECE4-DF11-BFE0-003048F024DC.root
    4E1FA0A6-DAE4-DF11-BDF3-001D09F23A3E.root
    4E1B1FC2-D0E4-DF11-92DF-001D09F231B0.root
    4C14F509-D5E4-DF11-85E5-0019B9F4A1D7.root
    3889EA35-C6E4-DF11-951F-003048F024F6.root
    365E9FC1-D0E4-DF11-BB74-001D09F28F0C.root
    360180A9-CEE4-DF11-887B-001D09F25109.root
    2CAAB35D-CFE4-DF11-A9CC-001D09F27003.root
    2A0CD409-D5E4-DF11-AF64-0019B9F72D71.root
    1626C4AE-D5E4-DF11-A70E-001D09F25109.root
    10D0FC37-D9E4-DF11-A875-0030487BC68E.root
    0E1067A9-CEE4-DF11-A52E-001D09F25456.root
    0CB2D85A-CFE4-DF11-8031-003048F024F6.root
    0C7164CA-C4E4-DF11-8E19-0019B9F70468.root
    02AD73D0-D7E4-DF11-945F-001D09F28F25.root
    """.split()
options.inputFiles = ",".join([sourcePath + f for f in sourceFiles])

# get and parse the command line arguments
options.parseArguments()


process = cms.Process('SKIM')

process.load("FWCore.MessageLogger.MessageLogger_cfi")
if options.maxEvents < 0:
    process.MessageLogger.cerr.FwkReport.reportEvery = 100

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(options.maxEvents)
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(options.inputFiles)
)

process.load("ElectroWeakAnalysis.MultiBosons.Skimming.ZToMMGSkim_cff")
process.ZToMMGSkimFilterPath = cms.Path(process.ZToMMGSkimFilterSequence)

process.out = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string(options.outputFile),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring("ZToMMGSkimFilterPath")
    ),
)

process.endpath = cms.EndPath(process.out)

if __name__ == "__main__": import user
