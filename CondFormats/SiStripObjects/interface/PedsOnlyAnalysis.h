#ifndef CondFormats_SiStripObjects_PedsOnlyAnalysis_H
#define CondFormats_SiStripObjects_PedsOnlyAnalysis_H

#include "CondFormats/SiStripObjects/interface/CommissioningAnalysis.h"
#include "DataFormats/SiStripCommon/interface/SiStripConstants.h"
#include <boost/cstdint.hpp>
#include <sstream>
#include <vector>

class TH1;

/** 
    @class PedsOnlyAnalysis
    @author M. Wingham, R.Bainbridge
    @brief Histogram-based analysis for pedestal run.
*/
class PedsOnlyAnalysis : public CommissioningAnalysis {
  
 public:

  // ---------- con(de)structors ----------

  PedsOnlyAnalysis( const uint32_t& key );

  PedsOnlyAnalysis();

  virtual ~PedsOnlyAnalysis() {;}

  // ---------- public interface ----------

  /** Identifies if analysis is valid or not. */
  bool isValid() const;
  
  // Pedestal, noise and raw noise (128-strip vector per APV)
  inline const VVFloat& peds() const;
  inline const VVFloat& raw() const;

  // Mean and rms spread (value per APV)
  inline const VFloat& pedsMean() const;
  inline const VFloat& pedsSpread() const;
  inline const VFloat& rawMean() const;
  inline const VFloat& rawSpread() const;

  // Max and min values (value per APV)
  inline const VFloat& pedsMax() const;
  inline const VFloat& pedsMin() const; 
  inline const VFloat& rawMax() const;
  inline const VFloat& rawMin() const;

  inline const Histo& hPeds() const;
  inline const Histo& hNoise() const;

  // ---------- public print methods ----------
  
  /** Prints analysis results. */
  void print( std::stringstream&, uint32_t apv_number = 0 );
  
  /** Overrides base method. */
  void summary( std::stringstream& ) const;

  // ---------- private methods ----------

 private:
  
  /** Resets analysis member data. */
  void reset();

  /** Extracts and organises histograms. */
  void extract( const std::vector<TH1*>& );

  /** Performs histogram anaysis. */
  void analyse();
  
  // ---------- private member data ----------

 private:
  
  // VVFloats means: 1 vector per APV, 1 value per strip.

  /** Peds values. */
  VVFloat peds_;

  /** Raw noise values. */
  VVFloat raw_;

  // VFloat: 1 value per APV

  /** Mean peds value. */
  VFloat pedsMean_;

  /** Rms spread in peds. */
  VFloat pedsSpread_;

  /** Mean raw noise value. */
  VFloat rawMean_;

  /** Rms spread in raw noise. */
  VFloat rawSpread_;

  /** Max peds value. */
  VFloat pedsMax_;

  /** Min peds value. */
  VFloat pedsMin_; 

  /** Max raw noise value. */
  VFloat rawMax_;

  /** Min raw noise value. */
  VFloat rawMin_;
  
  /** Pedestals and raw noise */
  Histo hPeds_;

  /** Residuals and noise */
  Histo hNoise_;

  // true if legacy histogram naming is used
  bool legacy_;
  
};

// ---------- Inline methods ----------

const PedsOnlyAnalysis::VVFloat& PedsOnlyAnalysis::peds() const { return peds_; }
const PedsOnlyAnalysis::VVFloat& PedsOnlyAnalysis::raw() const { return raw_; }

const PedsOnlyAnalysis::VFloat& PedsOnlyAnalysis::pedsMean() const { return pedsMean_; }
const PedsOnlyAnalysis::VFloat& PedsOnlyAnalysis::pedsSpread() const { return pedsSpread_; }
const PedsOnlyAnalysis::VFloat& PedsOnlyAnalysis::rawMean() const { return rawMean_; }
const PedsOnlyAnalysis::VFloat& PedsOnlyAnalysis::rawSpread() const { return rawSpread_; }

const PedsOnlyAnalysis::VFloat& PedsOnlyAnalysis::pedsMax() const { return pedsMax_; }
const PedsOnlyAnalysis::VFloat& PedsOnlyAnalysis::pedsMin() const { return pedsMin_; } 
const PedsOnlyAnalysis::VFloat& PedsOnlyAnalysis::rawMax() const { return rawMax_; }
const PedsOnlyAnalysis::VFloat& PedsOnlyAnalysis::rawMin() const { return rawMin_; }

const PedsOnlyAnalysis::Histo& PedsOnlyAnalysis::hPeds() const { return hPeds_; }
const PedsOnlyAnalysis::Histo& PedsOnlyAnalysis::hNoise() const { return hNoise_; }

#endif // CondFormats_SiStripObjects_PedsOnlyAnalysis_H
