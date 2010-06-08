#include "GeometryTester.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"

#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/PluginManager/interface/ModuleDef.h"

#include "CondFormats/GeometryObjects/interface/GeometryFile.h"
#include "CondFormats/GeometryObjects/interface/PGeometricDet.h"
#include "CondFormats/GeometryObjects/interface/PGeometricDetExtra.h"
#include "CondFormats/GeometryObjects/interface/PCaloGeometry.h"
#include "CondFormats/GeometryObjects/interface/RecoIdealGeometry.h" 
#include "CondFormats/GeometryObjects/interface/CSCRecoDigiParameters.h"

#include "Geometry/Records/interface/GeometryFileRcd.h"    
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "Geometry/Records/interface/PGeometricDetExtraRcd.h"
#include "Geometry/Records/interface/DTRecoGeometryRcd.h"
#include "Geometry/Records/interface/RPCRecoGeometryRcd.h"
#include "Geometry/Records/interface/CSCRecoGeometryRcd.h"
#include "Geometry/Records/interface/CSCRecoDigiParametersRcd.h"
#include "Geometry/Records/interface/PEcalBarrelRcd.h"
#include "Geometry/Records/interface/PEcalEndcapRcd.h"
#include "Geometry/Records/interface/PEcalPreshowerRcd.h"
#include "Geometry/Records/interface/PHcalRcd.h"
#include "Geometry/Records/interface/PCaloTowerRcd.h"
#include "Geometry/Records/interface/PCastorRcd.h"
#include "Geometry/Records/interface/PZdcRcd.h"

#include <iostream>
#include <string>
#include <vector>

GeometryTester::GeometryTester(const edm::ParameterSet& iConfig)
{
  xmltest=iConfig.getUntrackedParameter<bool>("XMLTest", true);
  tktest=iConfig.getUntrackedParameter<bool>("TrackerTest", true);
  ecaltest=iConfig.getUntrackedParameter<bool>("EcalTest", true);
  hcaltest=iConfig.getUntrackedParameter<bool>("HcalTest", true);
  calotowertest=iConfig.getUntrackedParameter<bool>("CaloTowerTest", true);
  castortest=iConfig.getUntrackedParameter<bool>("CastorTest", true);
  zdctest=iConfig.getUntrackedParameter<bool>("ZDCTest", true);
  csctest=iConfig.getUntrackedParameter<bool>("CSCTest", true);
  dttest=iConfig.getUntrackedParameter<bool>("DTTest", true);
  rpctest=iConfig.getUntrackedParameter<bool>("RPCTest", true);
  geomLabel_ = iConfig.getUntrackedParameter<std::string>("geomLabel", "Extended");
}

GeometryTester::~GeometryTester(){
}

void
GeometryTester::analyze( const edm::Event& iEvent, const edm::EventSetup& iSetup) 
{

  if(xmltest){
    edm::ESHandle<GeometryFile> xmlgeo;
    iSetup.get<GeometryFileRcd>().get(geomLabel_, xmlgeo);
    std::cout<<"XML FILE"<<std::endl;
    std::vector<unsigned char>* tb = (*xmlgeo).getUncompressedBlob();
    std::cout<<"SIZE FILE = "<<tb->size()<<std::endl;
    for(uint32_t i=0; i<tb->size();i++){
      std::cout<<(*tb)[i];
    }
    std::cout<<""<<std::endl;
  }

  
  if(tktest){
    edm::ESHandle<PGeometricDet> tkgeo;
    edm::ESHandle<PGeometricDetExtra> tkExtra;
    iSetup.get<IdealGeometryRecord>().get(tkgeo);
    iSetup.get<PGeometricDetExtraRcd>().get(tkExtra);
    std::cout<<"TRACKER"<<std::endl;
    //helper map
    std::map<uint32_t, uint32_t> diTogde;
    for (uint32_t g=0; g<tkExtra->pgdes_.size();++g) {
      diTogde[tkExtra->pgdes_[g]._geographicalId] = g;
    }
    uint32_t tkeInd;
    for(uint32_t i=0; i<tkgeo->pgeomdets_.size();i++){
      std::cout<<tkgeo->pgeomdets_[i]._params0<<tkgeo->pgeomdets_[i]._params1<<tkgeo->pgeomdets_[i]._params2<<tkgeo->pgeomdets_[i]._params3<<tkgeo->pgeomdets_[i]._params4<<tkgeo->pgeomdets_[i]._params5<<tkgeo->pgeomdets_[i]._params6<<tkgeo->pgeomdets_[i]._params7<<tkgeo->pgeomdets_[i]._params8<<tkgeo->pgeomdets_[i]._params9<<tkgeo->pgeomdets_[i]._params10<<tkgeo->pgeomdets_[i]._x<<tkgeo->pgeomdets_[i]._y<<tkgeo->pgeomdets_[i]._z<<tkgeo->pgeomdets_[i]._phi<<tkgeo->pgeomdets_[i]._rho<<tkgeo->pgeomdets_[i]._a11<< tkgeo->pgeomdets_[i]._a12<< tkgeo->pgeomdets_[i]._a13<< tkgeo->pgeomdets_[i]._a21<< tkgeo->pgeomdets_[i]._a22<< tkgeo->pgeomdets_[i]._a23<< tkgeo->pgeomdets_[i]._a31<< tkgeo->pgeomdets_[i]._a32<< tkgeo->pgeomdets_[i]._a33<<tkgeo->pgeomdets_[i]._shape<<tkgeo->pgeomdets_[i]._name<< tkgeo->pgeomdets_[i]._ns;
      tkeInd = diTogde[tkgeo->pgeomdets_[i]._geographicalID];
      std::cout <<tkExtra->pgdes_[tkeInd]._volume<<tkExtra->pgdes_[tkeInd]._density<<tkExtra->pgdes_[tkeInd]._weight<<tkExtra->pgdes_[tkeInd]._copy<<tkExtra->pgdes_[tkeInd]._material;
      std::cout <<tkgeo->pgeomdets_[i]._radLength<<tkgeo->pgeomdets_[i]._xi<<tkgeo->pgeomdets_[i]._pixROCRows<<tkgeo->pgeomdets_[i]._pixROCCols<<tkgeo->pgeomdets_[i]._pixROCx<<tkgeo->pgeomdets_[i]._pixROCy<<tkgeo->pgeomdets_[i]._stereo<<tkgeo->pgeomdets_[i]._siliconAPVNum<<tkgeo->pgeomdets_[i]._geographicalID<<tkgeo->pgeomdets_[i]._nt0<<tkgeo->pgeomdets_[i]._nt1<<tkgeo->pgeomdets_[i]._nt2<<tkgeo->pgeomdets_[i]._nt3<<tkgeo->pgeomdets_[i]._nt4<<tkgeo->pgeomdets_[i]._nt5<<tkgeo->pgeomdets_[i]._nt6<<tkgeo->pgeomdets_[i]._nt7<<tkgeo->pgeomdets_[i]._nt8<<tkgeo->pgeomdets_[i]._nt9<<tkgeo->pgeomdets_[i]._nt10<<std::endl;
    }
  }
  
  if(ecaltest){
    edm::ESHandle<PCaloGeometry> ebgeo;
    iSetup.get<PEcalBarrelRcd>().get(ebgeo);
    std::cout<<"ECAL BARREL"<<std::endl;
    std::vector<double> tseb = ebgeo->getTranslation();
    std::vector<double> dimeb = ebgeo->getDimension();
    std::vector<uint32_t> indeb = ebgeo->getIndexes();
    for(uint32_t i=0;i<tseb.size();i++){
      std::cout<<tseb[i];
    } 
    std::cout<<""<<std::endl;
    for(uint32_t i=0;i<dimeb.size();i++){
      std::cout<<dimeb[i];
    } 
    std::cout<<""<<std::endl;
    for(uint32_t i=0;i<indeb.size();i++){
      std::cout<<indeb[i];
    } 
    std::cout<<""<<std::endl;

    edm::ESHandle<PCaloGeometry> eegeo;
    iSetup.get<PEcalEndcapRcd>().get(eegeo);
    std::cout<<"ECAL ENDCAP"<<std::endl;
    std::vector<double> tsee = eegeo->getTranslation();
    std::vector<double> dimee = eegeo->getDimension();
    std::vector<uint32_t> indee = eegeo->getIndexes();
    for(uint32_t i=0;i<tsee.size();i++){
      std::cout<<tsee[i];
    } 
    std::cout<<""<<std::endl;
    for(uint32_t i=0;i<dimee.size();i++){
      std::cout<<dimee[i];
    } 
    std::cout<<""<<std::endl;
    for(uint32_t i=0;i<indee.size();i++){
      std::cout<<indee[i];
    } 
    std::cout<<""<<std::endl;

    edm::ESHandle<PCaloGeometry> epgeo;
    iSetup.get<PEcalPreshowerRcd>().get(epgeo);
    std::cout<<"ECAL PRESHOWER"<<std::endl;
    std::vector<double> tsep = epgeo->getTranslation();
    std::vector<double> dimep = epgeo->getDimension();
    std::vector<uint32_t> indep = epgeo->getIndexes();
    for(uint32_t i=0;i<tsep.size();i++){
      std::cout<<tsep[i];
    } 
    std::cout<<""<<std::endl;
    for(uint32_t i=0;i<dimep.size();i++){
      std::cout<<dimep[i];
    } 
    std::cout<<""<<std::endl;
    for(uint32_t i=0;i<indep.size();i++){
      std::cout<<indep[i];
    } 
    std::cout<<""<<std::endl;

  }

  if(hcaltest){
    edm::ESHandle<PCaloGeometry> hgeo;
    iSetup.get<PHcalRcd>().get(hgeo);
    std::cout<<"HCAL"<<std::endl;
    std::vector<double> tsh = hgeo->getTranslation();
    std::vector<double> dimh = hgeo->getDimension();
    std::vector<uint32_t> indh = hgeo->getIndexes();
    for(uint32_t i=0;i<tsh.size();i++){
      std::cout<<tsh[i];
    } 
    std::cout<<""<<std::endl;
    for(uint32_t i=0;i<dimh.size();i++){
      std::cout<<dimh[i];
    } 
    std::cout<<""<<std::endl;
    for(uint32_t i=0;i<indh.size();i++){
      std::cout<<indh[i];
    } 
    std::cout<<""<<std::endl;

  }

  if(calotowertest){
    edm::ESHandle<PCaloGeometry> ctgeo;
    iSetup.get<PCaloTowerRcd>().get(ctgeo);
    std::cout<<"CALO TOWER"<<std::endl;
    std::vector<double> tsct = ctgeo->getTranslation();
    std::vector<double> dimct = ctgeo->getDimension();
    std::vector<uint32_t> indct = ctgeo->getIndexes();
    for(uint32_t i=0;i<tsct.size();i++){
      std::cout<<tsct[i];
    } 
    std::cout<<""<<std::endl;
    for(uint32_t i=0;i<dimct.size();i++){
      std::cout<<dimct[i];
    } 
    std::cout<<""<<std::endl;
    for(uint32_t i=0;i<indct.size();i++){
      std::cout<<indct[i];
    } 
    std::cout<<""<<std::endl;

  }

  if(castortest){
    edm::ESHandle<PCaloGeometry> castgeo;
    iSetup.get<PCastorRcd>().get(castgeo);
    std::cout<<"CASTOR"<<std::endl;
    std::vector<double> tscast = castgeo->getTranslation();
    std::vector<double> dimcast = castgeo->getDimension();
    std::vector<uint32_t> indcast = castgeo->getIndexes();
    for(uint32_t i=0;i<tscast.size();i++){
      std::cout<<tscast[i];
    } 
    std::cout<<""<<std::endl;
    for(uint32_t i=0;i<dimcast.size();i++){
      std::cout<<dimcast[i];
    } 
    std::cout<<""<<std::endl;
    for(uint32_t i=0;i<indcast.size();i++){
      std::cout<<indcast[i];
    } 
    std::cout<<""<<std::endl;

  }

  if(zdctest){
    edm::ESHandle<PCaloGeometry> zdcgeo;
    iSetup.get<PZdcRcd>().get(zdcgeo);
    std::cout<<"ZDC"<<std::endl;
    std::vector<double> tszdc = zdcgeo->getTranslation();
    std::vector<double> dimzdc = zdcgeo->getDimension();
    std::vector<uint32_t> indzdc = zdcgeo->getIndexes();
    for(uint32_t i=0;i<tszdc.size();i++){
      std::cout<<tszdc[i];
    } 
    std::cout<<""<<std::endl;
    for(uint32_t i=0;i<dimzdc.size();i++){
      std::cout<<dimzdc[i];
    } 
    std::cout<<""<<std::endl;
    for(uint32_t i=0;i<indzdc.size();i++){
      std::cout<<indzdc[i];
    } 
    std::cout<<""<<std::endl;

  }

  if(csctest){
    edm::ESHandle<RecoIdealGeometry> cscgeo;
    iSetup.get<CSCRecoGeometryRcd>().get(cscgeo);

    edm::ESHandle<CSCRecoDigiParameters> cscdigigeo;
    iSetup.get<CSCRecoDigiParametersRcd>().get(cscdigigeo);
    std::cout<<"CSC"<<std::endl;
    
    std::vector<int> obj1(cscdigigeo->pUserParOffset);
    for(uint32_t i=0; i<obj1.size();i++){
      std::cout<<obj1[i];
    }
    std::cout<<""<<std::endl;

    std::vector<int> obj2(cscdigigeo->pUserParSize);
    for(uint32_t i=0; i<obj2.size();i++){
      std::cout<<obj2[i];
    }
    std::cout<<""<<std::endl;

    std::vector<int> obj3(cscdigigeo->pChamberType);
    for(uint32_t i=0; i<obj3.size();i++){
      std::cout<<obj3[i];
    }
    std::cout<<""<<std::endl;

    std::vector<float> obj4(cscdigigeo->pfupars);
    for(uint32_t i=0; i<obj4.size();i++){
      std::cout<<obj4[i];
    }
    std::cout<<""<<std::endl;

    std::vector<DetId> myIdcsc(cscgeo->detIds());
    for(uint32_t i=0; i<myIdcsc.size();i++){
      std::cout<<myIdcsc[i];
    }
    std::cout<<""<<std::endl;
    uint32_t cscsize = myIdcsc.size();
    for(uint32_t i=0; i<cscsize;i++){
      std::vector<double> trcsc(cscgeo->tranStart(i), cscgeo->tranEnd(i));
      for(uint32_t j=0; j<trcsc.size();j++){
        std::cout<<trcsc[j];
      }
      std::cout<<""<<std::endl;
    
      std::vector<double> rotcsc(cscgeo->rotStart(i), cscgeo->rotEnd(i));
      for(uint32_t j=0; j<rotcsc.size();j++){
        std::cout<<rotcsc[j];
      }
      std::cout<<""<<std::endl;

      std::vector<double> shapecsc(cscgeo->shapeStart(i), cscgeo->shapeEnd(i));
      for(uint32_t j=0; j<shapecsc.size();j++){
        std::cout<<shapecsc[j];
      }
      std::cout<<""<<std::endl;
    }
  
  }


  if(dttest){
    edm::ESHandle<RecoIdealGeometry> dtgeo;
    iSetup.get<DTRecoGeometryRcd>().get(dtgeo);
    std::cout<<"DT"<<std::endl;
    std::vector<DetId> myIddt(dtgeo->detIds());
    for(uint32_t i=0; i<myIddt.size();i++){
      std::cout<<myIddt[i];
    }
    std::cout<<""<<std::endl;
    uint32_t dtsize = myIddt.size();
    for(uint32_t i=0; i<dtsize;i++){
      std::vector<double> trdt(dtgeo->tranStart(i), dtgeo->tranEnd(i));
      for(uint32_t j=0; j<trdt.size();j++){
        std::cout<<trdt[j];
      }
      std::cout<<""<<std::endl;
    
      std::vector<double> rotdt(dtgeo->rotStart(i), dtgeo->rotEnd(i));
      for(uint32_t j=0; j<rotdt.size();j++){
        std::cout<<rotdt[j];
      }
      std::cout<<""<<std::endl;

      std::vector<double> shapedt(dtgeo->shapeStart(i), dtgeo->shapeEnd(i));
      for(uint32_t j=0; j<shapedt.size();j++){
        std::cout<<shapedt[j];
      }
      std::cout<<""<<std::endl;

    }
  }

  if(rpctest){
    edm::ESHandle<RecoIdealGeometry> rpcgeo;
    iSetup.get<RPCRecoGeometryRcd>().get(rpcgeo);
    std::cout<<"RPC"<<std::endl;
    
    std::vector<DetId> myIdrpc(rpcgeo->detIds());
    for(uint32_t i=0; i<myIdrpc.size();i++){
      std::cout<<myIdrpc[i];
    }
    std::cout<<""<<std::endl;
    uint32_t rpcsize = myIdrpc.size();
    for(uint32_t i=0; i<rpcsize;i++){
      std::vector<double> trrpc(rpcgeo->tranStart(i), rpcgeo->tranEnd(i));
      for(uint32_t j=0; j<trrpc.size();j++){
        std::cout<<trrpc[j];
      }
      std::cout<<""<<std::endl;
  
      std::vector<double> rotrpc(rpcgeo->rotStart(i), rpcgeo->rotEnd(i));
      for(uint32_t j=0; j<rotrpc.size();j++){
        std::cout<<rotrpc[j];
      }
      std::cout<<""<<std::endl;
      
      std::vector<double> shaperpc(rpcgeo->shapeStart(i), rpcgeo->shapeEnd(i));
      for(uint32_t j=0; j<shaperpc.size();j++){
        std::cout<<shaperpc[j];
      }
      std::cout<<""<<std::endl;
      
      std::vector<std::string> strrpc(rpcgeo->strStart(i), rpcgeo->strEnd(i));
      for(uint32_t j=0; j<strrpc.size();j++){
        std::cout<<strrpc[j];
      }
      std::cout<<""<<std::endl;
      
    }

  }
  
}

DEFINE_FWK_MODULE(GeometryTester);
