#!/bin/sh

#MYPATH="GeneratorInterface/ThePEGInterface/test"
MYPATH="Configuration/GenProduction"
CONDITIONS="FrontierConditions_GlobalTag,MC_31X_V9::All"

CMSOPTS="-n 100 --eventcontent RAWSIM --conditions $CONDITIONS $CMSOPTS --no_exec --mc --customise=$MYPATH/custom"

cmsDriver.py "$MYPATH/testThePEGGeneratorFilter" -s GEN --datatier GEN $CMSOPTS
cmsDriver.py "$MYPATH/testThePEGGeneratorFilter" -s GEN,SIM,DIGI,L1,DIGI2RAW,HLT --datatier GEN-SIM-RAW $CMSOPTS
cmsDriver.py "$MYPATH/testThePEGHadronisation"   -s GEN --datatier GEN $CMSOPTS
cmsDriver.py "$MYPATH/testThePEGHadronisation"   -s GEN,SIM,DIGI,L1,DIGI2RAW,HLT --datatier GEN-SIM-RAW $CMSOPTS
