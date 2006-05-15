//-------------------------------------------------
//
/** \class L1MuGMTLFDeltaEtaLUT
 *
 *   LFDeltaEta look-up table
 *          
 *   this class was automatically generated by 
 *     L1MuGMTLUT::MakeSubClass()  
*/ 
//   $Date: 2004/02/03 16:33:44 $
//   $Revision: 1.2 $
//
//   Author :
//   H. Sakulin            HEPHY Vienna
//
//   Migrated to CMSSW:
//   I. Mikulec
//
//--------------------------------------------------
#ifndef L1TriggerGlobalMuonTrigger_L1MuGMTLFDeltaEtaLUT_h
#define L1TriggerGlobalMuonTrigger_L1MuGMTLFDeltaEtaLUT_h

//---------------
// C++ Headers --
//---------------


//----------------------
// Base Class Headers --
//----------------------
#include "L1Trigger/GlobalMuonTrigger/src/L1MuGMTLUT.h"

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
class L1MuGMTScales;
class L1MuTriggerScales;


//              ---------------------
//              -- Class Interface --
//              ---------------------

using namespace std;

class L1MuGMTLFDeltaEtaLUT : public L1MuGMTLUT {
  
 public:
  enum {DTRPC, CSCRPC};

  /// constuctor using function-lookup
  L1MuGMTLFDeltaEtaLUT() : L1MuGMTLUT("LFDeltaEta", 
				       "DTRPC CSCRPC",
				       "eta_dtcsc(6) eta_rpc(6)",
				       "delta_eta(4)", 10, false) {
    InitParameters();
  } ;

  /// destructor
  virtual ~L1MuGMTLFDeltaEtaLUT() {};

  /// specific lookup function for delta_eta
  unsigned SpecificLookup_delta_eta (int idx, unsigned eta_dtcsc, unsigned eta_rpc) const {
    vector<unsigned> addr(2);
    addr[0] = eta_dtcsc;
    addr[1] = eta_rpc;
    return Lookup(idx, addr) [0];
  };

  /// specific lookup function for entire output field
  unsigned SpecificLookup (int idx, unsigned eta_dtcsc, unsigned eta_rpc) const {
    vector<unsigned> addr(2);
    addr[0] = eta_dtcsc;
    addr[1] = eta_rpc;
    return LookupPacked(idx, addr);
  };



  /// access to lookup function with packed input and output

  virtual unsigned LookupFunctionPacked (int idx, unsigned address) const {
    vector<unsigned> addr = u2vec(address, m_Inputs);
    return TheLookupFunction(idx ,addr[0] ,addr[1]);

  };

 private:
  /// Initialize scales, configuration parameters, alignment constants, ...
  void InitParameters();

  /// The lookup function - here the functionality of the LUT is implemented
  unsigned TheLookupFunction (int idx, unsigned eta_dtcsc, unsigned eta_rpc) const;

  /// Private data members (LUT parameters);
  L1MuGMTScales *m_theGMTScales; // pointer to L1MuGMTScales Singleton
  L1MuTriggerScales *m_theTriggerScales; // pointer to L1MuTriggerScales Singleton
};
#endif
