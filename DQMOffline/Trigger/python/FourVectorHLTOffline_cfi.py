import FWCore.ParameterSet.Config as cms
# $Id: FourVectorHLTOffline_cfi.py,v 1.14 2009/01/29 15:33:11 rekovic Exp $

hltResults = cms.EDFilter("FourVectorHLTOffline",
    plotAll = cms.untracked.bool(False),
    ptMax = cms.untracked.double(100.0),
    ptMin = cms.untracked.double(0.0),
    paths = cms.VPSet(
# single jet triggers
             cms.PSet(
              pathname = cms.string("HLT_Jet50"),
              denompathname = cms.string("HLT_Jet30"),  
             ),
             cms.PSet(
              pathname = cms.string("HLT_Jet110"),
              denompathname = cms.string("HLT_Jet50"),  
             )
            ),
                          
     # this is I think MC and CRUZET4
    triggerSummaryLabel = cms.InputTag("hltTriggerSummaryAOD","","HLT"),
    triggerResultsLabel = cms.InputTag("TriggerResults","","HLT"),
    gtObjectMapRecordLabel = cms.InputTag("hltL1GtObjectMap","","HLT"),
    l1GTRRLabel = cms.InputTag("gtDigis","",""),
    processname = cms.string("HLT")

    # this is data (CRUZET I or II best guess)
    #triggerSummaryLabel = cms.InputTag("hltTriggerSummaryAOD","","FU"),
    #triggerResultsLabel = cms.InputTag("TriggerResults","","FU"),
    #processname = cms.string("FU")

 )
