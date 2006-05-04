#ifndef DTSegment_DTRecSegment4DProducer_H
#define DTSegment_DTRecSegment4DProducer_H

/** \class DTRecSegment4DProducer
 *  Builds the segments in the DT chambers.
 *
 *  $Date: 2006/04/21 14:25:38 $
 *  $Revision: 1.2 $
 * \author Riccardo Bellan - INFN Torino <riccardo.bellan@cern.ch>
 */

#include "FWCore/Framework/interface/EDProducer.h"

namespace edm {
  class ParameterSet;
  class Event;
  class EventSetup;
}
class DTRecSegment4DBaseAlgo;

class DTRecSegment4DProducer: public edm::EDProducer {
public:
  /// Constructor
  DTRecSegment4DProducer(const edm::ParameterSet&) ;

  /// Destructor
  virtual ~DTRecSegment4DProducer();

  // Operations

  /// The method which produces the 4D rec segments
  virtual void produce(edm::Event& event, const edm::EventSetup& setup);
  

protected:

private:

  // Switch on verbosity
  bool debug;

  std::string theRecHits1DLabel;
  //static std::string theAlgoName;
  std::string theRecHits2DLabel;
  // The 4D-segments reconstruction algorithm
  DTRecSegment4DBaseAlgo* the4DAlgo;
};
#endif


