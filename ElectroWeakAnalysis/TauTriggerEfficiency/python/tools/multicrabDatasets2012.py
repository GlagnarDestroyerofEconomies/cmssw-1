from HiggsAnalysis.HeavyChHiggsToTauNu.tools.multicrabWorkflowsTools import TaskDef
from HiggsAnalysis.HeavyChHiggsToTauNu.tools.multicrabWorkflowsTriggerEff import addMetLegSkim_53X

skimVersion = "V00_12_03_CMSSW537_v1"
def addMetLegSkim(datasets):
    definitions = {
	"Tau_190456-190738_2012A_Jul13":		TaskDef("/Tau/local-Run2012A_13Jul2012_v1_AOD_190456_190738_analysis_metleg_v53_v1-165ff7ea0b6b7dc6426443c42f2a9122/USER"),
	"Tau_190782-190949_2012A_Aug06":             	TaskDef("/Tau/local-Run2012A_recover_06Aug2012_v1_AOD_190782_190949_analysis_metleg_v53_v1-de59482928f61dc06955dcb204f66afe/USER"),
	"Tau_191043-193621_2012A_Jul13":             	TaskDef("/Tau/local-Run2012A_13Jul2012_v1_AOD_191043_193621_analysis_metleg_v53_v1-43665de791ab95ebca4e518ee6c20319/USER"),
	"Tau_193834-196531_2012B_Jul13":             	TaskDef("/Tau/local-Run2012B_13Jul2012_v1_AOD_193834_196531_analysis_metleg_v53_v1-24eee222e7d7579368f51155ee8c17c1/USER"),
	"Tau_198022-198523_2012C_Aug24":             	TaskDef("/Tau/local-Run2012C_24Aug2012_v1_AOD_198022_198523_analysis_metleg_v53_v1-16cf49e1333af46ec078168d0a11d2bc/USER"),
	"Tau_198941-200601_2012C_Prompt":             	TaskDef("/Tau/local-Run2012C_PromptReco_v2_AOD_198941_200601_analysis_metleg_v53_v1-7a1c0e3252a37dec457bef41ab89bd2b/USER"),
	"Tau_200961-202504_2012C_Prompt":             	TaskDef("/Tau/local-Run2012C_PromptReco_v2_AOD_200961_202504_analysis_metleg_v53_v1-fe7b61f7d8aa75fa2fdc2671653816c0/USER"),
	"Tau_202792-203742_2012C_Prompt":             	TaskDef("/Tau/local-Run2012C_PromptReco_v2_AOD_202792_203742_analysis_metleg_v53_v1-122fb14a264e8e421d259fc9ade8407a/USER"),
	"Tau_203777-208686_2012D_Prompt":             	TaskDef("/Tau/local-Run2012D_PromptReco_v1_AOD_203777_208686_analysis_metleg_v53_v1-4edeba655e16935bfbce88d52178887d/USER"),

	"QCD_Pt30to50_TuneZ2star_Summer12":             TaskDef("/QCD_Pt-30to50_TuneZ2star_8TeV_pythia6/local-Summer12_DR53X_PU_S10_START53_V7A_v2_AODSIM_analysis_metleg_v53_v1-638a70bdbf1f7414f9f442a75689ed2b/USER"),
	"QCD_Pt50to80_TuneZ2star_Summer12":             TaskDef("/QCD_Pt-50to80_TuneZ2star_8TeV_pythia6/local-Summer12_DR53X_PU_S10_START53_V7A_v2_AODSIM_analysis_metleg_v53_v1-638a70bdbf1f7414f9f442a75689ed2b/USER"),
	"QCD_Pt80to120_TuneZ2star_Summer12":            TaskDef("/QCD_Pt-80to120_TuneZ2star_8TeV_pythia6/local-Summer12_DR53X_PU_S10_START53_V7A_v3_AODSIM_analysis_metleg_v53_v1-638a70bdbf1f7414f9f442a75689ed2b/USER"),
	"QCD_Pt120to170_TuneZ2star_Summer12":           TaskDef("/QCD_Pt-120to170_TuneZ2star_8TeV_pythia6/local-Summer12_DR53X_PU_S10_START53_V7A_v3_AODSIM_analysis_metleg_v53_v1-638a70bdbf1f7414f9f442a75689ed2b/USER"),
	"QCD_Pt170to300_TuneZ2star_Summer12":           TaskDef("/QCD_Pt-170to300_TuneZ2star_8TeV_pythia6/local-Summer12_DR53X_PU_S10_START53_V7A_v2_AODSIM_analysis_metleg_v53_v1-638a70bdbf1f7414f9f442a75689ed2b/USER"),
	"QCD_Pt170to300_TuneZ2star_v2_Summer12":        TaskDef("/QCD_Pt-170to300_TuneZ2star_8TeV_pythia6_v2/local-Summer12_DR53X_PU_S10_START53_V7A_v1_AODSIM_analysis_metleg_v53_v1-638a70bdbf1f7414f9f442a75689ed2b/USER"),
	"QCD_Pt300to470_TuneZ2star_Summer12":           TaskDef("/QCD_Pt-300to470_TuneZ2star_8TeV_pythia6/local-Summer12_DR53X_PU_S10_START53_V7A_v2_AODSIM_analysis_metleg_v53_v1-638a70bdbf1f7414f9f442a75689ed2b/USER"),
	"QCD_Pt300to470_TuneZ2star_v2_Summer12":        TaskDef("/QCD_Pt-300to470_TuneZ2star_8TeV_pythia6_v2/local-Summer12_DR53X_PU_S10_START53_V7A_v1_AODSIM_analysis_metleg_v53_v1-638a70bdbf1f7414f9f442a75689ed2b/USER"),
	"QCD_Pt300to470_TuneZ2star_v3_Summer12":        TaskDef("/QCD_Pt-300to470_TuneZ2star_8TeV_pythia6_v3/local-Summer12_DR53X_PU_S10_START53_V7A_v1_AODSIM_analysis_metleg_v53_v1-638a70bdbf1f7414f9f442a75689ed2b/USER"),

	"WW_TuneZ2star_Summer12":             		TaskDef("/WW_TuneZ2star_8TeV_pythia6_tauola/local-Summer12_DR53X_PU_S10_START53_V7A_v1_AODSIM_analysis_metleg_v53_v1-638a70bdbf1f7414f9f442a75689ed2b/USER"),
	"WZ_TuneZ2star_Summer12":             		TaskDef("/WZ_TuneZ2star_8TeV_pythia6_tauola/local-Summer12_DR53X_PU_S10_START53_V7A_v1_AODSIM_analysis_metleg_v53_v1-638a70bdbf1f7414f9f442a75689ed2b/USER"),
	"ZZ_TuneZ2star_Summer12":             		TaskDef("/ZZ_TuneZ2star_8TeV_pythia6_tauola/local-Summer12_DR53X_PU_S10_START53_V7A_v1_AODSIM_analysis_metleg_v53_v1-638a70bdbf1f7414f9f442a75689ed2b/USER"),
	"TTJets_TuneZ2star_Summer12":           	TaskDef("/TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola/local-Summer12_DR53X_PU_S10_START53_V7A_v1_AODSIM_analysis_metleg_v53_v1-638a70bdbf1f7414f9f442a75689ed2b/USER"),
	"WJets_TuneZ2star_v1_Summer12":         	TaskDef("/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/local-Summer12_DR53X_PU_S10_START53_V7A_v1_AODSIM_analysis_metleg_v53_v1-638a70bdbf1f7414f9f442a75689ed2b/USER"),
	"WJets_TuneZ2star_v2_Summer12":         	TaskDef("/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/local-Summer12_DR53X_PU_S10_START53_V7A_v2_AODSIM_analysis_metleg_v53_v1-638a70bdbf1f7414f9f442a75689ed2b/USER"),
	"W1Jets_TuneZ2star_Summer12":           	TaskDef("/W1JetsToLNu_TuneZ2Star_8TeV-madgraph/local-Summer12_DR53X_PU_S10_START53_V7A_v1_AODSIM_analysis_metleg_v53_v1-638a70bdbf1f7414f9f442a75689ed2b/USER"),
	"W2Jets_TuneZ2star_Summer12":           	TaskDef("/W2JetsToLNu_TuneZ2Star_8TeV-madgraph/local-Summer12_DR53X_PU_S10_START53_V7A_v1_AODSIM_analysis_metleg_v53_v1-638a70bdbf1f7414f9f442a75689ed2b/USER"),
	"W3Jets_TuneZ2star_Summer12":           	TaskDef("/W3JetsToLNu_TuneZ2Star_8TeV-madgraph/local-Summer12_DR53X_PU_S10_START53_V7A_v1_AODSIM_analysis_metleg_v53_v1-638a70bdbf1f7414f9f442a75689ed2b/USER"),
	"W4Jets_TuneZ2star_Summer12":           	TaskDef("/W4JetsToLNu_TuneZ2Star_8TeV-madgraph/local-Summer12_DR53X_PU_S10_START53_V7A_v1_AODSIM_analysis_metleg_v53_v1-638a70bdbf1f7414f9f442a75689ed2b/USER"),
	"DYJetsToLL_M50_TuneZ2star_Summer12":           TaskDef("/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/local-Summer12_DR53X_PU_S10_START53_V7A_v1_AODSIM_analysis_metleg_v53_v1-638a70bdbf1f7414f9f442a75689ed2b/USER"),
	"DYJetsToLL_M10to50_TuneZ2star_Summer12":	TaskDef("/DYJetsToLL_M-10To50_TuneZ2Star_8TeV-madgraph/local-Summer12_DR53X_PU_S10_START53_V7A_v1_AODSIM_analysis_metleg_v53_v1-638a70bdbf1f7414f9f442a75689ed2b/USER"),
	"T_t-channel_TuneZ2star_Summer12":             	TaskDef("/T_t-channel_TuneZ2star_8TeV-powheg-tauola/local-Summer12_DR53X_PU_S10_START53_V7A_v1_AODSIM_analysis_metleg_v53_v1-638a70bdbf1f7414f9f442a75689ed2b/USER"),
	"Tbar_t-channel_TuneZ2star_Summer12":           TaskDef("/Tbar_t-channel_TuneZ2star_8TeV-powheg-tauola/local-Summer12_DR53X_PU_S10_START53_V7A_v1_AODSIM_analysis_metleg_v53_v1-638a70bdbf1f7414f9f442a75689ed2b/USER"),
	"T_tW-channel_TuneZ2star_Summer12":             TaskDef("/T_tW-channel-DR_TuneZ2star_8TeV-powheg-tauola/local-Summer12_DR53X_PU_S10_START53_V7A_v1_AODSIM_analysis_metleg_v53_v1-638a70bdbf1f7414f9f442a75689ed2b/USER"),
	"Tbar_tW-channel_TuneZ2star_Summer12":          TaskDef("/Tbar_tW-channel-DR_TuneZ2star_8TeV-powheg-tauola/local-Summer12_DR53X_PU_S10_START53_V7A_v1_AODSIM_analysis_metleg_v53_v1-638a70bdbf1f7414f9f442a75689ed2b/USER"),
	"T_s-channel_TuneZ2star_Summer12":             	TaskDef("/T_s-channel_TuneZ2star_8TeV-powheg-tauola/local-Summer12_DR53X_PU_S10_START53_V7A_v1_AODSIM_analysis_metleg_v53_v1-638a70bdbf1f7414f9f442a75689ed2b/USER"),
	"Tbar_s-channel_TuneZ2star_Summer12":           TaskDef("/Tbar_s-channel_TuneZ2star_8TeV-powheg-tauola/local-Summer12_DR53X_PU_S10_START53_V7A_v1_AODSIM_analysis_metleg_v53_v1-638a70bdbf1f7414f9f442a75689ed2b/USER"),
    }

    addMetLegSkim_53X(skimVersion, datasets, definitions)
