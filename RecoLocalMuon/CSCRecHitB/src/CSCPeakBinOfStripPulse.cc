// This is CSCPeakBinOfStripPulse

#include <RecoLocalMuon/CSCRecHitB/src/CSCPeakBinOfStripPulse.h>

#include <DataFormats/CSCDigi/interface/CSCStripDigi.h>

#include <FWCore/ParameterSet/interface/ParameterSet.h>

#include <vector>


CSCPeakBinOfStripPulse::CSCPeakBinOfStripPulse( const edm::ParameterSet& ps ) {

  debug                      = ps.getUntrackedParameter<bool>("CSCDebug");

}


CSCPeakBinOfStripPulse::~CSCPeakBinOfStripPulse() {

}


/* peakAboveBaseline
 *
 * This finds the seed for the cluster, 
 *
 */
bool CSCPeakBinOfStripPulse::peakAboveBaseline( const CSCStripDigi& digi, float* height, int& tmax) const {
  
  std::vector<int> sca = digi.getADCCounts();
  
  tmax   = -1;
  for (int j = 0; j<4; j++) height[j] = 0.;

  if ( sca.empty() ) return false;

  // First find maximum time bin
  tmax = std::max_element( sca.begin(), sca.end() ) - sca.begin();

  // Maximum cannot occur in 2 first time bins
  if ( tmax < 1 ) return false;

  // Find pedestal
  float ped = baseline( digi );

  // Now find adc signal after pedestal subtraction for 4 time bins around peak [tmax-1, tmax+2]
  int i = 0;
  for ( int t = tmax-1; t < tmax+3; t++ ) {
    height[i] = sca[t] - ped;
    i++; 
  }    

  return true;
}



/* baseline
 *
 */
float CSCPeakBinOfStripPulse::baseline(const CSCStripDigi& digi) const {
    std::vector<int> sca = digi.getADCCounts();
    return ( sca[0]+sca[1] ) / 2.;
}


