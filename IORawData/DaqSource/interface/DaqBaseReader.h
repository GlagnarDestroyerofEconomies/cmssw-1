#ifndef DaqBaseReader_H
#define DaqBaseReader_H

/** \class DaqBaseReader
 *  Base class for a "data reader" for the DaqSource.  
 *
 *  Derived classes must have a constructor accepting a
 *  parameter (const edm::ParameterSet& pset).
 *  $Date: $
 *  $Revision: $
 *  \author N. Amapane - CERN
 */

namespace raw {class FEDRawDataCollection; }
namespace edm {class EventID; class Timestamp;   class ParameterSet;}

class DaqBaseReader {
public:
  /// Constructor
  DaqBaseReader() {}

  /// Destructor
  virtual ~DaqBaseReader() {}
  

  /// Fill in the raw data 
  virtual bool fillRawData(edm::EventID& eID,
			   edm::Timestamp& tstamp, 
			   raw::FEDRawDataCollection& data) = 0;  

private:

};
#endif

