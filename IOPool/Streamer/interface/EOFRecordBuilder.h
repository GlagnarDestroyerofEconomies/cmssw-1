#ifndef _EOFRecordBuilder_h
#define _EOFRecordBuilder_h

#include "IOPool/Streamer/interface/MsgHeader.h"
#include "IOPool/Streamer/interface/EOFRecord.h"
#include "IOPool/Streamer/interface/MsgTools.h"

// ------------------ EOF message builder ----------------
// Receives info in CTOR and turns it into a writeable buffer 
//Returns address of buffer and its size. 

class EOFRecordBuilder
{
public:

  EOFRecordBuilder(uint32 run, uint32 events, 
                   uint32 statusCode,
                   const std::vector<uint32>& hltStats,
                   uint64 first_event_offset,
                   uint64 last_event_offset);

  uint8* recAddress() const { return (uint8*) &buf_[0]; }
  uint32 size() const {return (uint32) buf_.size(); }

private:
  std::vector<uint8> buf_;
};

#endif

