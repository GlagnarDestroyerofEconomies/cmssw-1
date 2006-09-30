#ifndef CSCRecHitB_CSCHitFromStripOnly_h
#define CSCRecHitB_CSCHitFromStripOnly_h

/** \class CSCHitFromStripOnly
 *
 * Search for strip with ADC output exceeding theThresholdForAPeak.  For each of these strips,
 * build a cluster of strip of size theClusterSize (typically 5 strips).  Finally, make
 * a Strip Hit out of these clusters by finding the center-of-mass position of the hit
 * The DetId, strip hit position, and peaking time are stored in a CSCStripHit collection.
 *
 * Here one has to be careful with the ME_1/a chambers:  in MC, digis are produced only for the first 16
 * strips, so one has to account for the ganging in groups of 3.
 *
 * In data, the ME_11 digis are stored in the same collection, so one has to untangle the output from
 * the ME_1a and ME_1b strips.  64 readouts from ME_1b, 16 from ME_1a.  Will have to figure out if ME_1a comes
 * first, and then the 64 ME_1b...
 *
 * \author Dominique Fortin - UCR
 *
 */
#include <RecoLocalMuon/CSCRecHitB/src/CSCStripData.h>

#include <DataFormats/CSCRecHit/interface/CSCStripHit.h>
#include <DataFormats/CSCDigi/interface/CSCStripDigiCollection.h>

#include <FWCore/Framework/interface/Frameworkfwd.h>
#include <FWCore/ParameterSet/interface/ParameterSet.h>

#include <vector>

class CSCLayer;
class CSCChamberSpecs;
class CSCLayerGeometry;
class CSCGains;
class CSCcrosstalk;
class CSCNoiseMatrix;
class CSCStripDigi;
class CSCPeakBinOfStripPulse;
class CSCCalibrateStrip;
//class CSCFindPeakTime;

class CSCHitFromStripOnly 
{
  
 public:

  typedef std::vector<CSCStripData> PulseHeightMap;
  
  explicit CSCHitFromStripOnly( const edm::ParameterSet& ps );
  
  ~CSCHitFromStripOnly();
  
  std::vector<CSCStripHit> runStrip( const CSCDetId& id, const CSCLayer* layer, const CSCStripDigiCollection::Range& rstripd);

  void setCalibration( const CSCGains* gains,
                       const CSCcrosstalk* xtalk,
                       const CSCNoiseMatrix* noise ) {
    gains_ = gains;
    xtalk_ = xtalk;
    noise_ = noise;
  }

  
 protected:
  
  /// Go through strip in layers and build a table with 
  void fillPulseHeights( const CSCStripDigiCollection::Range& );  

  /// Compute crosstalk correction to strip adc
  void correctForCrosstalk( const CSCStripDigiCollection::Range& rstripd );

  /// Get crosstalk level for MC for a given tbin
  float crosstalkLevel( const CSCStripDigi& digi, const int& tbin );

  /// Find local maxima
  void findMaxima();    

  /// Make clusters using local maxima
  float makeCluster( int centerStrip );


  std::vector<int> theMaxima;

  PulseHeightMap thePulseHeightMap;
  

  /// Find position of hit in strip cluster in terms of strip #
  float findHitOnStripPosition( const std::vector<CSCStripData>& data, const int& centerStrip );
  
  CSCDetId id_;    
  const CSCLayer * layer_;
  const CSCLayerGeometry * layergeom_;
  const CSCChamberSpecs * specs_;
  
 private:
  
  CSCStripData makeStripData( int centerStrip, int offset, int stripIndex );


  // Variables entering the CSCStripHit construction:
  int tmax_cluster;
  int ClusterSize;
  std::vector<float> strips_adc;  
  
  // The cuts for forming the strip hits are described in the data/.cfi file
  bool debug;
  bool isData;
  int theClusterSize;
  float theThresholdForAPeak;
  float theThresholdForCluster;

  /// These are the gain correction weights and X-talks read in from database.
  float globalGainAvg;
  float gainWeight[100];
  float slopeRight[100];
  float slopeLeft[100];
  float interRight[100];
  float interLeft[100];  
  // Peaking time for strip hit
  int TmaxOfCluster;            // in time bins;
  float peakTime;               // in nanosec
  // Number of strips in layer
  unsigned Nstrips;

  /* Cache calibrations for current event
   *
   */
  const CSCGains*       gains_;
  const CSCcrosstalk*   xtalk_;
  const CSCNoiseMatrix* noise_;


  CSCPeakBinOfStripPulse* pulseheightOnStripFinder_;
  CSCCalibrateStrip*      calibrateStrip_;
//  CSCFindPeakTime*        peakTimeFinder_;

};

#endif

