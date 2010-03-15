#ifndef RecoLocalCalo_EcalRecAlgos_ESRecHitFitAlgo_HH
#define RecoLocalCalo_EcalRecAlgos_ESRecHitFitAlgo_HH

#include "DataFormats/EcalDigi/interface/ESDataFrame.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHit.h"
#include "CondFormats/ESObjects/interface/ESPedestals.h"
#include "CondFormats/ESObjects/interface/ESIntercalibConstants.h"
#include "CondFormats/ESObjects/interface/ESChannelStatus.h"
#include "CondFormats/ESObjects/interface/ESRecHitRatioCuts.h"

#include "TF1.h"

class ESRecHitFitAlgo {

 public:

  ESRecHitFitAlgo();
  ~ESRecHitFitAlgo();

  void setESGain(const double& value) { gain_ = value; }
  void setMIPGeV(const double& value) { MIPGeV_ = value; } 
  void setPedestals(const ESPedestals& peds) { peds_ = peds; }
  void setIntercalibConstants(const ESIntercalibConstants& mips) { mips_ = mips; }
  void setChannelStatus(const ESChannelStatus& status) { channelStatus_ = status; }
  void setRatioCuts(const ESRecHitRatioCuts& ratioCuts) { ratioCuts_ = ratioCuts; }

  double* EvalAmplitude(const ESDataFrame& digi, double ped) const;
  EcalRecHit reconstruct(const ESDataFrame& digi) const;

 private:

  TF1 *fit_;
  double gain_;
  ESPedestals peds_;
  ESIntercalibConstants mips_;
  ESChannelStatus channelStatus_;
  ESRecHitRatioCuts ratioCuts_;
  double MIPGeV_;

};

#endif
