#ifndef ExpoRandomPtGunProducer_H
#define ExpoRandomPtGunProducer_H

/** \class ExpoRandomPtGunProducer
 *
 * Generates single particle gun in HepMC format
 * Jean-Roch Vlimant
 ***************************************/

#include "IOMC/ParticleGuns/interface/BaseFlatGunProducer.h"
#include "CLHEP/Random/RandExponential.h"
namespace edm
{
  
  class ExpoRandomPtGunProducer : public BaseFlatGunProducer
  {
  
  public:
    ExpoRandomPtGunProducer(const ParameterSet & pset);
    virtual ~ExpoRandomPtGunProducer();

  private:
   
    virtual void produce(Event & e, const EventSetup& es);
    
  protected :
  
    // data members
    
    double            fMinPt   ;
    double            fMaxPt   ;
    double            fMeanPt ;
    RandExponential * fRandomExpoGenerator;

  };
} 

#endif
