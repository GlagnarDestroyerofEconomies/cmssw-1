import FWCore.ParameterSet.Config as cms

from DQMOffline.Trigger.MuonHLTValidation_cfi import *

from DQMOffline.Trigger.EgHLTOfflineSummaryClient_cfi import *

from DQMOffline.Trigger.DQMOffline_Trigger_SummaryCert_cfi import *

#--- Note: hltOverallCertSeq must be the last sequence!
#-- it relies on bits set in the other sequences
dqmOfflineTriggerCert = cms.Sequence(muonHLTCertSeq*egHLTOffCertSeq*hltOverallCertSeq)


