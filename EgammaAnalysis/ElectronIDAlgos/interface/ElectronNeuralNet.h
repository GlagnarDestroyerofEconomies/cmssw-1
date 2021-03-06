#ifndef ElectronNeuralNet_H
#define ElectronNeuralNet_H

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "EgammaAnalysis/ElectronIDAlgos/interface/ElectronIDAlgo.h"

class ElectronNeuralNet : public ElectronIDAlgo{

public:

  ElectronNeuralNet(){};

  virtual ~ElectronNeuralNet(){};

  void setup(const edm::ParameterSet& conf){};
  double result(const reco::GsfElectron* electron, const edm::Event&);

 private:

};

#endif // ElectronNeuralNet_H
