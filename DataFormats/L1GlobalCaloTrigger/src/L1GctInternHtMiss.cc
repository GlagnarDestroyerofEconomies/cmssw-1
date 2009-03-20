#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctInternHtMiss.h"


// PUBLIC METHODS

// Default ctor
L1GctInternHtMiss::L1GctInternHtMiss():
  type_(nulltype),
  capBlock_(0),
  capIndex_(0),
  bx_(0),
  data_(0)
{
}

// Destructor
L1GctInternHtMiss::~L1GctInternHtMiss() {}

// Named ctor for making missing Ht x-component object from unpacker raw data.
L1GctInternHtMiss L1GctInternHtMiss::unpackerMissHtx(const uint16_t capBlock,
                                                     const uint16_t capIndex,
                                                     const int16_t bx,
                                                     const uint32_t data)
{
  return L1GctInternHtMiss(miss_htx, capBlock, capIndex, bx, data & kSingleComponentRawMask);
}

// Named ctor for making missing Ht y-component object from unpacker raw data.
L1GctInternHtMiss L1GctInternHtMiss::unpackerMissHty(const uint16_t capBlock,
                                                     const uint16_t capIndex,
                                                     const int16_t bx,
                                                     const uint32_t data)
{
  return L1GctInternHtMiss(miss_hty, capBlock, capIndex, bx, data & kSingleComponentRawMask);
}

// Named ctor for making missing Ht x & y components object from unpacker raw data.
L1GctInternHtMiss L1GctInternHtMiss::unpackerMissHtxHty(const uint16_t capBlock,
                                                        const uint16_t capIndex,
                                                        const int16_t bx,
                                                        const uint32_t data)
{
  return L1GctInternHtMiss(miss_htx_and_hty, capBlock, capIndex, bx, data & kDoubleComponentRawMask);
}

// Get Ht x-component
int16_t L1GctInternHtMiss::htx() const
{
  if(type() == miss_htx)
  { 
    return static_cast<int16_t>(raw() & kSingleComponentHtMask);
  }
  if(type() == miss_htx_and_hty)
  {
    return convert14BitTwosCompTo16Bit(raw() & kDoubleComponentHtMask);
  }
  return 0;
}

// Get Ht y-component
int16_t L1GctInternHtMiss::hty() const
{
  if(type() == miss_hty)
  {
    return static_cast<int16_t>(raw() & kSingleComponentHtMask);
  }
  if(type() == miss_htx_and_hty)
  {
    return convert14BitTwosCompTo16Bit((raw() >> kDoubleComponentHtyShift) & kDoubleComponentHtMask);
  }
  return 0;
}

// Get overflow
bool L1GctInternHtMiss::overflow() const
{
  if(type() == miss_htx || type() == miss_hty) { return (raw() & kSingleComponentOflowMask) != 0; }
  if(type() == miss_htx_and_hty) { return (raw() & kDoubleComponentOflowMask) != 0; }
  return false;
}


// PRIVATE METHODS

L1GctInternHtMiss::L1GctInternHtMiss(const L1GctInternHtMissType type,
                                     const uint16_t capBlock,
                                     const uint16_t capIndex,
                                     const int16_t bx,
                                     const uint32_t data):
  type_(type),
  capBlock_(capBlock),
  capIndex_(capIndex),
  bx_(bx),
  data_(data)
{
}

int16_t L1GctInternHtMiss::convert14BitTwosCompTo16Bit(const uint16_t data) const
{
  // If bit 13 is high, set bits 13, 14, 15 high.
  if((data & 0x2000) != 0) { return static_cast<int16_t>(data | 0xe000); }
  
  // Else, bit 13 must be low, so set bits 13, 14, 15 low.
  return static_cast<int16_t>(data & 0x1fff);
}


// PRETTY PRINTOUT OPERATOR

std::ostream& operator<<(std::ostream& os, const L1GctInternHtMiss& rhs)
{
  os << " L1GctInternHtMiss:  htx=";
  if(rhs.isThereHtx()) { os << rhs.htx(); }
  else { os << "n/a"; }
  os << ", hty=";
  if(rhs.isThereHty()) { os << rhs.hty(); }
  else { os << "n/a"; }
  if (rhs.overflow()) { os << "; overflow set"; }
  os << "; cap block=0x" << std::hex << rhs.capBlock() << std::dec
     << ", index=" << rhs.capIndex()
     << ", BX=" << rhs.bx();
  return os;
}
