#ifndef DTResolutionAnalysisTask_H
#define DTResolutionAnalysisTask_H

/** \class DTResolutionAnalysis
 *  DQM Analysis of 4D DT segments, it produces plots about: <br>
 *      - number of segments per event <br>
 *      - position of the segments in chamber RF <br>
 *      - direction of the segments (theta and phi projections) <br>
 *      - reduced chi-square <br>
 *  All histos are produce per Chamber
 *
 *
 *  $Date: 2008/07/25 14:15:43 $
 *  $Revision: 1.5 $
 *  \author G. Cerminara - INFN Torino
 */

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "DataFormats/MuonDetId/interface/DTSuperLayerId.h"
#include <FWCore/Framework/interface/EDAnalyzer.h>
#include "FWCore/Framework/interface/ESHandle.h"


#include <string>
#include <map>
#include <vector>

class DQMStore;
class MonitorElement;
class DTGeometry;

class DTResolutionAnalysisTask: public edm::EDAnalyzer{
public:
  /// Constructor
  DTResolutionAnalysisTask(const edm::ParameterSet& pset);

  /// Destructor
  virtual ~DTResolutionAnalysisTask();

  /// BeginJob
  void beginJob(const edm::EventSetup& c);

  /// To reset the MEs
  void beginLuminosityBlock(edm::LuminosityBlock const& lumiSeg, edm::EventSetup const& context) ;

  /// Endjob
  void endJob();

  // Operations
  void analyze(const edm::Event& event, const edm::EventSetup& setup);
  
  /// Summary
  void endLuminosityBlock(edm::LuminosityBlock const& lumiSeg, edm::EventSetup const& c);

protected:

private:
  DQMStore* theDbe;

  edm::ESHandle<DTGeometry> dtGeom;
  
  int prescaleFactor;
  int resetCycle;

  std::string folderRoot;
  std::string histoTag;

  // Lable of 4D segments in the event
  std::string theRecHits4DLabel;
  // Lable of 1D rechits in the event
  std::string theRecHitLabel;
  
  // Book a set of histograms for a give chamber
  void bookHistos(DTSuperLayerId slId);
  void bookHistos(const DTChamberId & ch);
  // Fill a set of histograms for a give chamber 
  void fillHistos(DTSuperLayerId slId,
		  float distExtr,
		  float residual);

  // Histograms for tests
  std::map< std::pair<int,int> , MonitorElement* > MeanHistos;
  std::map< std::pair<int,int> , MonitorElement* > SigmaHistos;
  
  std::map<DTSuperLayerId, std::vector<MonitorElement*> > histosPerSL;
};
#endif

