#ifndef SimMuon_DTDigitizer_h
#define SimMuon_DTDigitizer_h

/** \class DTDigitizer
 *  Digitize the muon drift tubes. 
 *  The parametrisation function MuBarDriftTimeParametrization 
 *  from P.G.Abia, J.Puerta is used in all cases where it is applicable. 
 *
 *  $Date: $
 *  $Revision: $
 *  \author G. Bevilacqua, N. Amapane, G. Cerminara, R. Bellan
 */


#include "FWCore/Framework/interface/EDProducer.h"

#include "DataFormats/DTDigi/interface/DTDigiCollection.h"
#include "DataFormats/MuonDetId/interface/DTDetId.h"

#include <vector>

class DTGeomDetUnit;
class PSimHit;
class DTWireType;
class DTBaseDigiSync;
class DTTopology;
namespace edm {class ParameterSet; class Event; class EventSetup;}

class DTDigitizer : public edm::EDProducer {
  
 public:
  explicit DTDigitizer(const edm::ParameterSet&);
  ~DTDigitizer();
  virtual void produce(edm::Event&, const edm::EventSetup&);
  
 private:
  typedef std::pair<const PSimHit*,float> hitAndT; // hit & corresponding time
  typedef std::vector<hitAndT> TDContainer; // hits & times for one wire

  typedef std::map<DTDetId, std::vector<PSimHit> > DTDetIdMap;
  typedef std::map<DTDetId, std::vector<PSimHit> >::iterator DTDetIdMapIter;  
  typedef std::map<DTDetId, std::vector<PSimHit> >::const_iterator DTDetIdMapConstIter;  

  // Sort hits container by time.
  struct hitLessT {
    bool operator()(const  hitAndT & h1, const hitAndT & h2) {
      if (h1.second < h2.second) return true;
      return false;
    }
  };

  // Calculate the drift time for one hit. 
  // if status flag == false, hit has to be discarded.
  std::pair<float,bool> computeTime(const DTGeomDetUnit* layer,const DTDetId &wireId, const PSimHit *hit) ;

  // Calculate the drift time using the GARFIELD cell parametrization,
  // taking care of all conversions from CMSSW local coordinates
  // to the conventions used for the parametrization.
  std::pair<float,bool> driftTimeFromParametrization(float x, float alpha, float By,
						float Bz) const;
  
  // Calculate the drift time for the cases where it is not possible
  // to use the GARFIELD cell parametrization.
  std::pair<float,bool> driftTimeFromTimeMap() const;
  
  // Add all delays other than drift times (signal propagation along the wire, 
  // TOF etc.; subtract calibration time.
  float externalDelays(const DTTopology &topo, 
		       const DTDetId &wireId, 
		       const PSimHit *hit) const;

  // Store digis for one wire, taking into account the dead time.
  //FiXME put alias for the map.
  void storeDigis(DTDetId &wireId, 
		  TDContainer &hits,
		  DTDigiCollection &output);

  void loadOutput(DTDigiCollection &output,
		  std::vector<DTDigi> &digis, DTDetId &layerID);

  // Debug output
  void dumpHit(const PSimHit * hit, float xEntry, float xExit, const DTTopology &topo);


  // Check if given point (in cell r.f.) is on cell borders.
  enum sides {zMin,zMax,xMin,xMax,yMin,yMax,none}; // sides of the cell
  sides onWhichBorder_old(float x, float y, float z, const DTTopology& topo);
  sides onWhichBorder(float x, float y, float z, const DTTopology& topo);

  // Double half-gaussian smearing.
  float asymGausSmear(double mean, double sigmaLeft, double sigmaRight) const;

  // Additional "synchronization" delays
  DTBaseDigiSync * theSync; // ci sara' ancora??

  // Allow debugging and testing.
  friend class DTDigitizerAnalysis;

  // Its Atributes:
  double vPropWire;
  float deadTime;
  float smearing;
  bool debug;
  bool interpolate;
  bool onlyMuHits;
};
#endif
