import os
import time

localtime = time.localtime(time.time())

#print "Local current time :", localtime

from Configuration.Skimming.autoSkim import autoSkim

tier1dirname = 'tier1_%sy_%sm_%sd_%sh_%sm_%ss' %(localtime[0],localtime[1],localtime[2],localtime[3],localtime[4],localtime[5])
testdirname = 'test_%sy_%sm_%sd_%sh_%sm_%ss' %(localtime[0],localtime[1],localtime[2],localtime[3],localtime[4],localtime[5])
os.system( 'mkdir %s' %(tier1dirname) )
os.system( 'mkdir %s' %(testdirname) )

for k in autoSkim:
   #    print k
   #print ('cmsDriver.py skims -s SKIM:@%s --data --no_exec --dbs \"find file,file.parent where dataset=/%s/Run2012A-PromptReco-v1/RECO and run=190705\" -n 100 --conditions auto:com10 --python_filename=skim_%s.py' %(k,k,k))
   os.system('cmsDriver.py skims -s SKIM:@%s --data --no_exec --dbs \"find file,file.parent where dataset=/%s/Run2012A-PromptReco-v1/RECO and run=190705\" -n 100 --conditions auto:com10 --python_filename=skim_%s.py' %(k,k,k))
   os.system('mkdir -p %s/%s' %(testdirname,k))
   os.system('echo \"process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )\" >> skim_%s.py' %(k))
   os.system('cp skim_%s.py %s/%s' %(k,testdirname,k))
   os.system('mv skim_%s.py %s' %(k,tier1dirname))
   os.system('cd %s/%s ; cmsRun skim_%s.py > skim_%s.txt 2>&1' %(testdirname,k,k,k) )
   
   
