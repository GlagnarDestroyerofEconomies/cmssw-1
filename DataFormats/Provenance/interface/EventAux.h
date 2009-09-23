#ifndef DataFormats_Provenance_EventAux_h
#define DataFormats_Provenance_EventAux_h

#include <iosfwd>

#include "DataFormats/Provenance/interface/ProcessHistoryID.h"
#include "DataFormats/Provenance/interface/EventID.h"
#include "DataFormats/Provenance/interface/Timestamp.h"

// Auxiliary event data that is persistent
// Obsolete format, used for backward compatibility only.

namespace edm {
  struct EventAuxiliary;
  struct EventAux {
    EventAux() : processHistoryID_(), id_(), time_() {}
    ~EventAux() {}
    mutable ProcessHistoryID processHistoryID_;
    EventID id_;
    Timestamp time_;
  };
  void conversion(EventAux const& from, EventAuxiliary & to);
}
#endif
