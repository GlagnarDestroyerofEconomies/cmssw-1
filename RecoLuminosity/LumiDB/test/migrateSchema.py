#!/usr/bin/env python
VERSION='1.02'
import os,sys,array
import coral
from RecoLuminosity.LumiDB import argparse,idDealer,nameDealer,CommonUtil,lumidbDDL,dbUtil,dataDML,revisionDML
#
# data transfer section
#

def main():
    from RecoLuminosity.LumiDB import sessionManager,queryDataSource
    
    parser = argparse.ArgumentParser(prog=os.path.basename(sys.argv[0]),description="migrate lumidb schema",formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument('-c',dest='connect',action='store',required=False,default='oracle://cms_orcoff_prep/CMS_LUMI_DEV_OFFLINE',help='connect string to trigger DB(required)')
    parser.add_argument('-P',dest='authpath',action='store',required=False,default='/afs/cern.ch/user/x/xiezhen',help='path to authentication file')
    parser.add_argument('-r',dest='runnumber',action='store',required=True,help='run number')
    parser.add_argument('--debug',dest='debug',action='store_true',help='debug')
    args=parser.parse_args()
    runnumber=int(args.runnumber)
    #print 'processing run ',runnumber
    svc=sessionManager.sessionManager(args.connect,authpath=args.authpath,debugON=args.debug)
    session=svc.openSession(isReadOnly=False,cpp2sqltype=[('unsigned int','NUMBER(10)'),('unsigned long long','NUMBER(20)')])
    [bitnames,trglsdata]=queryDataSource.trgFromOldLumi(session,runnumber)
    session.transaction().start(False)
    schema=session.nominalSchema()
    lumidbDDL.newToOld(schema)
    lumidbDDL.oldToNew(schema)
    lumidbDDL.createUniqueConstraints(schema)
    (trunkrevid,trunkparentid,trunkparentname)=revisionDML.createBranch(schema,'TRUNK',None,comment='main')
    (datarevid,dataparentid,dataparentname)=revisionDML.createBranch(schema,'DATA','TRUNK',comment='hold data')
    (normrevid,normparentid,normparentname)=revisionDML.createBranch(schema,'NORM','TRUNK',comment='hold normalization factor')
    dataDML.addNormToBranch(schema,'pp7TeV','PROTPHYS',6370.0,3500,{},(normrevid,'NORM'))
    dataDML.addNormToBranch(schema,'hi7TeV','HIPHYS',2.38,3500,{},(normrevid,'NORM'))
    (lumirevid,lumientryid,lumidataid)=dataDML.addLumiRunDataToBranch(schema,runnumber,[args.connect],(datarevid,'DATA'))
    bitzeroname=bitnames.split(',')[0]
    trgrundata=['oracle://cms_oron_prod/cms_trg',bitzeroname,bitnames]
    (trgrevid,trgentryid,trgdataid)=dataDML.addTrgRunDataToBranch(schema,runnumber,trgrundata,(datarevid,'DATA'))
    dataDML.insertTrgLSData(schema,runnumber,trgdataid,trglsdata)
    [pathnames,hltlsdata]=queryDataSource.hltFromOldLumi(session,runnumber)
    hltrundata=[pathnames,'oracle://cms_orcon_prod/cms_runinfo']
    (hltrevid,hltentryid,hltdataid)=dataDML.addHLTRunDataToBranch(schema,runnumber,hltrundata,(datarevid,'DATA'))
    dataDML.insertHltLSData(schema,runnumber,hltdataid,hltlsdata)
    session.transaction().commit()
    del session
    del svc
if __name__=='__main__':
    main()
    
