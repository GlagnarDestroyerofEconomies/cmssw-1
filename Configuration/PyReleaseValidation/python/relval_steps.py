

class Matrix(dict):
    def __setitem__(self,key,value):
        if key in self:
            print "ERROR in Matrix"
            print "overwritting",key,"not allowed"
        else:
            self.update({float(key):WF(float(key),value)})

            
#the class to collect all possible steps
class Steps(dict):
    def __setitem__(self,key,value):
        if key in self:
            print "ERROR in Step"
            print "overwritting",key,"not allowed"
        else:
            self.update({key:value})
            # make the python file named <step>.py
            #if not '--python' in value:                self[key].update({'--python':'%s.py'%(key,)})

    def overwrite(self,keypair):
        value=self[keypair[1]]
        print "overwritting step",keypair[0],"with",keypair[1],str(value)
        self.update({keypair[0]:value})
        
class WF(list):
    def __init__(self,n,l):
        self.extend(l)
        self.num=n
        #the actual steps of this WF
        self.steps=[]

        
    def interpret(self,stepsDict):
        for s in self:
            steps.append(stepsDict[s])
    
InputInfoNDefault=2000000    
class InputInfo(object):
    def __init__(self,dataSet,label='',run=[],files=1000,events=InputInfoNDefault,location='CAF') :
        self.run = run
        self.files = files
        self.events = events
        self.location = location
        self.label = label
        self.dataSet = dataSet

    def dbs(self):
        command='dbs search --noheader --query "find file where dataset like '+self.dataSet
        def requ(r):
            return 'run=%d'%(r,)
        if len(self.run)!=0:
            command+=' and ('+' or '.join(map(requ,self.run))+' )'
        command+='"'
        return command
    def __str__(self):
        return 'input from: %s with run: %s'%(self.dataSet,str(self.run))
    
# merge dictionaries, with prioty on the [0] index
def merge(dictlist,TELL=False):
    import copy
    last=len(dictlist)-1
    if TELL: print last,dictlist
    if last==0:
        # ONLY ONE ITEM LEFT
        return copy.copy(dictlist[0])
    else:
        reducedlist=dictlist[0:max(0,last-1)]
        if TELL: print reducedlist
        # make a copy of the last item
        d=copy.copy(dictlist[last])
        # update with the last but one item
        d.update(dictlist[last-1])
        # and recursively do the rest
        reducedlist.append(d)
        return merge(reducedlist,TELL)


# step1 gensim
step1Defaults = {'--relval'      : None, # need to be explicitly set
                 '-s'            : 'GEN,SIM',
                 '-n'            : 10,
                 '--conditions'  : 'auto:startup',
                 '--datatier'    : 'GEN-SIM',
                 '--eventcontent': 'RAWSIM',
                 }

steps = Steps()

#### Production test section ####
steps['ProdMinBias']=merge([{'cfg':'MinBias_8TeV_cfi','--relval':'9000,100'},step1Defaults])
steps['ProdTTbar']=merge([{'cfg':'TTbar_Tauola_8TeV_cfi','--relval':'9000,50'},step1Defaults])
steps['ProdQCD_Pt_3000_3500']=merge([{'cfg':'QCD_Pt_3000_3500_8TeV_cfi','--relval':'9000,25'},step1Defaults])

#steps['ProdMinBiasINPUT']={'INPUT':InputInfo(dataSet='/RelValProdMinBias/CMSSW_4_3_0_pre2-MC_42_V9-v1/GEN-SIM',label='prodmbrv',location='STD')}
#steps['ProdTTbarINPUT']={'INPUT':InputInfo(dataSet='/RelValProdTTbar/CMSSW_4_3_0_pre2-MC_42_V9-v1/GEN-SIM',label='prodttbrv',location='STD')}
#steps['ProdQCD_Pt_3000_3500INPUT']={'INPUT':InputInfo(dataSet='/RelValProdQCD_Pt_3000_3500/CMSSW_4_3_0_pre2-MC_42_V9-v1/GEN-SIM',label='qcd335',location='STD')}



#### data ####
#list of run to harvest for 2010A: 144086,144085,144084,144083,144011,139790,139789,139788,139787,138937,138934,138924,138923
#list of run to harvest for 2010B: 149442,149291,149181,149011,148822,147929,147115,146644
Run2010ASk=[138937,138934,138924,138923,139790,139789,139788,139787,144086,144085,144084,144083,144011]
Run2010BSk=[146644,147115,147929,148822,149011,149181,149182,149291,149294,149442]
steps['MinimumBias2010A']={'INPUT':InputInfo(dataSet='/MinimumBias/Run2010A-valskim-v6/RAW-RECO',label='run2010A',location='STD',run=Run2010ASk)}
steps['MinimumBias2010B']={'INPUT':InputInfo(dataSet='/MinimumBias/Run2010B-valskim-v2/RAW-RECO',label='run2010B',run=Run2010BSk)}
steps['WZMuSkim2010A']={'INPUT':InputInfo(dataSet='/Mu/Run2010A-WZMu-Nov4Skim_v1/RAW-RECO',label='wzMu2010A',run=Run2010ASk)}
steps['WZMuSkim2010B']={'INPUT':InputInfo(dataSet='/Mu/Run2010B-WZMu-Nov4Skim_v1/RAW-RECO',label='wzMu2010B',run=Run2010BSk)}
steps['WZEGSkim2010A']={'INPUT':InputInfo(dataSet='/EG/Run2010A-WZEG-Nov4Skim_v1/RAW-RECO',label='wzEG2010A',run=Run2010ASk)}
steps['WZEGSkim2010B']={'INPUT':InputInfo(dataSet='/Electron/Run2010B-WZEG-Nov4Skim_v1/RAW-RECO',label='wzEG2010B',run=Run2010BSk)}

steps['RunCosmicsA']={'INPUT':InputInfo(dataSet='/Cosmics/Run2010A-v1/RAW',label='cos2010A',run=[142089],events=100000)}
Run2010B=[149011]
steps['RunMinBias2010B']={'INPUT':InputInfo(dataSet='/MinimumBias/Run2010B-RelValRawSkim-v1/RAW',label='mb2010B',run=Run2010B,events=100000)}
steps['RunMu2010B']={'INPUT':InputInfo(dataSet='/Mu/Run2010B-RelValRawSkim-v1/RAW',label='mu2010B',run=Run2010B,events=100000)}
steps['RunElectron2010B']={'INPUT':InputInfo(dataSet='/Electron/Run2010B-RelValRawSkim-v1/RAW',label='electron2010B',run=Run2010B,events=100000)}
steps['RunPhoton2010B']={'INPUT':InputInfo(dataSet='/Photon/Run2010B-RelValRawSkim-v1/RAW',label='photon2010B',run=Run2010B,events=100000)}
steps['RunJet2010B']={'INPUT':InputInfo(dataSet='/Jet/Run2010B-RelValRawSkim-v1/RAW',label='jet2010B',run=Run2010B,events=100000)}

#list of run to harvest 2011A: 165121, 172802,
Run2011ASk=[165121,172802]
steps['ValSkim2011A']={'INPUT':InputInfo(dataSet='/MinimumBias/Run2011A-ValSkim-PromptSkim-v6/RAW-RECO',label='run2011A',location='STD',run=Run2011ASk)}
steps['WMuSkim2011A']={'INPUT':InputInfo(dataSet='/SingleMu/Run2011A-WMu-PromptSkim-v6/RAW-RECO',label='wMu2011A',location='STD',run=Run2011ASk)}
steps['WElSkim2011A']={'INPUT':InputInfo(dataSet='/SingleElectron/Run2011A-WElectron-PromptSkim-v6/RAW-RECO',label='wEl2011A',location='STD',run=Run2011ASk)}
steps['ZMuSkim2011A']={'INPUT':InputInfo(dataSet='/DoubleMu/Run2011A-ZMu-PromptSkim-v6/RAW-RECO',label='zMu2011A',location='STD',run=Run2011ASk)}
steps['ZElSkim2011A']={'INPUT':InputInfo(dataSet='/DoubleElectron/Run2011A-ZElectron-PromptSkim-v6/RAW-RECO',label='zEl2011A',location='STD',run=Run2011ASk)}
steps['HighMet2011A']={'INPUT':InputInfo(dataSet='/Jet/Run2011A-HighMET-PromptSkim-v6/RAW-RECO',label='hMet2011A',location='STD',run=Run2011ASk)}

steps['RunCosmics2011A']={'INPUT':InputInfo(dataSet='/Cosmics/Run2011A-v1/RAW',label='cos2011A',run=[160960],events=100000,location='STD')}
Run2011A=[165121]
steps['RunMinBias2011A']={'INPUT':InputInfo(dataSet='/MinimumBias/Run2011A-v1/RAW',label='mb2011A',run=Run2011A,events=100000,location='STD')}
steps['RunMu2011A']={'INPUT':InputInfo(dataSet='/SingleMu/Run2011A-v1/RAW',label='mu2011A',run=Run2011A,events=100000)}
steps['RunElectron2011A']={'INPUT':InputInfo(dataSet='/SingleElectron/Run2011A-v1/RAW',label='electron2011A',run=Run2011A,events=100000)}
steps['RunPhoton2011A']={'INPUT':InputInfo(dataSet='/Photon/Run2011A-v1/RAW',label='photon2011A',run=Run2011A,events=100000)}
steps['RunJet2011A']={'INPUT':InputInfo(dataSet='/Jet/Run2011A-v1/RAW',label='jet2011A',run=Run2011A,events=100000)}

Run2011B=[177719]
Run2011BSk=[177719,177790,177096,175874]
steps['RunMinBias2011B']={'INPUT':InputInfo(dataSet='/MinimumBias/Run2011B-v1/RAW',label='mb2011B',run=Run2011B,events=100000,location='STD')}
steps['RunMu2011B']={'INPUT':InputInfo(dataSet='/SingleMu/Run2011B-v1/RAW',label='mu2011B',run=Run2011B,events=100000)}
steps['RunElectron2011B']={'INPUT':InputInfo(dataSet='/SingleElectron/Run2011B-v1/RAW',label='electron2011B',run=Run2011B,events=100000)}
steps['RunPhoton2011B']={'INPUT':InputInfo(dataSet='/Photon/Run2011B-v1/RAW',label='photon2011B',run=Run2011B,events=100000)}
steps['RunJet2011B']={'INPUT':InputInfo(dataSet='/Jet/Run2011B-v1/RAW',label='jet2011B',run=Run2011B,events=100000)}

steps['ValSkim2011B']={'INPUT':InputInfo(dataSet='/MinimumBias/Run2011B-ValSkim-PromptSkim-v1/RAW-RECO',label='run2011B',location='STD',run=Run2011BSk)}
steps['WMuSkim2011B']={'INPUT':InputInfo(dataSet='/SingleMu/Run2011B-WMu-PromptSkim-v1/RAW-RECO',label='wMu2011B',location='STD',run=Run2011BSk)}
steps['WElSkim2011B']={'INPUT':InputInfo(dataSet='/SingleElectron/Run2011B-WElectron-PromptSkim-v1/RAW-RECO',label='wEl2011B',location='STD',run=Run2011BSk)}
steps['ZMuSkim2011B']={'INPUT':InputInfo(dataSet='/DoubleMu/Run2011B-ZMu-PromptSkim-v1/RAW-RECO',label='zMu2011B',location='STD',run=Run2011BSk)}
steps['ZElSkim2011B']={'INPUT':InputInfo(dataSet='/DoubleElectron/Run2011B-ZElectron-PromptSkim-v1/RAW-RECO',label='zEl2011B',run=Run2011BSk)}
steps['HighMet2011B']={'INPUT':InputInfo(dataSet='/Jet/Run2011B-HighMET-PromptSkim-v1/RAW-RECO',label='hMet2011B',run=Run2011BSk)}

steps['RunHI2010']={'INPUT':InputInfo(dataSet='/HIAllPhysics/HIRun2010-v1/RAW',label='hi2010',run=[152698],events=10000,location='STD')}
steps['RunHI2011']={'INPUT':InputInfo(dataSet='/HIAllPhysics/HIRun2011A-v1/RAW',label='hi2011',run=[174773],events=10000,location='STD')}

#### Standard release validation samples ####

stCond={'--conditions':'auto:startup'}
K9by25={'--relval':'9000,25'}
K9by50={'--relval':'9000,50'}
K9by500={'--relval':'9000,500'}
K9by100={'--relval':'9000,100'}
K50by100={'--relval':'50000,100'}
K9by250={'--relval':'9000,250'}
K25by250={'--relval':'25000,250'}

def gen(fragment,howMuch):
    global step1Defaults
    return merge([{'cfg':fragment},howMuch,step1Defaults])

steps['MinBias']=gen('MinBias_8TeV_cfi',K9by100)
steps['QCD_Pt_3000_3500']=gen('QCD_Pt_3000_3500_8TeV_cfi',K9by25)
steps['QCD_Pt_80_120']=gen('QCD_Pt_80_120_8TeV_cfi',K9by50)
steps['SingleElectronPt10']=gen('SingleElectronPt10_cfi',K9by250)
steps['SingleElectronPt35']=gen('SingleElectronPt35_cfi',K9by250)
steps['SingleGammaPt10']=gen('SingleGammaPt10_cfi',K9by100)
steps['SingleGammaPt35']=gen('SingleGammaPt35_cfi',K9by100)
steps['SingleMuPt10']=gen('SingleMuPt10_cfi',K25by250)
steps['SingleMuPt100']=gen('SingleMuPt100_cfi',K9by250)
steps['SingleMuPt1000']=gen('SingleMuPt1000_cfi',K9by100)
steps['TTbar']=gen('TTbar_Tauola_8TeV_cfi',K9by50)
steps['TTbarLepton']=gen('TTbarLepton_Tauola_8TeV_cfi',K9by100)
steps['ZEE']=gen('ZEE_8TeV_cfi',K9by100)
steps['Wjet_Pt_80_120']=gen('Wjet_Pt_80_120_8TeV_cfi',K9by100)
steps['Wjet_Pt_3000_3500']=gen('Wjet_Pt_3000_3500_8TeV_cfi',K9by100)
steps['LM1_sfts']=gen('LM1_sfts_8TeV_cfi',K9by100)
steps['QCD_FlatPt_15_3000']=gen('QCDForPF_8TeV_cfi',K9by100)
steps['QCD_FlatPt_15_3000HS']=gen('QCDForPF_8TeV_cfi',K50by100)

baseDataSetRelease=['CMSSW_5_2_1-START52_V4-v1','CMSSW_5_2_1-STARTHI52_V4-v1','CMSSW_5_2_1-PU_START52_V4-v1']

steps['MinBiasINPUT']={'INPUT':InputInfo(dataSet='/RelValMinBias/%s/GEN-SIM'%(baseDataSetRelease[0],),location='STD')}
steps['QCD_Pt_3000_3500INPUT']={'INPUT':InputInfo(dataSet='/RelValQCD_Pt_3000_3500/%s/GEN-SIM'%(baseDataSetRelease[0],),location='STD')}
steps['QCD_Pt_80_120INPUT']={'INPUT':InputInfo(dataSet='/RelValQCD_Pt_80_120/%s/GEN-SIM'%(baseDataSetRelease[0],),location='STD')}
steps['SingleElectronPt10INPUT']={'INPUT':InputInfo(dataSet='/RelValSingleElectronPt10/%s/GEN-SIM'%(baseDataSetRelease[0],),location='STD')}
steps['SingleElectronPt35INPUT']={'INPUT':InputInfo(dataSet='/RelValSingleElectronPt35/%s/GEN-SIM'%(baseDataSetRelease[0],),location='STD')}
steps['SingleGammaPt10INPUT']={'INPUT':InputInfo(dataSet='/RelValSingleGammaPt10/%s/GEN-SIM'%(baseDataSetRelease[0],),location='STD')}
steps['SingleGammaPt35INPUT']={'INPUT':InputInfo(dataSet='/RelValSingleGammaPt35/%s/GEN-SIM'%(baseDataSetRelease[0],),location='STD')}
steps['SingleMuPt10INPUT']={'INPUT':InputInfo(dataSet='/RelValSingleMuPt10/%s/GEN-SIM'%(baseDataSetRelease[0],),location='STD')}
steps['SingleMuPt100INPUT']={'INPUT':InputInfo(dataSet='/RelValSingleMuPt100/%s/GEN-SIM'%(baseDataSetRelease[0],),location='STD')}
steps['SingleMuPt1000INPUT']={'INPUT':InputInfo(dataSet='/RelValSingleMuPt1000/%s/GEN-SIM'%(baseDataSetRelease[0],),location='STD')}
steps['TTbarINPUT']={'INPUT':InputInfo(dataSet='/RelValTTbar/%s/GEN-SIM'%(baseDataSetRelease[0],),location='STD')}
steps['TTbarLeptonINPUT']={'INPUT':InputInfo(dataSet='/RelValTTbarLepton/%s/GEN-SIM'%(baseDataSetRelease[0],),location='STD')}
steps['OldTTbarINPUT']={'INPUT':InputInfo(dataSet='/RelValProdTTbar/CMSSW_5_0_0_pre6-START50_V5-v1/GEN-SIM-RECO',location='STD')}
steps['OldGenSimINPUT']={'INPUT':InputInfo(dataSet='/RelValTTbar/CMSSW_4_4_2-START44_V7-v1/GEN-SIM-DIGI-RAW-HLTDEBUG',location='STD')}
steps['Wjet_Pt_80_120INPUT']={'INPUT':InputInfo(dataSet='/RelValWjet_Pt_80_120/%s/GEN-SIM'%(baseDataSetRelease[0],),location='STD')}
steps['Wjet_Pt_3000_3500INPUT']={'INPUT':InputInfo(dataSet='/RelValWjet_Pt_3000_3500/%s/GEN-SIM'%(baseDataSetRelease[0],),location='STD')}
steps['LM1_sftsINPUT']={'INPUT':InputInfo(dataSet='/RelValLM1_sfts/%s/GEN-SIM'%(baseDataSetRelease[0],),location='STD')}
steps['QCD_FlatPt_15_3000INPUT']={'INPUT':InputInfo(dataSet='/RelValQCD_FlatPt_15_3000/%s/GEN-SIM'%(baseDataSetRelease[0],),location='STD')}

steps['QCD_FlatPt_15_3000HSINPUT']={'INPUT':InputInfo(dataSet='/RelValQCD_FlatPt_15_3000/CMSSW_5_2_2-PU_START52_V4_special_120326-v1/GEN-SIM',location='STD')}
steps['QCD_FlatPt_15_3000HS__DIGIPU1INPUT']={'INPUT':InputInfo(dataSet='/RelValQCD_FlatPt_15_3000/CMSSW_5_2_2-PU_START52_V4_special_120326-v1/GEN-SIM-DIGI-RAW-HLTDEBUG',location='STD')}
steps['TTbar__DIGIPU1INPUT']={'INPUT':InputInfo(dataSet='/RelValTTbar/CMSSW_5_2_2-PU_START52_V4_special_120326-v1/GEN-SIM-DIGI-RAW-HLTDEBUG',location='STD')}

## high stat step1
K700by280={'--relval': '70000,280'}
K250by100={'--relval': '25000,100'}
K3250000by1300000={'--relval': '325000000,1300000'}
K250by250={'--relval': '25000,250'}
K110000by45000={'--relval': '11000000,45000'}
K562by225={'--relval': '56250,225'}

ecalHcal={
    '-s':'GEN,SIM,DIGI,DIGI2RAW,RAW2DIGI,L1Reco,RECO',
    '--datatier':'GEN-SIM-DIGI-RAW-RECO',
    #'--geometry':'ECALHCAL',
    '--eventcontent':'FEVTDEBUG',
    '--customise':'Validation/Configuration/ECALHCAL.customise',
    '--beamspot':'NoSmear'}
steps['SingleElectronE120EHCAL']=merge([{'cfg':'SingleElectronE120EHCAL_cfi'},ecalHcal,K25by250,step1Defaults])
steps['SinglePiE50HCAL']=merge([{'cfg':'SinglePiE50HCAL_cfi'},ecalHcal,K25by250,step1Defaults])

steps['MinBiasHS']=gen('MinBias_8TeV_cfi',K25by250)
steps['InclusiveppMuX']=gen('InclusiveppMuX_8TeV_cfi',K110000by45000)
steps['SingleElectronFlatPt5To100']=gen('SingleElectronFlatPt5To100_cfi',K250by250)
steps['SinglePiPt1']=gen('SinglePiPt1_cfi',K250by250)
steps['SingleMuPt1']=gen('SingleMuPt1_cfi',K250by250)
steps['ZPrime5000Dijet']=gen('ZPrime5000JJ_8TeV_cfi',K250by100)
steps['SinglePi0E10']=gen('SinglePi0E10_cfi',K250by100)
steps['SinglePiPt10']=gen('SinglePiPt10_cfi',K250by250)
steps['SingleGammaFlatPt10To100']=gen('SingleGammaFlatPt10To100_cfi',K250by250)
steps['SingleTauPt50Pythia']=gen('SingleTaupt_50_cfi',K250by100)
steps['SinglePiPt100']=gen('SinglePiPt100_cfi',K250by250)


def genS(fragment,howMuch):
    global step1Defaults,stCond
    return merge([{'cfg':fragment},stCond,howMuch,step1Defaults])

##steps['MinBias2']=genS('MinBias_8TeV_cfi',K9by100)
steps['Higgs200ChargedTaus']=genS('H200ChargedTaus_Tauola_8TeV_cfi',K9by100)
##steps['QCD_Pt_3000_3500_2']=genS('QCD_Pt_3000_3500_8TeV_cfi',K9by25)
##steps['QCD_Pt_80_120_2']=genS('QCD_Pt_80_120_8TeV_cfi',K9by50)
steps['JpsiMM']=genS('JpsiMM_8TeV_cfi',{'--relval':'65250,725'})
##steps['TTbar2']=genS('TTbar_Tauola_8TeV_cfi',K9by50)
steps['WE']=genS('WE_8TeV_cfi',K9by100)
steps['WM']=genS('WM_8TeV_cfi',K9by100)
##steps['ZEE2']=genS('ZEE_8TeV_cfi',K9by100)
steps['ZMM']=genS('ZMM_8TeV_cfi',{'--relval':'18000,200'})
steps['ZTT']=genS('ZTT_Tauola_All_hadronic_8TeV_cfi',K9by100)
steps['H130GGgluonfusion']=genS('H130GGgluonfusion_8TeV_cfi',K9by100)
steps['PhotonJets_Pt_10']=genS('PhotonJet_Pt_10_8TeV_cfi',K9by100)
steps['QQH1352T_Tauola']=genS('QQH1352T_Tauola_8TeV_cfi',K9by100)
steps['ZmumuJets_Pt_20_300']=gen('ZmumuJets_Pt_20_300_GEN_8TeV_cfg',K250by100)

steps['MinBias2INPUT']={'INPUT':InputInfo(dataSet='/RelValMinBias/%s/GEN-SIM'%(baseDataSetRelease[0],),location='STD')}
steps['Higgs200ChargedTausINPUT']={'INPUT':InputInfo(dataSet='/RelValHiggs200ChargedTaus/%s/GEN-SIM'%(baseDataSetRelease[0],),location='STD')}
steps['QCD_Pt_3000_3500_2INPUT']={'INPUT':InputInfo(dataSet='/RelValQCD_Pt_3000_3500/%s/GEN-SIM'%(baseDataSetRelease[0],),location='STD')}
steps['QCD_Pt_80_120_2INPUT']={'INPUT':InputInfo(dataSet='/RelValQCD_Pt_80_120/%s/GEN-SIM'%(baseDataSetRelease[0],),location='STD')}
steps['JpsiMMINPUT']={'INPUT':InputInfo(dataSet='/RelValJpsiMM/%s/GEN-SIM'%(baseDataSetRelease[0],),location='STD')}
steps['TTbar2INPUT']={'INPUT':InputInfo(dataSet='/RelValTTbar/%s/GEN-SIM'%(baseDataSetRelease[0],),location='STD')}
steps['WEINPUT']={'INPUT':InputInfo(dataSet='/RelValWE/%s/GEN-SIM'%(baseDataSetRelease[0],),location='STD')}
steps['WMINPUT']={'INPUT':InputInfo(dataSet='/RelValWM/%s/GEN-SIM'%(baseDataSetRelease[0],),location='STD')}
steps['ZEEINPUT']={'INPUT':InputInfo(dataSet='/RelValZEE/%s/GEN-SIM'%(baseDataSetRelease[0],),location='STD')}
steps['ZMMINPUT']={'INPUT':InputInfo(dataSet='/RelValZMM/%s/GEN-SIM'%(baseDataSetRelease[0],),location='STD')}
steps['ZTTINPUT']={'INPUT':InputInfo(dataSet='/RelValZTT/%s/GEN-SIM'%(baseDataSetRelease[0],),location='STD')}
steps['H130GGgluonfusionINPUT']={'INPUT':InputInfo(dataSet='/RelValH130GGgluonfusion/%s/GEN-SIM'%(baseDataSetRelease[0],),location='STD')}
steps['PhotonJets_Pt_10INPUT']={'INPUT':InputInfo(dataSet='/RelValPhotonJets_Pt_10/%s/GEN-SIM'%(baseDataSetRelease[0],),location='STD')}
steps['QQH1352T_TauolaINPUT']={'INPUT':InputInfo(dataSet='/RelValQQH1352T_Tauola/%s/GEN-SIM'%(baseDataSetRelease[0],),location='STD')}

steps['ZmumuJets_Pt_20_300INPUT']={'INPUT':InputInfo(dataSet='/RelValZmumuJets_Pt_20_300/%s/GEN-SIM'%(baseDataSetRelease[2],),location='STD')}

steps['Cosmics']=merge([{'cfg':'UndergroundCosmicMu_cfi.py','--relval':'666000,3000','--scenario':'cosmics'},step1Defaults])
steps['BeamHalo']=merge([{'cfg':'BeamHalo_cfi.py','--scenario':'cosmics'},K9by100,step1Defaults])

steps['CosmicsINPUT']={'INPUT':InputInfo(dataSet='/RelValCosmics/%s/GEN-SIM'%(baseDataSetRelease[0],),location='STD')}
steps['BeamHaloINPUT']={'INPUT':InputInfo(dataSet='/RelValBeamHalo/%s/GEN-SIM'%(baseDataSetRelease[0],),location='STD')}

steps['QCD_Pt_50_80']=genS('QCD_Pt_50_80_8TeV_cfi',K250by100)
steps['QCD_Pt_15_20']=genS('QCD_Pt_15_20_8TeV_cfi',K250by100)
steps['ZTTHS']=merge([K250by100,steps['ZTT']])
steps['QQH120Inv']=genS('QQH120Inv_8TeV_cfi',K250by100)
steps['TTbar2HS']=merge([K250by100,steps['TTbar']])
steps['JpsiMM_Pt_20_inf']=genS('JpsiMM_Pt_20_inf_8TeV_cfi',K700by280)
steps['QCD_Pt_120_170']=genS('QCD_Pt_120_170_8TeV_cfi',K250by100)
steps['H165WW2L']=genS('H165WW2L_Tauola_8TeV_cfi',K250by100)
steps['UpsMM']=genS('UpsMM_8TeV_cfi',K562by225)
steps['RSGrav']=genS('RS750_quarks_and_leptons_8TeV_cff',K250by100)
steps['QCD_Pt_80_120_2HS']=merge([K250by100,steps['QCD_Pt_80_120']])
steps['bJpsiX']=genS('bJpsiX_8TeV_cfi',K3250000by1300000)
steps['QCD_Pt_30_50']=genS('QCD_Pt_30_50_8TeV_cfi',K250by100)
steps['H200ZZ4L']=genS('H200ZZ4L_Tauola_8TeV_cfi',K250by100)
steps['LM9p']=genS('LM9p_8TeV_cff',K250by100)
steps['QCD_Pt_20_30']=genS('QCD_Pt_20_30_8TeV_cfi',K250by100)
steps['QCD_Pt_170_230']=genS('QCD_Pt_170_230_8TeV_cfi',K250by100)

## heavy ions tests
U500by5={'--relval': '500,5'}
U80by2={'--relval': '80,2'}
hiDefaults={'--conditions':'auto:starthi',
           '--scenario':'HeavyIons'}

steps['HydjetQ_MinBias_2760GeV']=merge([{'-n':1},hiDefaults,genS('Hydjet_Quenched_MinBias_2760GeV_cfi',U500by5)])
steps['HydjetQ_MinBias_2760GeVINPUT']={'INPUT':InputInfo(dataSet='/RelValHydjetQ_MinBias_2760GeV/%s/GEN-SIM'%(baseDataSetRelease[1],),location='STD')}
steps['HydjetQ_B0_2760GeV']=merge([{'-n':1},hiDefaults,genS('Hydjet_Quenched_B0_2760GeV_cfi',U80by2)])
steps['HydjetQ_B0_2760GeVINPUT']={'INPUT':InputInfo(dataSet='/RelValHydjetQ_B0_2760GeV/%s/GEN-SIM'%(baseDataSetRelease[1],),location='STD')}
steps['HydjetQ_B8_2760GeV']=merge([{'-n':1},hiDefaults,genS('Hydjet_Quenched_B8_2760GeV_cfi',U80by2)])
#leave it out to have Hydjet ran in IB test
#steps['HydjetQ_B8_2760GeVINPUT']={'INPUT':InputInfo(dataSet='/RelValHydjetQ_B0_2760GeV/%s/GEN-SIM'%(baseDataSetRelease[1],),location='STD')}



def changeRefRelease(step1s,listOfPairs):
    for s in step1s:
        if ('INPUT' in step1s[s]):
            oldD=steps[s]['INPUT'].dataSet
            for (ref,newRef) in listOfPairs:
                if  ref in oldD:
                    steps[s]['INPUT'].dataSet=oldD.replace(ref,newRef)
                                        
def addForAll(steps,d):
    for s in steps:
        steps[s].update(d)



#### fastsim section ####
##no forseen to do things in two steps GEN-SIM then FASTIM->end: maybe later
step1FastDefaults =merge([{'-s':'GEN,FASTSIM,HLT:GRun,VALIDATION',
                           '--eventcontent':'FEVTDEBUGHLT,DQM',
                           '--datatier':'GEN-SIM-DIGI-RECO,DQM',
                           '--relval':'27000,500'},
                          step1Defaults])
K100by500={'--relval':'100000,500'}
steps['TTbarFS']=merge([{'cfg':'TTbar_Tauola_8TeV_cfi'},K100by500,step1FastDefaults])
#steps['TTbarFS2']=merge([{'cfg':'TTbar_Tauola_8TeV_cfi'},K100by500,stCond,step1FastDefaults])
steps['SingleMuPt1FS']=merge([{'cfg':'SingleMuPt1_cfi'},step1FastDefaults])
steps['SingleMuPt10FS']=merge([{'cfg':'SingleMuPt10_cfi'},step1FastDefaults])
steps['SingleMuPt100FS']=merge([{'cfg':'SingleMuPt100_cfi'},step1FastDefaults])
steps['SinglePiPt1FS']=merge([{'cfg':'SinglePiPt1_cfi'},step1FastDefaults])
steps['SinglePiPt10FS']=merge([{'cfg':'SinglePiPt10_cfi'},step1FastDefaults])
steps['SinglePiPt100FS']=merge([{'cfg':'SinglePiPt100_cfi'},step1FastDefaults])
steps['ZEEFS']=merge([{'cfg':'ZEE_8TeV_cfi'},K100by500,step1FastDefaults])
#steps['ZEEFS2']=merge([{'cfg':'ZEE_8TeV_cfi'},K100by500,stCond,step1FastDefaults])
steps['ZTTFS']=merge([{'cfg':'ZTT_Tauola_OneLepton_OtherHadrons_8TeV_cfi'},K100by500,step1FastDefaults])
#steps['ZTTFS2']=merge([{'cfg':'ZTT_Tauola_OneLepton_OtherHadrons_8TeV_cfi'},K100by500,stCond,step1FastDefaults])
steps['QCDFlatPt153000FS']=merge([{'cfg':'QCDForPF_8TeV_cfi'},step1FastDefaults])
steps['QCD_Pt_80_120FS']=merge([{'cfg':'QCD_Pt_80_120_8TeV_cfi'},K100by500,stCond,step1FastDefaults])
steps['QCD_Pt_3000_3500FS']=merge([{'cfg':'QCD_Pt_3000_3500_8TeV_cfi'},K100by500,stCond,step1FastDefaults])
steps['H130GGgluonfusionFS']=merge([{'cfg':'H130GGgluonfusion_8TeV_cfi'},step1FastDefaults])
steps['SingleGammaFlatPt10To10FS']=merge([{'cfg':'SingleGammaFlatPt10To100_cfi'},K100by500,step1FastDefaults])

#### generator test section ####
step1GenDefaults=merge([{'-s':'GEN,VALIDATION:genvalid',
                         '--relval':'1000000,20000',
                         '--eventcontent':'RAWSIM',
                         '--datatier':'GEN'},
                        step1Defaults])
def genvalid(fragment,d,suffix='',fi=''):
    import copy
    c=copy.copy(d)
    if suffix:
        c['-s']=c['-s'].replace('genvalid','genvalid_'+suffix)
    if fi:
        c['--filein']='lhe:%d'%(fi,)
    c['cfg']=fragment
    return c
    
steps['QCD_Pt-30_8TeV_herwigpp']=genvalid('QCD_Pt_30_8TeV_herwigpp_cff',step1GenDefaults,'qcd')
steps['DYToLL_M-50_TuneZ2star_8TeV_pythia6-tauola']=genvalid('DYToLL_M_50_TuneZ2star_8TeV_pythia6_tauola_cff',step1GenDefaults,'dy')
steps['QCD_Pt-30_TuneZ2star_8TeV_pythia6']=genvalid('QCD_Pt_30_TuneZ2star_8TeV_pythia6_cff',step1GenDefaults,'qcd')
steps['QCD_Pt-30_8TeV_pythia8']=genvalid('QCD_Pt_30_8TeV_pythia8_cff',step1GenDefaults,'qcd')
steps['GluGluTo2Jets_M-100_8TeV_exhume']=genvalid('GluGluTo2Jets_M_100_8TeV_exhume_cff',step1GenDefaults,'qcd')
steps['TT_TuneZ2star_8TeV_pythia6-evtgen']=genvalid('TT_TuneZ2star_8TeV_pythia6_evtgen_cff',step1GenDefaults)
steps['MinBias_TuneZ2star_8TeV_pythia6']=genvalid('MinBias_TuneZ2star_8TeV_pythia6_cff',step1GenDefaults,'qcd')
steps['WToLNu_TuneZ2star_8TeV_pythia6-tauola']=genvalid('WToLNu_TuneZ2star_8TeV_pythia6_tauola_cff',step1GenDefaults,'w')
steps['QCD_Pt-30_8TeV_herwig6']=genvalid('QCD_Pt_30_8TeV_herwig6_cff',step1GenDefaults,'qcd')
steps['MinBias_8TeV_pythia8']=genvalid('MinBias_8TeV_pythia8_cff',step1GenDefaults,'qcd')


steps['QCD_Ht-100To250_TuneZ2star_8TeV_madgraph-tauola']=genvalid('Hadronizer_MgmMatchTuneZ2star_8TeV_madgraph_tauola_cff',step1GenDefaults,'qcd',5475)
steps['QCD_Ht-250To500_TuneZ2star_8TeV_madgraph-tauola']=genvalid('Hadronizer_MgmMatchTuneZ2star_8TeV_madgraph_tauola_cff',step1GenDefaults,'qcd',5476)
steps['QCD_Ht-500To1000_TuneZ2star_8TeV_madgraph-tauola']=genvalid('Hadronizer_MgmMatchTuneZ2star_8TeV_madgraph_tauola_cff',step1GenDefaults,'qcd',5481)
steps['TTJets_TuneZ2star_8TeV_madgraph-tauola']=genvalid('Hadronizer_MgmMatchTuneZ2star_8TeV_madgraph_tauola_cff',step1GenDefaults,'',5502)
steps['WJetsLNu_TuneZ2star_8TeV_madgraph-tauola']=genvalid('Hadronizer_MgmMatchTuneZ2star_8TeV_madgraph_tauola_cff',step1GenDefaults,'w',5607)
steps['ZJetsLNu_TuneZ2star_8TeV_madgraph-tauola']=genvalid('Hadronizer_MgmMatchTuneZ2star_8TeV_madgraph_tauola_cff',step1GenDefaults,'dy',5591)
steps['ZJetsLNu_Tune4C_8TeV_madgraph-pythia8']=genvalid('Hadronizer_MgmMatchTune4C_8TeV_madgraph_pythia8_cff',step1GenDefaults,'dy',5591)

PU={'--pileup':'default','--pileup_input':'dbs:/RelValProdMinBias/%s/GEN-SIM-RAW'%(baseDataSetRelease[0],)}
PUFS={'--pileup':'default'}
steps['TTbarFSPU']=merge([PUFS,steps['TTbarFS']])
##########################



# step2 
step2Defaults = { 
                  '-s'            : 'DIGI,L1,DIGI2RAW,HLT,RAW2DIGI,L1Reco',
                  '--datatier'    : 'GEN-SIM-DIGI-RAW-HLTDEBUG',
                  '--eventcontent': 'FEVTDEBUGHLT',
                  '--conditions'  : 'auto:startup',
                  }


steps['DIGIPROD1']=merge([{'--eventcontent':'RAWSIM','--datatier':'GEN-SIM-RAW'},step2Defaults])
steps['DIGI']=merge([step2Defaults])
#steps['DIGI2']=merge([stCond,step2Defaults])
steps['DIGICOS']=merge([{'--scenario':'cosmics','--eventcontent':'FEVTDEBUG','--datatier':'GEN-SIM-DIGI-RAW'},stCond,step2Defaults])

steps['DIGIPU1']=merge([step2Defaults,PU])
steps['REDIGIPU']=merge([{'-s':'reGEN,reDIGI,L1,DIGI2RAW,HLT,RAW2DIGI,L1Reco'},steps['DIGIPU1']])


steps['RESIM']=merge([{'-s':'reGEN,reSIM','-n':10},steps['DIGI']])
steps['RESIMDIGI']=merge([{'-s':'reGEN,reSIM,DIGI,L1,DIGI2RAW,HLT,RAW2DIGI,L1Reco','-n':10,'--restoreRNDSeeds':'','--process':'HLT'},steps['DIGI']])

    
steps['DIGIHI']=merge([{'--inputCommands':'"keep *","drop *_simEcalPreshowerDigis_*_*"','-n':10},hiDefaults,step2Defaults])

#add this line when testing from an input file that is not strictly GEN-SIM
#addForAll(step2,{'--process':'DIGI'})

dataReco={'--conditions':'auto:com10',
          '-s':'RAW2DIGI,L1Reco,RECO,ALCA:SiStripCalZeroBias+SiStripCalMinBias+TkAlMinBias,DQM',
          '--datatier':'RECO,DQMROOT',
          '--eventcontent':'RECO,DQMROOT',
          '--data':'',
          '--process':'reRECO',
          '--scenario':'pp',
          }

steps['HLTD']=merge([{'--process':'reHLT',
                      '-s':'L1REPACK,HLT',
                      '--condition':'auto:hltonline11',
                      '--data':'',
                      '--output':'\'[{"e":"RAW","t":"RAW","o":["drop FEDRawDataCollection_rawDataCollector__LHC"]}]\'',
                      },])
steps['RECOD']=merge([{'--scenario':'pp',},dataReco])
steps['RECOSKIMALCA']=merge([{'--inputCommands':'"keep *","drop *_*_*_RECO"'
                              },steps['RECOD']])
steps['RECOSKIM']=merge([{'-s':'RAW2DIGI,L1Reco,RECO,DQM',
                          },steps['RECOSKIMALCA']])

steps['REPACKHID']=merge([{'--scenario':'HeavyIons',
                         '-s':'RAW2DIGI,REPACK',
                         '--datatier':'RAW',
                         '--eventcontent':'REPACKRAW'},
                        steps['RECOD']])
steps['RECOHID10']=merge([{'--scenario':'HeavyIons',
                         '-s':'RAW2DIGI,L1Reco,RECO,ALCA:SiStripCalZeroBias+SiStripCalMinBias+TkAlMinBiasHI+HcalCalMinBias,DQM',
                         '--datatier':'RECO,DQMROOT',
                         '--eventcontent':'RECO,DQMROOT'},
                        steps['RECOD']])
steps['RECOHID11']=merge([{'--repacked':''},
                          steps['RECOHID10']])
steps['RECOHID10']['-s']+=',REPACK'
steps['RECOHID10']['--datatier']+=',RAW'
steps['RECOHID10']['--eventcontent']+=',REPACKRAW'

steps['TIER0']=merge([{'--customise':'Configuration/DataProcessing/RecoTLR.customisePrompt',
                       '-s':'RAW2DIGI,L1Reco,RECO,ALCAPRODUCER:@allForPrompt,DQM,ENDJOB',
                       '--datatier':'RECO,AOD,ALCARECO,DQMROOT',
                       '--eventcontent':'RECO,AOD,ALCARECO,DQMROOT',
                       '--process':'RECO'
                       },dataReco])
steps['TIER0EXP']=merge([{'-s':'RAW2DIGI,L1Reco,RECO,ALCAPRODUCER:@allForExpress,DQM,ENDJOB',
                          '--datatier':'ALCARECO,DQM',
                          '--eventcontent':'ALCARECO,DQM',
                          '--customise':'Configuration/DataProcessing/RecoTLR.customiseExpress',
                          },steps['TIER0']])

steps['RECOCOSD']=merge([{'--scenario':'cosmics',
                          '-s':'RAW2DIGI,L1Reco,RECO,DQM,ALCA:MuAlCalIsolatedMu+DtCalib',
                          '--customise':'Configuration/DataProcessing/RecoTLR.customiseCosmicData'
                          },dataReco])

step2HImixDefaults=merge([{'-n':'10',
                           '--himix':'',
                           '--filein':'file.root',
                           '--process':'HISIGNAL'
                           },hiDefaults,step1Defaults])
steps['Pyquen_GammaJet_pt20_2760GeV']=merge([{'cfg':'Pyquen_GammaJet_pt20_2760GeV_cfi'},step2HImixDefaults])
steps['Pyquen_DiJet_pt80to120_2760GeV']=merge([{'cfg':'Pyquen_DiJet_pt80to120_2760GeV_cfi'},step2HImixDefaults])
steps['Pyquen_ZeemumuJets_pt10_2760GeV']=merge([{'cfg':'Pyquen_ZeemumuJets_pt10_2760GeV_cfi'},step2HImixDefaults])

# step3 
step3Defaults = {
                  '-s'            : 'RAW2DIGI,L1Reco,RECO,VALIDATION,DQM',
                  '--conditions'  : 'auto:startup',
                  '--no_exec'     : '',
                  '--datatier'    : 'GEN-SIM-RECO,DQM',
                  '--eventcontent': 'RECOSIM,DQM'
                  }

steps['DIGIPU']=merge([{'--process':'REDIGI'},steps['DIGIPU1']])

steps['RECODst3']=merge([{'--hltProcess':'reHLT'},steps['RECOD']])
steps['RECO']=merge([step3Defaults])
steps['RECODBG']=merge([{'--eventcontent':'RECODEBUG,DQM'},steps['RECO']])
steps['RECOPROD1']=merge([{ '-s' : 'RAW2DIGI,L1Reco,RECO', '--datatier' : 'GEN-SIM-RECO,AODSIM', '--eventcontent' : 'RECOSIM,AODSIM'},step3Defaults])
steps['RECOCOS']=merge([{'-s':'RAW2DIGI,L1Reco,RECO,ALCA:MuAlCalIsolatedMu,DQM','--scenario':'cosmics'},stCond,step3Defaults])
steps['RECOMIN']=merge([{'-s':'RAW2DIGI,L1Reco,RECO,ALCA:SiStripCalZeroBias+SiStripCalMinBias+EcalCalPhiSym+EcalCalPi0Calib+EcalCalEtaCalib,VALIDATION,DQM'},stCond,step3Defaults])

steps['RECOPU1']=merge([steps['RECO'],PU])
steps['RECOPUDBG']=merge([{'--eventcontent':'RECODEBUG,DQM'},steps['RECOPU1']])
steps['RERECOPU1']=merge([{'--hltProcess':'REDIGI'},steps['RECOPU1']])


steps['RECOHI']=merge([hiDefaults,step3Defaults])
steps['DIGIHISt3']=steps['DIGIHI']

steps['RECOHID11St3']=merge([{
                              '--process':'ZStoRECO'},
                             steps['RECOHID11']])
steps['RECOHIR10D11']=merge([{'--filein':'file:step2_inREPACKRAW.root',
                              '--filtername':'reRECO'},
                             steps['RECOHID11St3']])

#add this line when testing from an input file that is not strictly GEN-SIM
#addForAll(step3,{'--hltProcess':'DIGI'})

steps['ALCACOSD']={'--conditions':'auto:com10',
                   '--datatier':'ALCARECO',
                   '--eventcontent':'ALCARECO',
                   '--scenario':'cosmics',
                   '-s':'ALCA:TkAlCosmics0T+MuAlGlobalCosmics+HcalCalHOCosmics+DQM'
                   }
steps['ALCAPROMPT']={'-s':'ALCA:PromptCalibProd',
                     '--filein':'file:TkAlMinBias.root',
                     '--conditions':'auto:com10',
                     '--datatier':'ALCARECO',
                     '--eventcontent':'ALCARECO'}
steps['ALCAEXP']={'-s':'ALCA:PromptCalibProd',
                  '--conditions':'auto:com10',
                  '--datatier':'ALCARECO',
                  '--eventcontent':'ALCARECO'}

# step4
step4Defaults = { 
                  '-s'            : 'ALCA:TkAlMuonIsolated+TkAlMinBias+EcalCalElectron+HcalCalIsoTrk+MuAlOverlaps',
                  '-n'            : 1000,
                  '--conditions'  : 'auto:startup',
                  '--datatier'    : 'ALCARECO',
                  '--eventcontent': 'ALCARECO',
                  }

steps['RERECOPU']=steps['RERECOPU1']

steps['ALCATT']=merge([step4Defaults])
steps['ALCAMIN']=merge([{'-s':'ALCA:TkAlMinBias'},stCond,step4Defaults])
#steps['ALCAQCD']=merge([{'-s':'ALCA:HcalCalIsoTrk+HcalCalDijets'},stCond,step4Defaults])
#steps['ALCAMU']=merge([{'-s':'ALCA:@Mu'},stCond,step4Defaults])
steps['ALCACOS']=merge([{'-s':'ALCA:TkAlCosmics0T+MuAlGlobalCosmics+HcalCalHOCosmics'},stCond,step4Defaults])
steps['ALCABH']=merge([{'-s':'ALCA:TkAlBeamHalo+MuAlBeamHaloOverlaps+MuAlBeamHalo'},stCond,step4Defaults])
steps['ALCAELE']=merge([{'-s':'ALCA:EcalCalElectron'},stCond,step4Defaults])

steps['ALCAHARVD']={'-s':'ALCAHARVEST:BeamSpotByRun+BeamSpotByLumi+SiStripQuality',
                    '--conditions':'auto:com10',
                    '--scenario':'pp',
                    '--data':'',
                    '--filein':'file:PromptCalibProd.root'}

steps['RECOHISt4']=steps['RECOHI']

steps['ALCANZS']=merge([{'-s':'ALCA:HcalCalMinBias','--mc':''},step4Defaults])
steps['HARVGEN']={'-s':'HARVESTING:genHarvesting',
                  '--harvesting':'AtJobEnd',
                  '--conditions':'auto:startup',
                  '--mc':'',
                  '--filein':'file:step1.root'
                  }

#data
steps['HARVESTD']={'-s':'HARVESTING:dqmHarvesting',
                   '--conditions':'auto:com10',
                   '--filetype':'DQM',
                   '--data':'',
                   '--scenario':'pp'}
steps['HARVESTDfst2']=merge([{'--filein':'file:step2_inDQM.root'},steps['HARVESTD']])

steps['HARVESTDC']={'-s':'HARVESTING:dqmHarvesting',
                   '--conditions':'auto:com10',
                   '--filetype':'DQM',
                   '--data':'',
                    '--filein':'file:step2_inDQM.root',
                   '--scenario':'cosmics'}
steps['HARVESTDHI']={'-s':'HARVESTING:dqmHarvesting',
                   '--conditions':'auto:com10',
                   '--filetype':'DQM',
                   '--data':'',
                   '--scenario':'HeavyIons'}

#MC
steps['HARVEST']={'-s':'HARVESTING:validationHarvesting+dqmHarvesting',
                   '--conditions':'auto:startup',
                   '--mc':'',
                   '--scenario':'pp'}
steps['HARVESTCOS']={'-s':'HARVESTING:dqmHarvesting',
                     '--conditions':'auto:startup',
                     '--mc':'',
                     '--filein':'file:step3_inDQM.root',
                   '--scenario':'cosmics'}
steps['HARVESTFS']={'-s':'HARVESTING:validationHarvestingFS',
                   '--conditions':'auto:startup',
                   '--mc':'',
                   '--scenario':'pp'}
steps['HARVESTHI']={'-s':'HARVESTING:validationHarvesting+dqmHarvesting',
                   '--conditions':'auto:startup',
                   '--mc':'',
                   '--scenario':'HeavyIons'}

steps['ALCASPLIT']={'-s':'ALCAOUTPUT:@allForPrompt',
                    '--conditions':'auto:com10',
                    '--scenario':'pp',
                    '--data':'',
                    '--triggerResultsProcess':'RECO',
                    '--filein':'file:step2_inALCARECO.root'}

steps['SKIMD']={'-s':'SKIM:all',
                '--conditions':'auto:com10',
                '--data':'',
                '--scenario':'pp',
                '--filein':'file:step2.root',
                '--secondfilein':'filelist:step1_dbsquery.log'}


steps['SKIMCOSD']={'-s':'SKIM:all',
                   '--conditions':'auto:com10',
                   '--data':'',
                   '--scenario':'cosmics',
                   '--filein':'file:step2.root',
                   '--secondfilein':'filelist:step1_dbsquery.log'}
                 

#### for special wfs ###
#steps['TTbar_REDIGI_RERECO']=merge([{'cfg':'TTbar_Tauola_8TeV_cfi',
#                                     '-s':'GEN,SIM,DIGI,L1,DIGI2RAW,HLT:GRun,RAW2DIGI,L1Reco,RECO,ALCA:MuAlCalIsolatedMu+DtCalib,VALIDATION,DQM',
#                                     '--datatier':'GEN-SIM-DIGI-RAW-HLTDEBUG-RECO,DQM',
#                                     '--eventcontent':'FEVTDEBUGHLT,DQM'},
#                                    K9by50,stCond,step1Defaults])
#steps['DIGI2RECO']=merge([{'-s':'DIGI,L1,DIGI2RAW,HLT:GRun,RAW2DIGI,L1Reco,RECO,VALIDATION,DQM',
#                           '--filtername':'DIGItoRECO',
#                           '--process':'RECO',
#                           '--eventcontent':'RECOSIM,DQM',
#                           '--datatier':'GEN-SIM-RECO,DQM',
#                           },
#                            stCond,step3Defaults])
steps['RECOFROMRECO']=merge([{'-s':'RECO',
                              '--filtername':'RECOfromRECO',
                              '--process':'reRECO',
                              '--datatier':'AODSIM',
                              '--eventcontent':'AODSIM',
                              },
                             stCond,step3Defaults])


steps['RECOFROMRECOSt2']=steps['RECOFROMRECO']

steps['RECODFROMRAWRECO']=merge([{'-s':'RAW2DIGI:RawToDigi_noTk,L1Reco,RECO:reconstruction_noTracking',
                                  '--filtername':'RECOfromRAWRECO',
                                  '--process':'rereRECO',
                                  '--datatier':'AOD',
                                  '--eventcontent':'AOD',
                                  '--secondfilein':'filelist:step1_dbsquery.log',
                                  },
                                 steps['RECOD']])


### over write a few things to add PU to every sample in the standard set
#steps.overwrite('DIGI',steps['DIGIPU1'])
#steps.overwrite('RECO',steps['RECOPU1'])
#steps.overwrite(('DIGI','RESIMDIGI'))
