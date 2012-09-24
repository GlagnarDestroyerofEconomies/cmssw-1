#include "DQM/EcalCommon/interface/DBWriterWorkers.h"

#include "DQM/EcalCommon/interface/LogicID.h"
#include "DQM/EcalCommon/interface/EcalDQMCommonUtils.h"

#include "DQM/EcalCommon/interface/MESetMulti.h"
#include "DQM/EcalCommon/interface/MESetUtils.h"

#include "OnlineDB/EcalCondDB/interface/MonCrystalConsistencyDat.h"
#include "OnlineDB/EcalCondDB/interface/MonTTConsistencyDat.h"
#include "OnlineDB/EcalCondDB/interface/MonMemChConsistencyDat.h"
#include "OnlineDB/EcalCondDB/interface/MonMemTTConsistencyDat.h"

#include "OnlineDB/EcalCondDB/interface/MonLaserBlueDat.h"
#include "OnlineDB/EcalCondDB/interface/MonLaserGreenDat.h"
#include "OnlineDB/EcalCondDB/interface/MonLaserIRedDat.h"
#include "OnlineDB/EcalCondDB/interface/MonLaserRedDat.h"
#include "OnlineDB/EcalCondDB/interface/MonPNBlueDat.h"
#include "OnlineDB/EcalCondDB/interface/MonPNGreenDat.h"
#include "OnlineDB/EcalCondDB/interface/MonPNIRedDat.h"
#include "OnlineDB/EcalCondDB/interface/MonPNRedDat.h"
#include "OnlineDB/EcalCondDB/interface/MonTimingLaserBlueCrystalDat.h"
#include "OnlineDB/EcalCondDB/interface/MonTimingLaserGreenCrystalDat.h"
#include "OnlineDB/EcalCondDB/interface/MonTimingLaserIRedCrystalDat.h"
#include "OnlineDB/EcalCondDB/interface/MonTimingLaserRedCrystalDat.h"
#include "OnlineDB/EcalCondDB/interface/MonPedestalsDat.h"
#include "OnlineDB/EcalCondDB/interface/MonPNPedDat.h"

#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "DataFormats/EcalDetId/interface/EcalTrigTowerDetId.h"
#include "DataFormats/EcalDetId/interface/EcalScDetId.h"
#include "DataFormats/EcalDetId/interface/EcalElectronicsId.h"
#include "DataFormats/EcalDetId/interface/EcalPnDiodeDetId.h"

#include "FWCore/Utilities/interface/Exception.h"

namespace ecaldqm {
  enum Quality {
    kBad = 0,
    kGood = 1,
    kUnknown = 2,
    kMBad = 3,
    kMGood = 4,
    kMUnknown = 5
  };

  enum Constants {
    nChannels = EBDetId::kSizeForDenseIndexing + EEDetId::kSizeForDenseIndexing,
    nTowers = EcalTrigTowerDetId::kEBTotalTowers + EcalScDetId::kSizeForDenseIndexing
  };

  bool
  qualityOK(int _quality)
  {
    return (_quality != kBad && _quality != kUnknown);
  }

  EcalLogicID
  crystalID(DetId const& _id)
  {
    unsigned iDCC(dccId(_id) - 1);
    if(iDCC <= kEEmHigh || iDCC >= kEEpLow){
      int ism(iDCC <= kEEmHigh ? 10 + iDCC - kEEmLow : 1 + iDCC - kEEpLow);
      EEDetId eeid(_id);
      int index(eeid.ix() * 1000 + eeid.iy());
      return LogicID::getEcalLogicID("EE_crystal_number", ism, index);
    }
    else{
      int ism(iDCC <= kEBmHigh ? 19 + iDCC - kEBmLow : 1 + iDCC - kEBpLow);
      EBDetId ebid(_id);
      return LogicID::getEcalLogicID("EB_crystal_number", ism, ebid.ic());
    }
  }

  EcalLogicID
  towerID(EcalElectronicsId const& _id)
  {
    unsigned iDCC(_id.dccId() - 1);
    if(iDCC <= kEEmHigh || iDCC >= kEEpLow){
      int ism(iDCC <= kEEmHigh ? 10 + iDCC - kEEmLow : 1 + iDCC - kEEpLow);
      return LogicID::getEcalLogicID("EE_readout_tower", ism, _id.towerId());
    }
    else{
      int ism(iDCC <= kEBmHigh ? 19 + iDCC - kEBmLow : 1 + iDCC - kEBpLow);
      return LogicID::getEcalLogicID("EB_trigger_tower", ism, _id.towerId());
    }
  }

  EcalLogicID
  memChannelID(EcalPnDiodeDetId const& _id)
  {
    // using the PN ID degenerates the logic ID - 50 time samples are actually split into 5 channels each
    unsigned iDCC(_id.iDCCId() - 1);
    int pnid(_id.iPnId());
    if(iDCC <= kEEmHigh || iDCC >= kEEpLow){
      int ism(iDCC <= kEEmHigh ? 10 + iDCC - kEEmLow : 1 + iDCC - kEEpLow);
      return LogicID::getEcalLogicID("EE_mem_channel", ism, (pnid - 1) % 5 + ((pnid - 1) / 5) * 25 + 1);
    }
    else{
      int ism(iDCC <= kEBmHigh ? 19 + iDCC - kEBmLow : 1 + iDCC - kEBpLow);
      return LogicID::getEcalLogicID("EB_mem_channel", ism, (pnid - 1) % 5 + ((pnid - 1) / 5) * 25 + 1);
    }
  }

  EcalLogicID
  memTowerID(EcalElectronicsId const& _id)
  {
    unsigned iDCC(_id.dccId() - 1);
    if(iDCC <= kEEmHigh || iDCC >= kEEpLow){
      int ism(iDCC <= kEEmHigh ? 10 + iDCC - kEEmLow : 1 + iDCC - kEEpLow);
      return LogicID::getEcalLogicID("EE_mem_TT", ism, _id.towerId());
    }
    else{
      int ism(iDCC <= kEBmHigh ? 19 + iDCC - kEBmLow : 1 + iDCC - kEBpLow);
      return LogicID::getEcalLogicID("EB_mem_TT", ism, _id.towerId());
    }
  }

  EcalLogicID
  lmPNID(EcalPnDiodeDetId const& _id)
  {
    unsigned iDCC(_id.iDCCId() - 1);
    int pnid(_id.iPnId());
    if(iDCC <= kEEmHigh || iDCC >= kEEpLow){
      int ism(iDCC <= kEEmHigh ? 10 + iDCC - kEEmLow : 1 + iDCC - kEEpLow);
      return LogicID::getEcalLogicID("EE_LM_PN", ism, pnid);
    }
    else{
      int ism(iDCC <= kEBmHigh ? 19 + iDCC - kEBmLow : 1 + iDCC - kEBpLow);
      return LogicID::getEcalLogicID("EB_LM_PN", ism, pnid);
    }
  }

  DBWriterWorker::DBWriterWorker(std::string const& _name) :
    name_(_name),
    runTypes_(),
    source_()
  {
  }

  void
  DBWriterWorker::setup(edm::ParameterSet const& _ps, BinService const* _binService)
  {
    edm::ParameterSet const& meParams(_ps.getUntrackedParameterSet(name_));

    std::vector<std::string> meNames(meParams.getParameterNames());
    for(unsigned iP(0); iP < meNames.size(); ++iP){
      std::string& meName(meNames[iP]);
      edm::ParameterSet const& meParam(meParams.getUntrackedParameterSet(meName));
      source_[meName] = ecaldqm::createMESet(meParam, _binService);
    }
  }

  void
  DBWriterWorker::retrieveSource()
  {
    for(PtrMap<std::string, MESet const>::iterator sItr(source_.begin()); sItr != source_.end(); ++sItr){
      if(!sItr->second->retrieve())
        throw cms::Exception("IOError") << name_ << ": MESet " << sItr->first << " not found";
    }
  }

  IntegrityWriter::IntegrityWriter() :
    DBWriterWorker("Integrity")
  {
    runTypes_.insert("COSMIC");
    runTypes_.insert("BEAM");
    runTypes_.insert("MTCC");
    runTypes_.insert("LASER");
    runTypes_.insert("TEST_PULSE");
    runTypes_.insert("PEDESTAL");
    runTypes_.insert("PEDESTAL-OFFSET");
    runTypes_.insert("LED");
    runTypes_.insert("PHYSICS");
  }
 
  bool
  IntegrityWriter::run(EcalCondDBInterface* _db, MonRunIOV& _iov)
  {
    /*
      uses
      OccupancyTask.Digi (h_)
      PNDiodeTask.Occupancy (hmem_)
      IntegrityTask.Gain (h01_)
      IntegrityTask.ChId (h02_)
      IntegrityTask.GainSwitch (h03_)
      IntegrityTask.TowerId (h04_)
      IntegrityTask.BlockSize (h05_)
      RawDataTask.L1AFE
      RawDataTask.BXFE
      PNDiodeTask.MemChId (h06_)
      PNDiodeTask.MemGain (h07_)
      PNDiodeTask.MemTowerId (h08_)
      PNDiodeTask.MomBlockSize (h09_)
      IntegrityClient.Quality
      PNIntegrityClient.QualitySummary
    */

    bool result(true);

    std::map<EcalLogicID, MonCrystalConsistencyDat> crystalConsistencies;
    std::map<EcalLogicID, MonTTConsistencyDat> towerConsistencies;
    std::map<EcalLogicID, MonMemChConsistencyDat> memChannelConsistencies;
    std::map<EcalLogicID, MonMemTTConsistencyDat> memTowerConsistencies;

    MESet const* digiME(source_["Digi"]);
    MESet const* gainME(source_["Gain"]);
    MESet const* chidME(source_["ChId"]);
    MESet const* gainswitchME(source_["GainSwitch"]);
    MESet const* qualityME(source_["Quality"]);
    if(!digiME || !gainME || !chidME || !gainswitchME || !qualityME)
      throw cms::Exception("Configuration") << "Channel integrity MEs not found";

    MESet const* toweridME(source_["TowerId"]);
    MESet const* blocksizeME(source_["BlockSize"]);
    MESet const* l1aME(source_["L1AFE"]);
    MESet const* bxME(source_["BXFE"]);
    if(!toweridME || !blocksizeME || !l1aME || !bxME)
      throw cms::Exception("Configuration") << "Tower integrity MEs not found";

    MESet const* memdigiME(source_["MEMDigi"]);
    MESet const* memchidME(source_["MEMChId"]);
    MESet const* memgainME(source_["MEMGain"]);
    MESet const* pnqualityME(source_["PNQuality"]);
    if(!memdigiME || !memchidME || !memgainME || !pnqualityME)
      throw cms::Exception("Configuration") << "MEM channel integrity MEs not found";

    MESet const* memtoweridME(source_["MEMTowerId"]);
    MESet const* memblocksizeME(source_["MEMBlockSize"]);
    if(!memtoweridME || !memblocksizeME)
      throw cms::Exception("Configuration") << "MEM tower integrity MEs not found";

    if(verbosity_ > 1) std::cout << " Looping over crystals" << std::endl;

    MESet::const_iterator dEnd(digiME->end());
    MESet::const_iterator qItr(qualityME);
    for(MESet::const_iterator dItr(digiME->beginChannel()); dItr != dEnd; dItr.toNextChannel()){
      DetId id(dItr->getId());

      int nDigis(dItr->getBinContent());
      int gain(gainME->getBinContent(id));
      int chid(chidME->getBinContent(id));
      int gainswitch(gainswitchME->getBinContent(id));
      qItr = dItr;

      if(gain > 0 || chid > 0 || gainswitch > 0){
        MonCrystalConsistencyDat& data(crystalConsistencies[crystalID(id)]);
        data.setProcessedEvents(nDigis);
        data.setProblematicEvents(gain + chid + gainswitch);
        data.setProblemsGainZero(gain);
        data.setProblemsID(chid);
        data.setProblemsGainSwitch(gainswitch);

        int channelStatus(qItr->getBinContent());
        bool channelBad(channelStatus == kBad || channelStatus == kMBad);
        data.setTaskStatus(channelBad);

        result &= qualityOK(channelStatus);
      }
    }

    if(verbosity_ > 1) std::cout << " Looping over towers" << std::endl;

    for(unsigned iDCC(kEEmLow); iDCC <= kEBpHigh; ++iDCC){
      for(unsigned iTower(1); iTower <= 68; ++iTower){
        if(!ccuExists(iDCC + 1, iTower)) continue;

        EcalElectronicsId eid(iDCC + 1, iTower, 1, 1);
        std::vector<DetId> channels(getElectronicsMap()->dccTowerConstituents(iDCC + 1, iTower));
        int nDigis(0);
        bool towerBad(false);
        for(unsigned iD(0); iD < channels.size(); ++iD){
          int n(digiME->getBinContent(channels[iD]));
          if(n > nDigis) nDigis = n;
          int channelStatus(qualityME->getBinContent(channels[iD]));
          if(channelStatus == kBad || channelStatus == kMBad) towerBad = true;
        }

        int towerid(toweridME->getBinContent(eid));
        int blocksize(blocksizeME->getBinContent(eid));
        int l1a(l1aME->getBinContent(iDCC + 1, iTower));
        int bx(bxME->getBinContent(iDCC + 1, iTower));

        if(towerid > 0 || blocksize > 0 || l1a > 0 || bx > 0){
          MonTTConsistencyDat& data(towerConsistencies[towerID(eid)]);
          data.setProcessedEvents(nDigis);
          data.setProblematicEvents(towerid + blocksize + l1a + bx);
          data.setProblemsID(towerid);
          data.setProblemsSize(blocksize);
          data.setProblemsLV1(l1a);
          data.setProblemsBunchX(bx);
          data.setTaskStatus(towerBad);

          result &= !towerBad;
        }
      }
    }

    if(verbosity_ > 1) std::cout << " Looping over MEM channels and towers" << std::endl;

    for(unsigned iMD(0); iMD < memDCC.size(); ++iMD){
      unsigned iDCC(memDCC[iMD]);

      int subdet(iDCC <= kEEmHigh || iDCC >= kEEpLow ? EcalEndcap : EcalBarrel);

      for(unsigned iPN(1); iPN <= 10; ++iPN){
        EcalPnDiodeDetId pnid(subdet, iDCC + 1, iPN);

        int nDigis(memdigiME->getBinContent(pnid));
        int memchid(memchidME->getBinContent(pnid));
        int memgain(memgainME->getBinContent(pnid));

        if(memchid > 0 || memgain > 0){
          MonMemChConsistencyDat& data(memChannelConsistencies[memChannelID(pnid)]);

          data.setProcessedEvents(nDigis);
          data.setProblematicEvents(memchid + memgain);
          data.setProblemsID(memchid);
          data.setProblemsGainZero(memgain);

          int channelStatus(pnqualityME->getBinContent(pnid));
          bool channelBad(channelStatus == kBad || channelStatus == kMBad);
          data.setTaskStatus(channelBad);

          result &= qualityOK(channelStatus);
        }
      }

      for(unsigned iTower(69); iTower <= 70; ++iTower){
        EcalElectronicsId eid(iDCC + 1, iTower, 1, 1);

        int nDigis(0);
        bool towerBad(false);
        for(unsigned iPN(1); iPN <= 10; ++iPN){
          EcalPnDiodeDetId pnid(subdet, iDCC + 1, iPN);
          int n(memdigiME->getBinContent(pnid));
          if(n > nDigis) nDigis = n;
          int channelStatus(pnqualityME->getBinContent(pnid));
          if(channelStatus == kBad || channelStatus == kMBad) towerBad = true;
        }

        int towerid(memtoweridME->getBinContent(eid));
        int blocksize(memblocksizeME->getBinContent(eid));

        if(towerid > 0 || blocksize > 0){
          MonMemTTConsistencyDat& data(memTowerConsistencies[memTowerID(eid)]);

          data.setProcessedEvents(nDigis);
          data.setProblematicEvents(towerid + blocksize);
          data.setProblemsID(towerid);
          data.setProblemsSize(blocksize);
          data.setTaskStatus(towerBad);

          result &= !towerBad;
        }
      }
    }

    if(verbosity_ > 1) std::cout << " Inserting data" << std::endl;

    try{
      if(crystalConsistencies.size() > 0){
        if(verbosity_ > 2) std::cout << " crystalConsistencies" << std::endl;
        _db->insertDataArraySet(&crystalConsistencies, &_iov);
      }
      if(towerConsistencies.size() > 0){
        if(verbosity_ > 2) std::cout << " towerConsistencies" << std::endl;
        _db->insertDataArraySet(&towerConsistencies, &_iov);
      }
      if(memChannelConsistencies.size() > 0){
        if(verbosity_ > 2) std::cout << " memChannelConsistencies" << std::endl;
        _db->insertDataArraySet(&memChannelConsistencies, &_iov);
      }
      if(memTowerConsistencies.size() > 0){
        if(verbosity_ > 2) std::cout << " memTowerConsistencies" << std::endl;
        _db->insertDataArraySet(&memTowerConsistencies, &_iov);
      }
    }
    catch(std::runtime_error& e){
      throw cms::Exception("DBError") << e.what();
    }

    return result;
  }

  LaserWriter::LaserWriter() :
    DBWriterWorker("Laser")
  {
    runTypes_.insert("COSMIC");
    runTypes_.insert("BEAM");
    runTypes_.insert("MTCC");
    runTypes_.insert("LASER");
    runTypes_.insert("TEST_PULSE");
    runTypes_.insert("PEDESTAL");
    runTypes_.insert("LED");
    runTypes_.insert("PHYSICS");
  }

  void
  LaserWriter::setup(edm::ParameterSet const& _ps, BinService const* _binService)
  {
    using namespace std;

    DBWriterWorker::setup(_ps, _binService);

    vector<int> laserWavelengths(_ps.getUntrackedParameter<vector<int> >("laserWavelengths"));

    unsigned iMEWL(0);
    for(vector<int>::iterator wlItr(laserWavelengths.begin()); wlItr != laserWavelengths.end(); ++wlItr){
      if(*wlItr <= 0 || *wlItr >= 5) throw cms::Exception("InvalidConfiguration") << "Laser Wavelength" << endl;
      wlToME_[*wlItr] = iMEWL++;
    }

    map<string, string> replacements;
    stringstream ss;

    string wlPlots[] = {"Amplitude", "AOverP", "Timing"};
    for(unsigned iS(0); iS < sizeof(wlPlots) / sizeof(string); ++iS){
      string plot(wlPlots[iS]);
      MESetMulti const* multi(static_cast<MESetMulti const*>(source_[plot]));

      for(map<int, unsigned>::iterator wlItr(wlToME_.begin()); wlItr != wlToME_.end(); ++wlItr){
        multi->use(wlItr->second);

        ss.str("");
        ss << wlItr->first;
        replacements["wl"] = ss.str();

        multi->formPath(replacements);
      }
    }
  }

  bool
  LaserWriter::run(EcalCondDBInterface* _db, MonRunIOV& _iov)
  {
    /*
      uses
      LaserTask.Amplitude (h01, h03, h05, h07)
      LaserTask.AOverP (h02, h04, h06, h08)
      LaserTask.Timing (h09, h10, h11, h12)
      LaserClient.Quality (meg01, meg02, meg03, meg04)
      LaserTask.PNAmplitude (i09, i10, i11, i12)
      LaserClient.PNQualitySummary (meg09, meg10, meg11, meg12)
      PNDiodeTask.Pedestal (i13, i14, i15, i16)
    */

    bool result(true);

    std::map<EcalLogicID, MonLaserBlueDat> l1Amp;
    std::map<EcalLogicID, MonTimingLaserBlueCrystalDat> l1Time;
    std::map<EcalLogicID, MonPNBlueDat> l1PN;
    std::map<EcalLogicID, MonLaserGreenDat> l2Amp;
    std::map<EcalLogicID, MonTimingLaserGreenCrystalDat> l2Time;
    std::map<EcalLogicID, MonPNGreenDat> l2PN;
    std::map<EcalLogicID, MonLaserIRedDat> l3Amp;
    std::map<EcalLogicID, MonTimingLaserIRedCrystalDat> l3Time;
    std::map<EcalLogicID, MonPNIRedDat> l3PN;
    std::map<EcalLogicID, MonLaserRedDat> l4Amp;
    std::map<EcalLogicID, MonTimingLaserRedCrystalDat> l4Time;
    std::map<EcalLogicID, MonPNRedDat> l4PN;

    MESet const* ampME(source_["Amplitude"]);
    MESet const* aopME(source_["AOverP"]);
    MESet const* timeME(source_["Timing"]);
    MESet const* qualityME(source_["Quality"]);
    if(!ampME || !aopME || !timeME || !qualityME)
      throw cms::Exception("Configuration") << "Laser MEs not found";

    MESet const* pnME(source_["PNAmplitude"]);
    MESet const* pnQualityME(source_["PNQuality"]);
    MESet const* pnPedestalME(source_["PNPedestal"]);
    if(!pnME || !pnQualityME || !pnPedestalME)
      throw cms::Exception("Configuration") << "Laser PN MEs not found";

    for(std::map<int, unsigned>::iterator wlItr(wlToME_.begin()); wlItr != wlToME_.end(); ++wlItr){
      int wl(wlItr->first);
      unsigned iM(wlItr->second);

      static_cast<MESetMulti const*>(ampME)->use(iM);
      static_cast<MESetMulti const*>(aopME)->use(iM);
      static_cast<MESetMulti const*>(timeME)->use(iM);
      static_cast<MESetMulti const*>(qualityME)->use(iM);
      static_cast<MESetMulti const*>(pnME)->use(iM);
      static_cast<MESetMulti const*>(pnQualityME)->use(iM);

      MESet::const_iterator aEnd(ampME->end());
      MESet::const_iterator qItr(qualityME);
      MESet::const_iterator oItr(aopME);
      MESet::const_iterator tItr(timeME);
      for(MESet::const_iterator aItr(ampME->beginChannel()); aItr != aEnd; aItr.toNextChannel()){
        float aEntries(aItr->getBinEntries());
        if(aEntries < 1.) continue;

        qItr = aItr;
        oItr = aItr;
        tItr = aItr;

        DetId id(aItr->getId());

        float ampMean(aItr->getBinContent());
        float ampRms(aItr->getBinError() * std::sqrt(aEntries));

        float aopEntries(oItr->getBinEntries());
        float aopMean(oItr->getBinContent());
        float aopRms(oItr->getBinError() * std::sqrt(aopEntries));

        float timeEntries(tItr->getBinEntries());
        float timeMean(tItr->getBinContent());
        float timeRms(tItr->getBinError() * std::sqrt(timeEntries));

        int channelStatus(qItr->getBinContent());
        bool channelBad(channelStatus == kBad || channelStatus == kMBad);

        EcalLogicID logicID(crystalID(id));

        switch(wl){
        case 1:
          {
            MonLaserBlueDat& aData(l1Amp[logicID]);
            aData.setAPDMean(ampMean);
            aData.setAPDRMS(ampRms);
            aData.setAPDOverPNMean(aopMean);
            aData.setAPDOverPNRMS(aopRms);
            aData.setTaskStatus(channelBad);
 
            MonTimingLaserBlueCrystalDat& tData(l1Time[logicID]);
            tData.setTimingMean(timeMean);
            tData.setTimingRMS(timeRms);
            tData.setTaskStatus(channelBad);
          }
          break;
        case 2:
          {
            MonLaserGreenDat& aData(l2Amp[logicID]);
            aData.setAPDMean(ampMean);
            aData.setAPDRMS(ampRms);
            aData.setAPDOverPNMean(aopMean);
            aData.setAPDOverPNRMS(aopRms);
            aData.setTaskStatus(channelBad);
 
            MonTimingLaserGreenCrystalDat& tData(l2Time[logicID]);
            tData.setTimingMean(timeMean);
            tData.setTimingRMS(timeRms);
            tData.setTaskStatus(channelBad);
          }
          break;
        case 3:
          {
            MonLaserIRedDat& aData(l3Amp[logicID]);
            aData.setAPDMean(ampMean);
            aData.setAPDRMS(ampRms);
            aData.setAPDOverPNMean(aopMean);
            aData.setAPDOverPNRMS(aopRms);
            aData.setTaskStatus(channelBad);

            MonTimingLaserIRedCrystalDat& tData(l3Time[logicID]);
            tData.setTimingMean(timeMean);
            tData.setTimingRMS(timeRms);
            tData.setTaskStatus(channelBad);
          }
          break;
        case 4:
          {
            MonLaserRedDat& aData(l4Amp[logicID]);
            aData.setAPDMean(ampMean);
            aData.setAPDRMS(ampRms);
            aData.setAPDOverPNMean(aopMean);
            aData.setAPDOverPNRMS(aopRms);
            aData.setTaskStatus(channelBad);

            MonTimingLaserRedCrystalDat& tData(l4Time[logicID]);
            tData.setTimingMean(timeMean);
            tData.setTimingRMS(timeRms);
            tData.setTaskStatus(channelBad);
          }
          break;
        }
        result &= qualityOK(channelStatus);
      }

      for(unsigned iMD(0); iMD < memDCC.size(); ++iMD){
        unsigned iDCC(memDCC[iMD]);

        int subdet(iDCC <= kEEmHigh || iDCC >= kEEpLow ? EcalEndcap : EcalBarrel);

        for(unsigned iPN(1); iPN <= 10; ++iPN){
          EcalPnDiodeDetId pnid(subdet, iDCC + 1, iPN);

          float entries(pnME->getBinEntries(pnid));
          if(entries < 1.) continue;

          float mean(pnME->getBinContent(pnid));
          float rms(pnME->getBinError(pnid) * std::sqrt(entries));

          float pedestalEntries(pnPedestalME->getBinEntries(pnid));
          float pedestalMean(pnPedestalME->getBinContent(pnid));
          float pedestalRms(pnPedestalME->getBinError(pnid) * std::sqrt(pedestalEntries));

          int channelStatus(pnQualityME->getBinContent(pnid));
          bool channelBad(channelStatus == kBad || channelStatus == kMBad);

          switch(wl){
          case 1:
            {
              MonPNBlueDat& data(l1PN[lmPNID(pnid)]);
              data.setADCMeanG1(-1.);
              data.setADCRMSG1(-1.);
              data.setPedMeanG1(-1.);
              data.setPedRMSG1(-1.);
              data.setADCMeanG16(mean);
              data.setADCRMSG16(rms);
              data.setPedMeanG16(pedestalMean);
              data.setPedRMSG16(pedestalRms);
              data.setTaskStatus(channelBad);
            }
            break;
          case 2:
            {
              MonPNGreenDat& data(l2PN[lmPNID(pnid)]);
              data.setADCMeanG1(-1.);
              data.setADCRMSG1(-1.);
              data.setPedMeanG1(-1.);
              data.setPedRMSG1(-1.);
              data.setADCMeanG16(mean);
              data.setADCRMSG16(rms);
              data.setPedMeanG16(pedestalMean);
              data.setPedRMSG16(pedestalRms);
              data.setTaskStatus(channelBad);
            }
            break;
          case 3:
            {
              MonPNIRedDat& data(l3PN[lmPNID(pnid)]);
              data.setADCMeanG1(-1.);
              data.setADCRMSG1(-1.);
              data.setPedMeanG1(-1.);
              data.setPedRMSG1(-1.);
              data.setADCMeanG16(mean);
              data.setADCRMSG16(rms);
              data.setPedMeanG16(pedestalMean);
              data.setPedRMSG16(pedestalRms);
              data.setTaskStatus(channelBad);
            }
            break;
          case 4:
            {
              MonPNRedDat& data(l4PN[lmPNID(pnid)]);
              data.setADCMeanG1(-1.);
              data.setADCRMSG1(-1.);
              data.setPedMeanG1(-1.);
              data.setPedRMSG1(-1.);
              data.setADCMeanG16(mean);
              data.setADCRMSG16(rms);
              data.setPedMeanG16(pedestalMean);
              data.setPedRMSG16(pedestalRms);
              data.setTaskStatus(channelBad);
            }
            break;
          }

          result &= qualityOK(channelStatus);

        }
      }
    }

    try{
      if(l1Amp.size() > 0)
        _db->insertDataArraySet(&l1Amp, &_iov);
      if(l1Time.size() > 0)
        _db->insertDataArraySet(&l1Time, &_iov);
      if(l1PN.size() > 0)
        _db->insertDataArraySet(&l1PN, &_iov);
      if(l2Amp.size() > 0)
        _db->insertDataArraySet(&l2Amp, &_iov);
      if(l2Time.size() > 0)
        _db->insertDataArraySet(&l2Time, &_iov);
      if(l2PN.size() > 0)
        _db->insertDataArraySet(&l2PN, &_iov);
      if(l3Amp.size() > 0)
        _db->insertDataArraySet(&l3Amp, &_iov);
      if(l3Time.size() > 0)
        _db->insertDataArraySet(&l3Time, &_iov);
      if(l3PN.size() > 0)
        _db->insertDataArraySet(&l3PN, &_iov);
      if(l4Amp.size() > 0)
        _db->insertDataArraySet(&l4Amp, &_iov);
      if(l4Time.size() > 0)
        _db->insertDataArraySet(&l4Time, &_iov);
      if(l4PN.size() > 0)
        _db->insertDataArraySet(&l4PN, &_iov);
    }
    catch(std::runtime_error& e){
      throw cms::Exception("DBError") << e.what();
    }

    return result;
  }

  PedestalWriter::PedestalWriter() :
    DBWriterWorker("Pedestal")
  {
    runTypes_.insert("COSMIC");
    runTypes_.insert("BEAM");
    runTypes_.insert("MTCC");
    runTypes_.insert("LASER");
    runTypes_.insert("TEST_PULSE");
    runTypes_.insert("PEDESTAL");
    runTypes_.insert("LED");
    runTypes_.insert("PHYSICS");
  }

  void
  PedestalWriter::setup(edm::ParameterSet const& _ps, BinService const* _binService)
  {
    using namespace std;

    DBWriterWorker::setup(_ps, _binService);

    vector<int> MGPAGains(_ps.getUntrackedParameter<vector<int> >("MGPAGains"));
    vector<int> MGPAGainsPN(_ps.getUntrackedParameter<vector<int> >("MGPAGainsPN"));

    unsigned iMEGain(0);
    for(vector<int>::iterator gainItr(MGPAGains.begin()); gainItr != MGPAGains.end(); ++gainItr){
      if(*gainItr != 1 && *gainItr != 6 && *gainItr != 12) throw cms::Exception("InvalidConfiguration") << "MGPA gain" << endl;
      gainToME_[*gainItr] = iMEGain++;
    }

    unsigned iMEPNGain(0);
    for(vector<int>::iterator gainItr(MGPAGainsPN.begin()); gainItr != MGPAGainsPN.end(); ++gainItr){
      if(*gainItr != 1 && *gainItr != 16) throw cms::Exception("InvalidConfiguration") << "PN diode gain" << endl;	
      pnGainToME_[*gainItr] = iMEPNGain++;
    }

    map<string, string> replacements;
    stringstream ss;

    string apdSources[] = {"Pedestal"};
    for(unsigned iS(0); iS < sizeof(apdSources) / sizeof(string); ++iS){
      string plot(apdSources[iS]);
      MESetMulti const* multi(static_cast<MESetMulti const*>(source_[plot]));

      for(map<int, unsigned>::iterator gainItr(gainToME_.begin()); gainItr != gainToME_.end(); ++gainItr){
        multi->use(gainItr->second);

        ss.str("");
        ss << std::setfill('0') << std::setw(2) << gainItr->first;
        replacements["gain"] = ss.str();

        multi->formPath(replacements);
      }
    }

    string pnSources[] = {"PNPedestal"};
    for(unsigned iS(0); iS < sizeof(pnSources) / sizeof(string); ++iS){
      string plot(pnSources[iS]);
      MESetMulti const* multi(static_cast<MESetMulti const*>(source_[plot]));

      for(map<int, unsigned>::iterator gainItr(pnGainToME_.begin()); gainItr != pnGainToME_.end(); ++gainItr){
        multi->use(gainItr->second);

        ss.str("");
        ss << std::setfill('0') << std::setw(2) << gainItr->first;
        replacements["pngain"] = ss.str();

        multi->formPath(replacements);
      }
    }
  }

  bool
  PedestalWriter::run(EcalCondDBInterface* _db, MonRunIOV& _iov)
  {
    /*
      uses
      PedestalTask.Pedestal (h01, h02, h03)
      PedestalTask.PNPedestal (i01, i02)
      PedestalClient.Quality (meg01, meg02, meg03)
      PedestalClient.PNQualitySummary (meg04, meg05)
    */

    bool result(true);

    std::map<EcalLogicID, MonPedestalsDat> pedestals;
    std::map<EcalLogicID, MonPNPedDat> pnPedestals;

    MESet const* pedestalME(source_["Pedestal"]);
    MESet const* qualityME(source_["Quality"]);
    if(!pedestalME || !qualityME)
      throw cms::Exception("Configuration") << "Channel integrity MEs not found";

    MESet const* pnPedestalME(source_["PNPedestal"]);
    MESet const* pnQualityME(source_["PNQuality"]);
    if(!pnPedestalME || !pnQualityME)
      throw cms::Exception("Configuration") << "Tower integrity MEs not found";

    for(std::map<int, unsigned>::iterator gainItr(gainToME_.begin()); gainItr != gainToME_.end(); ++gainItr){
      int gain(gainItr->first);
      int iM(gainItr->second);

      static_cast<MESetMulti const*>(pedestalME)->use(iM);
      static_cast<MESetMulti const*>(qualityME)->use(iM);

      MESet::const_iterator pEnd(pedestalME->end());
      MESet::const_iterator qItr(qualityME);
      for(MESet::const_iterator pItr(pedestalME->beginChannel()); pItr != pEnd; pItr.toNextChannel()){
        float entries(pItr->getBinEntries());
        if(entries < 1.) continue;

        qItr = pItr;

        float mean(pItr->getBinContent());
        float rms(pItr->getBinError() * std::sqrt(entries));

        EcalLogicID logicID(crystalID(pItr->getId()));
        if(pedestals.find(logicID) == pedestals.end()){
          MonPedestalsDat& insertion(pedestals[logicID]);
          insertion.setPedMeanG1(-1.);
          insertion.setPedRMSG1(-1.);
          insertion.setPedMeanG6(-1.);
          insertion.setPedRMSG6(-1.);
          insertion.setPedMeanG12(-1.);
          insertion.setPedRMSG12(-1.);
          insertion.setTaskStatus(false);
        }

        MonPedestalsDat& data(pedestals[logicID]);
        switch(gain){
        case 1:
          data.setPedMeanG1(mean);
          data.setPedRMSG1(rms);
          break;
        case 6:
          data.setPedMeanG6(mean);
          data.setPedRMSG6(rms);
          break;
        case 12:
          data.setPedMeanG12(mean);
          data.setPedRMSG12(rms);
          break;
        }

        int channelStatus(qItr->getBinContent());
        bool channelBad(channelStatus == kBad || channelStatus == kMBad);
        if(channelBad)
          data.setTaskStatus(true);

        result &= qualityOK(channelStatus);
      }
    }

    for(std::map<int, unsigned>::iterator gainItr(pnGainToME_.begin()); gainItr != pnGainToME_.end(); ++gainItr){
      int gain(gainItr->first);
      int iM(gainItr->second);

      static_cast<MESetMulti const*>(pnPedestalME)->use(iM);
      static_cast<MESetMulti const*>(pnQualityME)->use(iM);

      for(unsigned iMD(0); iMD < memDCC.size(); ++iMD){
        unsigned iDCC(memDCC[iMD]);

        int subdet(iDCC <= kEEmHigh || iDCC >= kEEpLow ? EcalEndcap : EcalBarrel);

        for(unsigned iPN(1); iPN <= 10; ++iPN){
          EcalPnDiodeDetId pnid(subdet, iDCC + 1, iPN);

          float entries(pnPedestalME->getBinEntries(pnid));
          if(entries < 1.) continue;

          float mean(pnPedestalME->getBinContent(pnid));
          float rms(pnPedestalME->getBinError(pnid) * std::sqrt(entries));

          EcalLogicID logicID(lmPNID(pnid));
          if(pnPedestals.find(logicID) == pnPedestals.end()){
            MonPNPedDat& insertion(pnPedestals[logicID]);
            insertion.setPedMeanG1(-1.);
            insertion.setPedRMSG1(-1.);
            insertion.setPedMeanG16(-1.);
            insertion.setPedRMSG16(-1.);
            insertion.setTaskStatus(false);
          }

          MonPNPedDat& data(pnPedestals[lmPNID(pnid)]);
          switch(gain){
          case 1:
            data.setPedMeanG1(mean);
            data.setPedRMSG1(rms);
            break;
          case 2:
            data.setPedMeanG16(mean);
            data.setPedRMSG16(rms);
            break;
          }

          int channelStatus(pnQualityME->getBinContent(pnid));
          bool channelBad(channelStatus == kBad || channelStatus == kMBad);
          if(channelBad)
            data.setTaskStatus(true);

          result &= qualityOK(channelStatus);
        }
      }
    }

    try{
      if(pedestals.size() > 0)
        _db->insertDataArraySet(&pedestals, &_iov);
      if(pnPedestals.size() > 0)
        _db->insertDataArraySet(&pnPedestals, &_iov);
    }
    catch(std::runtime_error& e){
      throw cms::Exception("DBError") << e.what();
    }

    return result;
  }

  PresampleWriter::PresampleWriter() :
    DBWriterWorker("Presample")
  {
    runTypes_.insert("COSMIC");
    runTypes_.insert("BEAM");
    runTypes_.insert("MTCC");
    runTypes_.insert("LASER");
    runTypes_.insert("TEST_PULSE");
    runTypes_.insert("PEDESTAL");
    runTypes_.insert("LED");
    runTypes_.insert("PHYSICS");
  }

  bool
  PresampleWriter::run(EcalCondDBInterface* _db, MonRunIOV& _iov)
  {
    return true;
  }

  TestPulseWriter::TestPulseWriter() :
    DBWriterWorker("TestPulse")
  {
    runTypes_.insert("COSMIC");
    runTypes_.insert("BEAM");
    runTypes_.insert("MTCC");
    runTypes_.insert("LASER");
    runTypes_.insert("TEST_PULSE");
    runTypes_.insert("PEDESTAL");
    runTypes_.insert("LED");
    runTypes_.insert("PHYSICS");
  }

  void
  TestPulseWriter::setup(edm::ParameterSet const& _ps, BinService const* _binService)
  {
    using namespace std;

    DBWriterWorker::setup(_ps, _binService);

    vector<int> MGPAGains(_ps.getUntrackedParameter<vector<int> >("MGPAGains"));
    vector<int> MGPAGainsPN(_ps.getUntrackedParameter<vector<int> >("MGPAGainsPN"));

    unsigned iMEGain(0);
    for(vector<int>::iterator gainItr(MGPAGains.begin()); gainItr != MGPAGains.end(); ++gainItr){
      if(*gainItr != 1 && *gainItr != 6 && *gainItr != 12) throw cms::Exception("InvalidConfiguration") << "MGPA gain" << endl;
      gainToME_[*gainItr] = iMEGain++;
    }

    unsigned iMEPNGain(0);
    for(vector<int>::iterator gainItr(MGPAGainsPN.begin()); gainItr != MGPAGainsPN.end(); ++gainItr){
      if(*gainItr != 1 && *gainItr != 16) throw cms::Exception("InvalidConfiguration") << "PN diode gain" << endl;	
      pnGainToME_[*gainItr] = iMEPNGain++;
    }

    map<string, string> replacements;
    stringstream ss;

    string apdSources[] = {"Amplitude"};
    for(unsigned iS(0); iS < sizeof(apdSources) / sizeof(string); ++iS){
      string plot(apdSources[iS]);
      MESetMulti const* multi(static_cast<MESetMulti const*>(source_[plot]));

      for(map<int, unsigned>::iterator gainItr(gainToME_.begin()); gainItr != gainToME_.end(); ++gainItr){
        multi->use(gainItr->second);

        ss.str("");
        ss << std::setfill('0') << std::setw(2) << gainItr->first;
        replacements["gain"] = ss.str();

        multi->formPath(replacements);
      }
    }

    string pnSources[] = {"PNAmplitude"};
    for(unsigned iS(0); iS < sizeof(pnSources) / sizeof(string); ++iS){
      string plot(pnSources[iS]);
      MESetMulti const* multi(static_cast<MESetMulti const*>(source_[plot]));

      for(map<int, unsigned>::iterator gainItr(pnGainToME_.begin()); gainItr != pnGainToME_.end(); ++gainItr){
        multi->use(gainItr->second);

        ss.str("");
        ss << std::setfill('0') << std::setw(2) << gainItr->first;
        replacements["pngain"] = ss.str();

        multi->formPath(replacements);
      }
    }
  }

  bool
  TestPulseWriter::run(EcalCondDBInterface* _db, MonRunIOV& _iov)
  {
    return true;
  }

  TimingWriter::TimingWriter() :
    DBWriterWorker("Timing")
  {
    runTypes_.insert("COSMIC");
    runTypes_.insert("BEAM");
    runTypes_.insert("MTCC");
    runTypes_.insert("LASER");
    runTypes_.insert("TEST_PULSE");
    runTypes_.insert("PEDESTAL");
    runTypes_.insert("LED");
    runTypes_.insert("PHYSICS");
  }

  bool
  TimingWriter::run(EcalCondDBInterface* _db, MonRunIOV& _iov)
  {
    return true;
  }

  LedWriter::LedWriter() :
    DBWriterWorker("Led")
  {
    runTypes_.insert("COSMIC");
    runTypes_.insert("BEAM");
    runTypes_.insert("MTCC");
    runTypes_.insert("LASER");
    runTypes_.insert("TEST_PULSE");
    runTypes_.insert("PEDESTAL");
    runTypes_.insert("LED");
    runTypes_.insert("PHYSICS");
  }

  void
  LedWriter::setup(edm::ParameterSet const& _ps, BinService const* _binService)
  {
    using namespace std;

    DBWriterWorker::setup(_ps, _binService);

    vector<int> ledWavelengths(_ps.getUntrackedParameter<vector<int> >("ledWavelengths"));

    unsigned iMEWL(0);
    for(vector<int>::iterator wlItr(ledWavelengths.begin()); wlItr != ledWavelengths.end(); ++wlItr){
      if(*wlItr <= 0 || *wlItr >= 5) throw cms::Exception("InvalidConfiguration") << "Led Wavelength" << endl;
      wlToME_[*wlItr] = iMEWL++;
    }

    map<string, string> replacements;
    stringstream ss;

    string wlPlots[] = {"Amplitude", "AOverP", "Timing"};
    for(unsigned iS(0); iS < sizeof(wlPlots) / sizeof(string); ++iS){
      string plot(wlPlots[iS]);
      MESetMulti const* multi(static_cast<MESetMulti const*>(source_[plot]));

      for(map<int, unsigned>::iterator wlItr(wlToME_.begin()); wlItr != wlToME_.end(); ++wlItr){
        multi->use(wlItr->second);

        ss.str("");
        ss << wlItr->first;
        replacements["wl"] = ss.str();

        multi->formPath(replacements);
      }
    }
  }

  bool
  LedWriter::run(EcalCondDBInterface* _db, MonRunIOV& _iov)
  {
    return true;
  }

  RawDataWriter::RawDataWriter() :
    DBWriterWorker("RawData")
  {
    runTypes_.insert("COSMIC");
    runTypes_.insert("BEAM");
    runTypes_.insert("MTCC");
    runTypes_.insert("LASER");
    runTypes_.insert("TEST_PULSE");
    runTypes_.insert("PEDESTAL");
    runTypes_.insert("PEDESTAL-OFFSET");
    runTypes_.insert("LED");
    runTypes_.insert("PHYSICS");
  }

  bool
  RawDataWriter::run(EcalCondDBInterface* _db, MonRunIOV& _iov)
  {
    return true;
  }

  OccupancyWriter::OccupancyWriter() :
    DBWriterWorker("Occupancy")
  {
    runTypes_.insert("COSMIC");
    runTypes_.insert("BEAM");
    runTypes_.insert("MTCC");
    runTypes_.insert("LASER");
    runTypes_.insert("TEST_PULSE");
    runTypes_.insert("PEDESTAL");
    runTypes_.insert("PEDESTAL-OFFSET");
    runTypes_.insert("LED");
    runTypes_.insert("PHYSICS");
  }

  bool
  OccupancyWriter::run(EcalCondDBInterface* _db, MonRunIOV& _iov)
  {
    return true;
  }

}