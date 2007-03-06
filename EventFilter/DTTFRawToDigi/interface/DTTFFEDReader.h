//-------------------------------------------------
//
/**  \class DTTFFEDReader
 *
 *   L1 DT Track Finder Raw-to-Digi
 *
 *
 *   $Date: 2006/06/01 00:00:00 $
 *   $Revision: 1.1 $
 *
 *   J. Troconiz  UAM Madrid
 *   E. Delmeire  UAM Madrid
 */
//
//--------------------------------------------------
#ifndef DTTFRawToDigi_DTTFFEDReader_h
#define DTTFRawToDigi_DTTFFEDReader_h

#include "DataFormats/L1DTTrackFinder/interface/L1MuDTChambPhContainer.h"
#include "DataFormats/L1DTTrackFinder/interface/L1MuDTChambThContainer.h"

#include <FWCore/Framework/interface/EDProducer.h>
#include <FWCore/ParameterSet/interface/ParameterSet.h>

#include <string>

class DTTFFEDReader : public edm::EDProducer {

 public:

  /// Constructor
  DTTFFEDReader(const edm::ParameterSet& pset);

  /// Destructor
  virtual ~DTTFFEDReader();

  /// Produce digis out of raw data
  void produce(edm::Event & e, const edm::EventSetup& c);

  /// Generate and fill FED raw data for a full event
  bool fillRawData(edm::Event& e,
		   L1MuDTChambPhContainer::Phi_Container& phi_data,
		   L1MuDTChambThContainer::The_Container& the_data);

 private:
  
  // Operations

  // access data
  const L1MuDTChambPhContainer::Phi_Container& p_data();

  const L1MuDTChambThContainer::The_Container& t_data();

  // Process one event
  void analyse(edm::Event& e);

  // clear data container
  void clear();

  // process data
  void process(edm::Event& e);

  // data containers
  L1MuDTChambPhContainer::Phi_Container phiSegments;

  L1MuDTChambThContainer::The_Container theSegments;

  // utilities
  int channel(int wheel, int sector, int bx);

  int bxNr(int channel);

  int sector(int channel);

  int wheel(int channel);

  void calcCRC(long long myD, int &myC);

};
#endif
