#include "EgammaAnalysis/ElectronTools/interface/ElectronEPcombinator.h"
#include <iostream>

//Accessor to the combination results
void ElectronEPcombinator::combine(){

	computeEPcombination();
	electron_.setCombinedMomentum(combinedMomentum_);
	electron_.setCombinedMomentumError(combinedMomentumError_);
}

//Core code to compute the EP combination
void ElectronEPcombinator::computeEPcombination(){

  scEnergy_ = electron_.getSCEnergy();
  scEnergyError_ = electron_.getSCEnergyError();
  trackerMomentum_ = electron_.getTrackerMomentum();
  trackerMomentumError_ = electron_.getTrackerMomentumError();
  elClass_ = electron_.getElClass();

  combinedMomentum_ = scEnergy_; // initial
  combinedMomentumError_ = 999.;
  
  std::cout<<"Combinatior initialized"<<std::endl;
  // first check for large errors
 
  if (trackerMomentumError_/trackerMomentum_ > 0.5 && scEnergyError_/scEnergy_ <= 0.5) {
    combinedMomentum_ = scEnergy_;    combinedMomentumError_ = scEnergyError_;
   }
  else if (trackerMomentumError_/trackerMomentum_ <= 0.5 && scEnergyError_/scEnergy_ > 0.5){
    combinedMomentum_ = trackerMomentum_;  combinedMomentumError_ = trackerMomentumError_;
   }
  else if (trackerMomentumError_/trackerMomentum_ > 0.5 && scEnergyError_/scEnergy_ > 0.5){
    if (trackerMomentumError_/trackerMomentum_ < scEnergyError_/scEnergy_) {
      combinedMomentum_ = trackerMomentum_; combinedMomentumError_ = trackerMomentumError_;
     }
    else{
      combinedMomentum_ = scEnergy_; combinedMomentumError_ = scEnergyError_;
     }
  }
  
  // then apply the combination algorithm
  else {

     // calculate E/p and corresponding error
    float eOverP = scEnergy_ / trackerMomentum_;
    float errorEOverP = sqrt(
			     (scEnergyError_/trackerMomentum_)*(scEnergyError_/trackerMomentum_) +
			     (scEnergy_*trackerMomentumError_/trackerMomentum_/trackerMomentum_)*
			     (scEnergy_*trackerMomentumError_/trackerMomentum_/trackerMomentum_));

    bool eleIsNotInCombination = false ;
     if ( (eOverP  > 1 + 2.5*errorEOverP) || (eOverP  < 1 - 2.5*errorEOverP) || (eOverP < 0.8) || (eOverP > 1.3) )
      { eleIsNotInCombination = true ; }
     if (eleIsNotInCombination)
      {
       if (eOverP > 1)
        { combinedMomentum_ = scEnergy_ ; combinedMomentumError_ = scEnergyError_ ; }
       else
        {
         if (elClass_==0) // == reco::GsfElectron::GOLDEN)
          { combinedMomentum_ = scEnergy_; combinedMomentumError_ = scEnergyError_; }
         if (elClass_ == 1) //reco::GsfElectron::BIGBREM)
          {
           if (scEnergy_<36)
            { combinedMomentum_ = trackerMomentum_ ; combinedMomentumError_ = trackerMomentumError_ ; }
           else
            { combinedMomentum_ = scEnergy_ ; combinedMomentumError_ = scEnergyError_ ; }
          }
         if (elClass_==2) // == reco::GsfElectron::BADTRACK)
          { combinedMomentum_ = scEnergy_; combinedMomentumError_ = scEnergyError_ ; }
         if (elClass_ == 3) //reco::GsfElectron::SHOWERING)
          {
           if (scEnergy_<30)
            { combinedMomentum_ = trackerMomentum_ ; combinedMomentumError_ = trackerMomentumError_; }
           else
            { combinedMomentum_ = scEnergy_; combinedMomentumError_ = scEnergyError_;}
          }
         if (elClass_ == 4) //reco::GsfElectron::GAP)
          {
           if (scEnergy_<60)
            { combinedMomentum_ = trackerMomentum_ ; combinedMomentumError_ = trackerMomentumError_ ; }
           else
            { combinedMomentum_ = scEnergy_; combinedMomentumError_ = scEnergyError_ ; }
          }
        }
      }
 
     else
      {
       // combination
       combinedMomentum_ = (scEnergy_/scEnergyError_/scEnergyError_ + trackerMomentum_/trackerMomentumError_/trackerMomentumError_) /
         (1/scEnergyError_/scEnergyError_ + 1/trackerMomentumError_/trackerMomentumError_);
       float combinedMomentum_Variance = 1 / (1/scEnergyError_/scEnergyError_ + 1/trackerMomentumError_/trackerMomentumError_);
       combinedMomentumError_ = sqrt(combinedMomentum_Variance);
  std::cout<<"Combinaed momentum "<<combinedMomentum_<<" Combined momentum error "<<combinedMomentumError_<<std::endl;
      }
  } 

}
