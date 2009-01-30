import FWCore.ParameterSet.Config as cms

# maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring() 
source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles)
readFiles.extend( [
       '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP_V5_v1/0004/06EC90FA-816C-DD11-B8FE-001617E30F4C.root',
       '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP_V5_v1/0004/127CE150-806C-DD11-B0B0-000423D98B5C.root',
       '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP_V5_v1/0004/14D23EB4-806C-DD11-A228-000423D98DD4.root',
       '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP_V5_v1/0004/14F971B3-816C-DD11-A88F-001617E30CE8.root',
       '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP_V5_v1/0004/202B8F81-806C-DD11-8618-000423D944DC.root',
       '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP_V5_v1/0004/24055C81-806C-DD11-BDF1-001617C3B79A.root',
       '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP_V5_v1/0004/24C81B27-806C-DD11-AB7E-001617E30D4A.root',
       '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP_V5_v1/0004/30FABF86-7F6C-DD11-8710-000423D98B5C.root',
       '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP_V5_v1/0004/3407BDAE-806C-DD11-83F2-001617C3B6CC.root',
       '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP_V5_v1/0004/34FCA5DA-806C-DD11-A890-001617E30F4C.root',
       '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP_V5_v1/0004/42578386-7F6C-DD11-A4A1-000423D98834.root',
       '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP_V5_v1/0004/42AAD705-816C-DD11-AD80-001617C3B77C.root',
       '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP_V5_v1/0004/44A972C4-806C-DD11-8094-001617DBD472.root',
       '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP_V5_v1/0004/4A2C64DB-806C-DD11-A84C-001617C3B778.root',
       '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP_V5_v1/0004/6243A57F-806C-DD11-A0DA-000423D9A212.root',
       '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP_V5_v1/0004/683B5CBF-816C-DD11-9BED-001617DBCF6A.root',
       '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP_V5_v1/0004/6A5FDA85-806C-DD11-AE5C-000423D9939C.root',
       '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP_V5_v1/0004/6AACB643-806C-DD11-96B3-001617DBD224.root',
       '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP_V5_v1/0004/6AE7DA4D-806C-DD11-8E60-000423D99E46.root',
       '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP_V5_v1/0004/6AFFE9DF-806C-DD11-95D5-001617DBD224.root',
       '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP_V5_v1/0004/6CB140A7-816C-DD11-825B-000423D95220.root',
       '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP_V5_v1/0004/70146535-816C-DD11-B4EA-000423D94908.root',
       '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP_V5_v1/0004/70CC7FFE-806C-DD11-8011-001617C3B76E.root',
       '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP_V5_v1/0004/740AD73C-816C-DD11-B7D7-001617DBD224.root',
       '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP_V5_v1/0004/7646127D-816C-DD11-9161-001617C3B65A.root',
       '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP_V5_v1/0004/765353EF-7F6C-DD11-B778-0016177CA7A0.root',
       '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP_V5_v1/0004/78821E42-816C-DD11-B5CC-001617DBD5AC.root',
       '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP_V5_v1/0004/78FA9058-806C-DD11-A633-001617C3B76E.root',
       '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP_V5_v1/0004/7C567A82-806C-DD11-ABB9-000423D98C20.root',
       '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP_V5_v1/0004/7E2C2AA9-816C-DD11-AB19-000423D99AA2.root',
       '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP_V5_v1/0004/7E64CACD-7F6C-DD11-ADBB-000423D9997E.root',
       '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP_V5_v1/0004/84BE969F-816C-DD11-B37E-001617C3B70E.root',
       '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP_V5_v1/0004/901D6E4F-806C-DD11-8443-000423D99AAE.root',
       '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP_V5_v1/0004/90E0AE9A-816C-DD11-A055-001617DBD5AC.root',
       '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP_V5_v1/0004/9863A393-816C-DD11-B990-001617C3B70E.root',
       '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP_V5_v1/0004/9A1C1AE4-816C-DD11-A4BB-000423D944DC.root',
       '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP_V5_v1/0004/9ED19410-816C-DD11-8379-000423D98E6C.root',
       '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP_V5_v1/0004/A465C955-816C-DD11-A680-000423D98834.root',
       '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP_V5_v1/0004/A493BC7B-806C-DD11-BBCB-001617C3B6C6.root',
       '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP_V5_v1/0004/AEDE6F13-816C-DD11-A300-001617C3B65A.root',
       '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP_V5_v1/0004/B2DC384E-806C-DD11-AD2C-000423D98EC4.root',
       '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP_V5_v1/0004/B4405CD8-806C-DD11-8BF8-001617E30F58.root',
       '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP_V5_v1/0004/BA8F6803-816C-DD11-9FD3-001617E30D4A.root',
       '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP_V5_v1/0004/BE46AAB6-806C-DD11-A036-000423D60FF6.root',
       '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP_V5_v1/0004/C6A36E3C-816C-DD11-B0A0-001617DBD230.root',
       '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP_V5_v1/0004/C8C3A9A2-816C-DD11-AF7C-000423D99896.root',
       '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP_V5_v1/0004/CACBD67B-806C-DD11-B468-001617E30F50.root',
       '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP_V5_v1/0004/CC907F16-806C-DD11-94AE-000423D94494.root',
       '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP_V5_v1/0004/D2307A6F-816C-DD11-9724-001617C3B77C.root',
       '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP_V5_v1/0004/D60C86AC-806C-DD11-9591-001617E30D4A.root',
       '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP_V5_v1/0004/D8886A13-806C-DD11-A6BB-000423D9970C.root',
       '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP_V5_v1/0004/E4D835A9-816C-DD11-B3B3-000423D991F0.root',
       '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP_V5_v1/0004/EA329C74-816C-DD11-97D1-001617DBD556.root',
       '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP_V5_v1/0004/ECFF0074-806C-DD11-9E78-001617E30CD4.root',
       '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP_V5_v1/0004/EE871138-816C-DD11-B723-001617E30F4C.root',
       '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP_V5_v1/0004/F0E42303-816C-DD11-AA47-001617DBD5B2.root',
       '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP_V5_v1/0004/F420A6E0-816C-DD11-9CB9-000423D9853C.root',
       '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP_V5_v1/0004/F6D6601A-806C-DD11-ACEA-000423D98634.root',
       '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP_V5_v1/0004/F6FD24F8-7F6C-DD11-B8C0-001617C3B76A.root',
       '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP_V5_v1/0004/F82B3559-7F6C-DD11-B436-001617C3B6CC.root',
       '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP_V5_v1/0004/F87832CD-7F6C-DD11-84FF-001617E30CE8.root',
       '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP_V5_v1/0004/FA64BB1E-806C-DD11-87A1-001617C3B76A.root',
       '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP_V5_v1/0004/FAAEF355-806C-DD11-8483-000423D99658.root',
       '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP_V5_v1/0004/FE46CB3B-816C-DD11-97D0-0019DB29C614.root',
       '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP_V5_v1/0004/FEA41872-816C-DD11-975A-001617C3B76A.root',
       '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP_V5_v1/0005/B02FA2B2-AC6C-DD11-ABE8-001617C3B654.root' ] );
