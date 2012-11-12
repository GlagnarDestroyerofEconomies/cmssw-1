#include "../interface/PedestalClient.h"
#include "../interface/EcalDQMClientUtils.h"

#include <iomanip>

#include "DQM/EcalCommon/interface/MESetMulti.h"

#include "DataFormats/EcalDetId/interface/EcalPnDiodeDetId.h"

#include "CondFormats/EcalObjects/interface/EcalDQMStatusHelper.h"

namespace ecaldqm
{

  PedestalClient::PedestalClient(edm::ParameterSet const& _workerParams, edm::ParameterSet const& _commonParams) :
    DQWorkerClient(_workerParams, _commonParams, "PedestalClient"),
    minChannelEntries_(_workerParams.getUntrackedParameter<int>("minChannelEntries")),
    expectedMean_(_workerParams.getUntrackedParameter<double>("expectedMean")),
    toleranceMean_(_workerParams.getUntrackedParameter<double>("toleranceMean")),
    toleranceRMS_(0),
    expectedPNMean_(_workerParams.getUntrackedParameter<double>("expectedPNMean")),
    tolerancePNMean_(_workerParams.getUntrackedParameter<double>("tolerancePNMean")),
    tolerancePNRMS_(0)
  {
    using namespace std;

    vector<int> MGPAGains(_commonParams.getUntrackedParameter<vector<int> >("MGPAGains"));
    vector<int> MGPAGainsPN(_commonParams.getUntrackedParameter<vector<int> >("MGPAGainsPN"));

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

    toleranceRMS_.resize(iMEGain);

    std::vector<double> inToleranceRMS(_workerParams.getUntrackedParameter<std::vector<double> >("toleranceRMS"));

    for(map<int, unsigned>::iterator gainItr(gainToME_.begin()); gainItr != gainToME_.end(); ++gainItr){
      unsigned iME(gainItr->second);
      unsigned iGain(0);
      switch(gainItr->first){
      case 1:
        iGain = 0; break;
      case 6:
        iGain = 1; break;
      case 12:
        iGain = 2; break;
      }

      toleranceRMS_[iME] = inToleranceRMS[iGain];
    }

    tolerancePNRMS_.resize(iMEPNGain);

    std::vector<double> inTolerancePNRMS(_workerParams.getUntrackedParameter<std::vector<double> >("tolerancePNRMS"));

    for(map<int, unsigned>::iterator gainItr(pnGainToME_.begin()); gainItr != pnGainToME_.end(); ++gainItr){
      unsigned iME(gainItr->second);
      unsigned iGain(0);
      switch(gainItr->first){
      case 1:
        iGain = 0; break;
      case 16:
        iGain = 1; break;
      }

      tolerancePNRMS_[iME] = inTolerancePNRMS[iGain];
    }

    map<string, string> replacements;
    stringstream ss;

    std::string apdPlots[] = {"Quality", "Mean", "RMS", "QualitySummary"};
    for(unsigned iS(0); iS < sizeof(apdPlots) / sizeof(std::string); ++iS){
      std::string& plot(apdPlots[iS]);
      MESetMulti* multi(static_cast<MESetMulti*>(MEs_[plot]));

      for(map<int, unsigned>::iterator gainItr(gainToME_.begin()); gainItr != gainToME_.end(); ++gainItr){
        multi->use(gainItr->second);

        ss.str("");
        ss << std::setfill('0') << std::setw(2) << gainItr->first;
        replacements["gain"] = ss.str();

        multi->formPath(replacements);
      }
    }

    std::string pnPlots[] = {"PNRMS", "PNQualitySummary"};
    for(unsigned iS(0); iS < sizeof(pnPlots) / sizeof(std::string); ++iS){
      std::string& plot(pnPlots[iS]);
      MESetMulti* multi(static_cast<MESetMulti*>(MEs_[plot]));

      for(map<int, unsigned>::iterator gainItr(pnGainToME_.begin()); gainItr != pnGainToME_.end(); ++gainItr){
        multi->use(gainItr->second);

        ss.str("");
        ss << std::setfill('0') << std::setw(2) << gainItr->first;
        replacements["pngain"] = ss.str();

        multi->formPath(replacements);
      }
    }

    std::string apdSources[] = {"Pedestal"};
    for(unsigned iS(0); iS < sizeof(apdSources) / sizeof(std::string); ++iS){
      std::string& plot(apdSources[iS]);
      MESetMulti const* multi(static_cast<MESetMulti const*>(sources_[plot]));

      for(map<int, unsigned>::iterator gainItr(gainToME_.begin()); gainItr != gainToME_.end(); ++gainItr){
        multi->use(gainItr->second);

        ss.str("");
        ss << std::setfill('0') << std::setw(2) << gainItr->first;
        replacements["gain"] = ss.str();

        multi->formPath(replacements);
      }
    }

    std::string pnSources[] = {"PNPedestal"};
    for(unsigned iS(0); iS < sizeof(pnSources) / sizeof(std::string); ++iS){
      std::string& plot(pnSources[iS]);
      MESetMulti const* multi(static_cast<MESetMulti const*>(sources_[plot]));

      for(map<int, unsigned>::iterator gainItr(pnGainToME_.begin()); gainItr != pnGainToME_.end(); ++gainItr){
        multi->use(gainItr->second);

        ss.str("");
        ss << std::setfill('0') << std::setw(2) << gainItr->first;
        replacements["pngain"] = ss.str();

        multi->formPath(replacements);
      }
    }

    qualitySummaries_.insert("Quality");
    qualitySummaries_.insert("QualitySummary");
    qualitySummaries_.insert("PNQualitySummary");
  }

  void
  PedestalClient::producePlots()
  {
    using namespace std;

    MESetMulti* meQuality(static_cast<MESetMulti*>(MEs_["Quality"]));
    MESetMulti* meQualitySummary(static_cast<MESetMulti*>(MEs_["QualitySummary"]));
    MESetMulti* meMean(static_cast<MESetMulti*>(MEs_["Mean"]));
    MESetMulti* meRMS(static_cast<MESetMulti*>(MEs_["RMS"]));
    MESetMulti* mePNQualitySummary(static_cast<MESetMulti*>(MEs_["PNQualitySummary"]));
    MESetMulti* mePNRMS(static_cast<MESetMulti*>(MEs_["PNRMS"]));

    MESetMulti const* sPedestal(static_cast<MESetMulti const*>(sources_["Pedestal"]));
    MESetMulti const* sPNPedestal(static_cast<MESetMulti const*>(sources_["PNPedestal"]));

    for(map<int, unsigned>::iterator gainItr(gainToME_.begin()); gainItr != gainToME_.end(); ++gainItr){
      meQuality->use(gainItr->second);
      meQualitySummary->use(gainItr->second);
      meMean->use(gainItr->second);
      meRMS->use(gainItr->second);

      sPedestal->use(gainItr->second);

      meMean->reset();
      meRMS->reset();

      uint32_t mask(0);
      switch(gainItr->first){
      case 1:
        mask |= (1 << EcalDQMStatusHelper::PEDESTAL_LOW_GAIN_MEAN_ERROR |
                 1 << EcalDQMStatusHelper::PEDESTAL_LOW_GAIN_RMS_ERROR);
        break;
      case 6:
        mask |= (1 << EcalDQMStatusHelper::PEDESTAL_MIDDLE_GAIN_MEAN_ERROR |
                 1 << EcalDQMStatusHelper::PEDESTAL_MIDDLE_GAIN_RMS_ERROR);
        break;
      case 12:
        mask |= (1 << EcalDQMStatusHelper::PEDESTAL_HIGH_GAIN_MEAN_ERROR |
                 1 << EcalDQMStatusHelper::PEDESTAL_HIGH_GAIN_RMS_ERROR);
        break;
      default:
        break;
      }

      MESet::iterator qEnd(meQuality->end());
      MESet::const_iterator pItr(sPedestal);
      for(MESet::iterator qItr(meQuality->beginChannel()); qItr != qEnd; qItr.toNextChannel()){

        DetId id(qItr->getId());

        bool doMask(applyMask(meQuality->getBinType(), id, mask));

        pItr = qItr;

        float entries(pItr->getBinEntries());

        if(entries < minChannelEntries_){
          qItr->setBinContent(doMask ? kMUnknown : kUnknown);
          continue;
        }

        float mean(pItr->getBinContent());
        float rms(pItr->getBinError() * sqrt(entries));

        meMean->fill(id, mean);
        meRMS->fill(id, rms);

        if(abs(mean - expectedMean_) > toleranceMean_ || rms > toleranceRMS_[gainItr->second])
          qItr->setBinContent(doMask ? kMBad : kBad);
        else
          qItr->setBinContent(doMask ? kMGood : kGood);
      }

      towerAverage_(meQualitySummary, meQuality, 0.2);
    }

    for(map<int, unsigned>::iterator gainItr(pnGainToME_.begin()); gainItr != pnGainToME_.end(); ++gainItr){
      mePNQualitySummary->use(gainItr->second);
      mePNRMS->use(gainItr->second);

      sPNPedestal->use(gainItr->second);

      mePNRMS->reset();

      uint32_t mask(0);
      switch(gainItr->first){
      case 1:
        mask |= (1 << EcalDQMStatusHelper::PEDESTAL_LOW_GAIN_MEAN_ERROR |
                 1 << EcalDQMStatusHelper::PEDESTAL_LOW_GAIN_RMS_ERROR);
        break;
      case 16:
        mask |= (1 << EcalDQMStatusHelper::PEDESTAL_HIGH_GAIN_MEAN_ERROR |
                 1 << EcalDQMStatusHelper::PEDESTAL_HIGH_GAIN_RMS_ERROR);
        break;
      default:
        break;
      }

      for(unsigned iDCC(0); iDCC < BinService::nDCC; ++iDCC){

        if(memDCCIndex(iDCC + 1) == unsigned(-1)) continue;

        for(unsigned iPN(0); iPN < 10; ++iPN){
          int subdet(0);
          if(iDCC >= kEBmLow && iDCC <= kEBpHigh) subdet = EcalBarrel;
          else subdet = EcalEndcap;

          EcalPnDiodeDetId id(subdet, iDCC + 1, iPN + 1);

          bool doMask(applyMask(mePNQualitySummary->getBinType(), id, mask));

          float entries(sPNPedestal->getBinEntries(id));

          if(entries < minChannelEntries_){
            mePNQualitySummary->setBinContent(id, doMask ? kMUnknown : kUnknown);
            continue;
          }

          float mean(sPNPedestal->getBinContent(id));
          float rms(sPNPedestal->getBinError(id) * sqrt(entries));

          mePNRMS->fill(id, rms);

          if(abs(mean - expectedPNMean_) > tolerancePNMean_ || rms > tolerancePNRMS_[gainItr->second])
            mePNQualitySummary->setBinContent(id, doMask ? kMBad : kBad);
          else
            mePNQualitySummary->setBinContent(id, doMask ? kMGood : kGood);
        }
      }
    }
  }

  DEFINE_ECALDQM_WORKER(PedestalClient);
}
