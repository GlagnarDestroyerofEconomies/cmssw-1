#include "SimCalorimetry/HcalTrigPrimAlgos/interface/HcalTriggerPrimitiveAlgo.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include <iostream>
#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "Geometry/HcalTowerAlgo/interface/HcalTrigTowerGeometry.h"
#include "DataFormats/HcalDetId/interface/HcalTrigTowerDetId.h"
#include "DataFormats/FEDRawData/interface/FEDNumbering.h"
#include "DataFormats/HcalDetId/interface/HcalElectronicsId.h"
#include "EventFilter/HcalRawToDigi/interface/HcalDCCHeader.h"
#include "EventFilter/HcalRawToDigi/interface/HcalHTRData.h"

using namespace std;

HcalTriggerPrimitiveAlgo::HcalTriggerPrimitiveAlgo( bool pf, const std::vector<double>& w, int latency,
                                                    uint32_t FG_threshold, uint32_t ZS_threshold,
                                                    int numberOfSamples, int numberOfPresamples,
                                                    uint32_t minSignalThreshold, uint32_t PMT_NoiseThreshold)
                                                   : incoder_(0), outcoder_(0),
                                                   theThreshold(0), peakfind_(pf), weights_(w), latency_(latency),
                                                   FG_threshold_(FG_threshold), ZS_threshold_(ZS_threshold),
                                                   numberOfSamples_(numberOfSamples),
                                                   numberOfPresamples_(numberOfPresamples),
                                                   minSignalThreshold_(minSignalThreshold),
                                                   PMT_NoiseThreshold_(PMT_NoiseThreshold) {
   //No peak finding setting (for Fastsim)
   if (!peakfind_){
      numberOfSamples_ = 1; 
      numberOfPresamples_ = 0;
   }
}


HcalTriggerPrimitiveAlgo::~HcalTriggerPrimitiveAlgo() {
}


void HcalTriggerPrimitiveAlgo::run(const HcalTPGCoder* incoder,
                                   const HcalTPGCompressor* outcoder,
                                   const HBHEDigiCollection& hbheDigis,
                                   const HFDigiCollection& hfDigis,
                                   HcalTrigPrimDigiCollection& result) {

   incoder_=dynamic_cast<const HcaluLUTTPGCoder*>(incoder);
   outcoder_=outcoder;

   theSumMap.clear();
   theTowerMapFGSum.clear();
   HF_Veto.clear();
   fgMap_.clear();

   // do the HB/HE digis
   for(HBHEDigiCollection::const_iterator hbheItr = hbheDigis.begin();
   hbheItr != hbheDigis.end(); ++hbheItr) {
      addSignal(*hbheItr);
   }

   // and the HF digis
   for(HFDigiCollection::const_iterator hfItr = hfDigis.begin();
   hfItr != hfDigis.end(); ++hfItr) {
      addSignal(*hfItr);

   }

   for(SumMap::iterator mapItr = theSumMap.begin(); mapItr != theSumMap.end(); ++mapItr) {
      result.push_back(HcalTriggerPrimitiveDigi(mapItr->first));
      HcalTrigTowerDetId detId(mapItr->second.id());
      if(detId.ietaAbs() >= theTrigTowerGeometry.firstHFTower())
         { analyzeHF(mapItr->second, result.back());}
         else{analyze(mapItr->second, result.back());}
   }
   return;
}


void HcalTriggerPrimitiveAlgo::addSignal(const HBHEDataFrame & frame) {
   //Hack for 300_pre10, should be removed.
   if (frame.id().depth()==5) return;

   std::vector<HcalTrigTowerDetId> ids = theTrigTowerGeometry.towerIds(frame.id());
   assert(ids.size() == 1 || ids.size() == 2);
   IntegerCaloSamples samples1(ids[0], int(frame.size()));

   samples1.setPresamples(frame.presamples());
   incoder_->adc2Linear(frame, samples1);

   std::vector<bool> msb;
   incoder_->lookupMSB(frame, msb);

   if(ids.size() == 2) {
      // make a second trigprim for the other one, and split the energy
      IntegerCaloSamples samples2(ids[1], samples1.size());
      for(int i = 0; i < samples1.size(); ++i) {
         samples1[i] = uint32_t(samples1[i]*0.5);
         samples2[i] = samples1[i];
      }
      samples2.setPresamples(frame.presamples());
      addSignal(samples2);
      addFG(ids[1], msb);
   }
   addSignal(samples1);
   addFG(ids[0], msb);
}


void HcalTriggerPrimitiveAlgo::addSignal(const HFDataFrame & frame) {

   if(frame.id().depth() == 1 || frame.id().depth() == 2) {
      std::vector<HcalTrigTowerDetId> ids = theTrigTowerGeometry.towerIds(frame.id());
      assert(ids.size() == 1);
      IntegerCaloSamples samples(ids[0], frame.size());
      samples.setPresamples(frame.presamples());
      incoder_->adc2Linear(frame, samples);

      // Don't add to final collection yet
      // HF PMT veto sum is calculated in analyzerHF()
      IntegerCaloSamples zero_samples(ids[0], frame.size());
      zero_samples.setPresamples(frame.presamples());
      addSignal(zero_samples);

      // Mask off depths: fgid is the same for both depths
      uint32_t fgid = (frame.id().rawId() | 0x1c000) ;

      if ( theTowerMapFGSum.find(ids[0]) == theTowerMapFGSum.end() ) {
         SumFGContainer sumFG;
         theTowerMapFGSum.insert(std::pair<HcalTrigTowerDetId, SumFGContainer >(ids[0], sumFG));
      }

      SumFGContainer& sumFG = theTowerMapFGSum[ids[0]];
      SumFGContainer::iterator sumFGItr;
      for ( sumFGItr = sumFG.begin(); sumFGItr != sumFG.end(); ++sumFGItr) {
         if (sumFGItr->id() == fgid) break;
      }
      // If find
      if (sumFGItr != sumFG.end()) {
         for (int i=0; i<samples.size(); ++i) (*sumFGItr)[i] += samples[i];
      }
      else {
         //Copy samples (change to fgid)
         IntegerCaloSamples sumFGSamples(DetId(fgid), samples.size());
         sumFGSamples.setPresamples(samples.presamples());
         for (int i=0; i<samples.size(); ++i) sumFGSamples[i] = samples[i];
         sumFG.push_back(sumFGSamples);
      }

      // set veto to true if Long or Short less than threshold
      if (HF_Veto.find(fgid) == HF_Veto.end()) {
         vector<bool> vetoBits(samples.size(), false);
         HF_Veto[fgid] = vetoBits;
      }
      for (int i=0; i<samples.size(); ++i)
         if (samples[i] < minSignalThreshold_)
            HF_Veto[fgid][i] = true;
   }
}


void HcalTriggerPrimitiveAlgo::addSignal(const IntegerCaloSamples & samples) {
   HcalTrigTowerDetId id(samples.id());
   SumMap::iterator itr = theSumMap.find(id);
   if(itr == theSumMap.end()) {
      theSumMap.insert(std::make_pair(id, samples));
   }
   else {
      // wish CaloSamples had a +=
      for(int i = 0; i < samples.size(); ++i) {
         (itr->second)[i] += samples[i];
      }
   }
}


void HcalTriggerPrimitiveAlgo::analyze(IntegerCaloSamples & samples, HcalTriggerPrimitiveDigi & result) {
   int shrink = weights_.size() - 1;
   std::vector<bool> finegrain(numberOfSamples_,false);
   std::vector<bool>& msb = fgMap_[samples.id()];
   IntegerCaloSamples sum(samples.id(), samples.size());

   //slide algo window
   for(int ibin = 0; ibin < int(samples.size())- shrink; ++ibin) {
      int algosumvalue = 0;
      for(unsigned int i = 0; i < weights_.size(); i++) {
         //add up value * scale factor
         algosumvalue += int(samples[ibin+i] * weights_[i]);
      }
      if (algosumvalue<0) sum[ibin]=0;            // low-side
                                                  //high-side
      else if (algosumvalue>0x3FF) sum[ibin]=0x3FF;
      else sum[ibin] = algosumvalue;              //assign value to sum[]
   }

   IntegerCaloSamples output(samples.id(), numberOfSamples_);
   output.setPresamples(numberOfPresamples_);

   // Align digis and TP
   int shift = samples.presamples() - numberOfPresamples_;
   assert (shift >= (peakfind_ ? shrink : 0));
   assert((shift + numberOfSamples_ + shrink) <= samples.size());

   for (int ibin = 0; ibin < numberOfSamples_; ++ibin) {
      // ibin - index for output TP
      // idx - index for samples + shift
      int idx = ibin + shift;

      //Peak finding
      if (peakfind_) {
         if ( samples[idx] > samples[idx-1] && samples[idx] >= samples[idx+1] && samples[idx] > theThreshold) {
            output[ibin] = sum[idx];
            finegrain[ibin] = msb[idx];
         }
         // Not a peak
         else output[ibin] = 0;
      }
      else { // No peak finding, just output running sum
         output[ibin] = sum[idx];               
         finegrain[ibin] = msb[idx];
      }

      //Pegged
      if (samples[idx] > 0x3FF) output[ibin] = 0x3FF;

      outcoder_->compress(output, finegrain, result);
   }
}


void HcalTriggerPrimitiveAlgo::analyzeHF(IntegerCaloSamples & samples, HcalTriggerPrimitiveDigi & result) {
   std::vector<bool> finegrain(numberOfSamples_, false);
   HcalTrigTowerDetId detId(samples.id());

   // Align digis and TP
   int shift = samples.presamples() - numberOfPresamples_;
   assert(shift >=  0);
   assert((shift + numberOfSamples_) <=  samples.size());

   TowerMapFGSum::const_iterator tower2fg = theTowerMapFGSum.find(detId);
   assert(tower2fg != theTowerMapFGSum.end());

   const SumFGContainer& sumFG = tower2fg->second;
   // Loop over all L+S pairs that mapped from samples.id()
   // Note: 1 samples.id() = 6 x (L+S) without noZS
   for (SumFGContainer::const_iterator sumFGItr = sumFG.begin(); sumFGItr != sumFG.end(); ++sumFGItr) {
      const std::vector<bool>& veto = HF_Veto[sumFGItr->id().rawId()];
      for (int ibin = 0; ibin < numberOfSamples_; ++ibin) {
         int idx = ibin + shift;
         // if not vetod, add L+S to total sum and calculate FG
         if (!(veto[idx] && (*sumFGItr)[idx] > PMT_NoiseThreshold_)) {
            samples[idx] += (*sumFGItr)[idx];
            finegrain[ibin] = (finegrain[ibin] || (*sumFGItr)[idx] >= FG_threshold_);
         }
      }
   }

   IntegerCaloSamples output(samples.id(), numberOfSamples_);
   output.setPresamples(numberOfPresamples_);

   for (int ibin = 0; ibin < numberOfSamples_; ++ibin) {
      int idx = ibin + shift;
      // TODO get LSB from DB.
      output[ibin] = samples[idx] / 4;
      if (output[ibin] > 0x3FF) output[ibin] = 0x3FF;
   }
   outcoder_->compress(output, finegrain, result);
}

void HcalTriggerPrimitiveAlgo::runZS(HcalTrigPrimDigiCollection & result){
   for (HcalTrigPrimDigiCollection::iterator tp = result.begin(); tp != result.end(); ++tp){
      bool ZS = true;
      for (int i=0; i<tp->size(); ++i) {
         if (tp->sample(i).compressedEt()  > ZS_threshold_) {
            ZS=false;
            break;
         }
      }
      if (ZS) tp->setZSInfo(false,true);
      else tp->setZSInfo(true,false);
   }
}

void HcalTriggerPrimitiveAlgo::runFEFormatError(const FEDRawDataCollection* rawraw,
                                                const HcalElectronicsMap *emap,
                                                HcalTrigPrimDigiCollection & result
                                                ){
  std::set<uint32_t> FrontEndErrors;

  for(int i=FEDNumbering::getHcalFEDIds().first; i<=FEDNumbering::getHcalFEDIds().second; ++i) {
    const FEDRawData& raw = rawraw->FEDData(i);
    if (raw.size()<12) continue;
    const HcalDCCHeader* dccHeader=(const HcalDCCHeader*)(raw.data());
    if(!dccHeader) continue;
    HcalHTRData htr;
    for (int spigot=0; spigot<HcalDCCHeader::SPIGOT_COUNT; spigot++) {
      if (!dccHeader->getSpigotPresent(spigot)) continue;
      dccHeader->getSpigotData(spigot,htr,raw.size());
      int dccid = dccHeader->getSourceId();
      int errWord = htr.getErrorsWord() & 0x1FFFF;
      bool HTRError = (!htr.check() || htr.isHistogramEvent() || (errWord & 0x800)!=0);

      if(HTRError) {
        bool valid =false;
        for(int fchan=0; fchan<3 && !valid; fchan++) {
          for(int fib=0; fib<9 && !valid; fib++) {
            HcalElectronicsId eid(fchan,fib,spigot,dccid-FEDNumbering::getHcalFEDIds().first);
            eid.setHTR(htr.readoutVMECrateId(),htr.htrSlot(),htr.htrTopBottom());
            DetId detId = emap->lookup(eid);
            if(detId.null()) continue;
            HcalSubdetector subdet=(HcalSubdetector(detId.subdetId()));
            if (detId.det()!=4||
              (subdet!=HcalBarrel && subdet!=HcalEndcap &&
              subdet!=HcalForward )) continue;
            std::vector<HcalTrigTowerDetId> ids = theTrigTowerGeometry.towerIds(detId);
            for (std::vector<HcalTrigTowerDetId>::const_iterator triggerId=ids.begin(); triggerId != ids.end(); ++triggerId) {
              FrontEndErrors.insert(triggerId->rawId());
            }
            //valid = true;
          }
        }
      }
    }
  }

  // Loop over TP collection
  // Set TP to zero if there is FE Format Error
  HcalTriggerPrimitiveSample zeroSample(0);
  for (HcalTrigPrimDigiCollection::iterator tp = result.begin(); tp != result.end(); ++tp){
    if (FrontEndErrors.find(tp->id().rawId()) != FrontEndErrors.end()) {
      for (int i=0; i<tp->size(); ++i) tp->setSample(i, zeroSample);
    }
  }
}

void HcalTriggerPrimitiveAlgo::addFG(const HcalTrigTowerDetId& id, std::vector<bool>& msb){
   FGbitMap::iterator itr = fgMap_.find(id);
   if (itr != fgMap_.end()){
      std::vector<bool>& _msb = itr->second;
      for (size_t i=0; i<msb.size(); ++i)
         _msb[i] = _msb[i] || msb[i];
   }
   else fgMap_[id] = msb;
}
