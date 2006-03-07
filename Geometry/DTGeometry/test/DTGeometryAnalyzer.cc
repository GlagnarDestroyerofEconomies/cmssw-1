/** \file
 *
 *  $Date: 2006/02/22 11:06:57 $
 *  $Revision: 1.1 $
 *  \author N. Amapane - CERN
 */

#include <memory>

#include <FWCore/Framework/interface/Frameworkfwd.h>
#include <FWCore/Framework/interface/EDAnalyzer.h>
#include <FWCore/Framework/interface/Event.h>
#include <FWCore/Framework/interface/EventSetup.h>
#include <FWCore/Framework/interface/ESHandle.h>
#include <FWCore/ParameterSet/interface/ParameterSet.h>

#include <Geometry/CommonDetUnit/interface/GeomDetUnit.h>

#include <Geometry/DTGeometry/interface/DTGeometry.h>
#include <Geometry/Records/interface/MuonGeometryRecord.h>
#include <Geometry/Vector/interface/GlobalPoint.h>
//#include <Geometry/DTGeometry/interface/DTLayer.h>

#include <string>
#include <cmath>
#include <iomanip> // for setw() etc.
#include <vector>

using namespace std;

class DTGeometryAnalyzer : public edm::EDAnalyzer {

  public: 
    DTGeometryAnalyzer( const edm::ParameterSet& pset);

    ~DTGeometryAnalyzer();

    virtual void analyze( const edm::Event&, const edm::EventSetup& );

    const string& myName() { return myName_;}

  private: 

    const int dashedLineWidth_;
    const string dashedLine_;
    const string myName_;
};

DTGeometryAnalyzer::DTGeometryAnalyzer( const edm::ParameterSet& iConfig ) : 
  dashedLineWidth_(104), dashedLine_( string(dashedLineWidth_, '-') ), myName_( "DTGeometryAnalyzer" ) {
  }


DTGeometryAnalyzer::~DTGeometryAnalyzer() {
}

void DTGeometryAnalyzer::analyze( const edm::Event& iEvent,
                                  const edm::EventSetup& iSetup ) {
  edm::ESHandle<DTGeometry> pDD;
  iSetup.get<MuonGeometryRecord>().get( pDD );     

  cout << myName() << ": Analyzer..." << endl;
  cout << "start " << dashedLine_ << endl;


  cout << " Geometry node for DTGeom is  " << &(*pDD) << endl;   
  cout << " I have "<<pDD->detUnits().size() << " detUnits" << endl;
  cout << " I have "<<pDD->detTypes().size() << " types" << endl;
  cout << " I have "<<pDD->layers().size() << " layers" << endl;
  cout << " I have "<<pDD->superLayers().size() << " superlayers" << endl;
  cout << " I have "<<pDD->chambers().size() << " chambers" << endl;

  cout << myName() << ": Begin iteration over geometry..." << endl;
  cout << "iter " << dashedLine_ << endl;

  // check dets
  for(DTGeometry::DetUnitContainer::const_iterator det = pDD->detUnits().begin(); 
      det != pDD->detUnits().end(); ++det){

    DetId detId = (*det)->geographicalId();
    int id = detId(); // or detId.rawId()
    const GeomDetUnit* gdet=pDD->idToDetUnit(detId);
    const DTLayer* lay=dynamic_cast<const DTLayer*>(gdet);
    cout << "GeomDetUnit is of type " << detId.det() << " and raw id = " << id
      << " get back GDU from ID " << gdet << " " <<(*det==gdet) << " " << lay << endl;


  }

  cout << "LAYERS " << dashedLine_ << endl;
  // check layers
  for(vector<DTLayer*>::const_iterator det = pDD->layers().begin(); 
      det != pDD->layers().end(); ++det){
    const DTTopology& topo = (*det)->specificTopology();
    const BoundPlane& surf=(*det)->surface();
    cout << "Layer " << (*det)->id() << " SL " << (*det)->superLayer()->id() 
      << " chamber " << (*det)->chamber()->id() 
      << " Topology W/H/L: " 
      << topo.cellWidth() << "/" << topo.cellHeight() << "/" << topo.cellLenght() 
      << " Position " << surf.position()
      << " normVect " << surf.normalVector() 
      << " bounds W/H/L: " << surf.bounds().width() << "/" 
      << surf.bounds().thickness() << "/" << surf.bounds().length()
      << endl;
  }

  // check superlayers
  cout << "SUPERLAYERS " << dashedLine_ << endl;
  for(vector<DTSuperLayer*>::const_iterator det = pDD->superLayers().begin(); 
      det != pDD->superLayers().end(); ++det){
    const BoundPlane& surf=(*det)->surface();
    cout << "SuperLayer " << (*det)->id()
      << " chamber " << (*det)->chamber()->id()
      << " Position " << surf.position()
      << " normVect " << surf.normalVector() 
      << " bounds W/H/L: " << surf.bounds().width() << "/" 
      << surf.bounds().thickness() << "/" << surf.bounds().length()
      << endl;
  }

  // check chamber
  cout << "CHAMBERS " << dashedLine_ << endl;
  for(vector<DTChamber*>::const_iterator det = pDD->chambers().begin(); 
      det != pDD->chambers().end(); ++det){
    //cout << "Chamber " << (*det)->geographicalId().det() << endl;
    const BoundPlane& surf=(*det)->surface();
    //cout << "surf " << &surf <<  endl;
    cout << "Chamber " << (*det)->id() 
      << " Position " << surf.position()
      << " normVect " << surf.normalVector() 
      << " bounds W/H/L: " << surf.bounds().width() << "/" 
      << surf.bounds().thickness() << "/" << surf.bounds().length()
      << endl;
  }
  cout << "END " << dashedLine_ << endl;
  for (int w=-2; w<=2; ++w) {
    for (int st=1; st<=4; ++st) {
      for (int se=1; se <= ((st==4) ? 14 : 12) ; ++se) {
        DTChamberId id(w,st,se);
        const DTChamber* ch = pDD->chamber(id);
        if (!ch) cout << "ERROR ch not found " << id << endl;
        if (id!=ch->id()) cout << "ERROR: got wrong chamber: Cerco camera " << id << " e trovo " << ch->id() << endl;
        for (int sl=1; sl<= 3 ; ++sl) {
          if (sl==2 && st==4) continue;
          DTSuperLayerId slid(id,sl);
          const DTSuperLayer* sl = pDD->superLayer(slid);
          if (!sl) cout << "ERROR sl not found " << slid << endl;
          if (slid!=sl->id()) cout << "ERROR: got wrong sl! Cerco sl " << slid << " e trovo " << sl->id() << endl;
          for (int l=1; l<=4; ++l) {
            DTLayerId lid(slid,l);
            const DTLayer* lay = pDD->layer(lid);
            if (!lay) cout << "ERROR lay not found " << lid << endl;
            if (lid!=lay->id()) cout << "ERROR: got wrong layer Cerco lay  " << lid << " e trovo " << lay->id() << endl;
          }
        }
      }
    }
  }
  cout << "END " << dashedLine_ << endl;

  for (int w=-2; w<=2; ++w) {
    for (int st=1; st<=4; ++st) {
      for (int se=1; se <= ((st==4) ? 14 : 12) ; ++se) {
        DTChamberId id(w,st,se);
        const DTChamber* ch = pDD->chamber(id);
        if (!ch) cout << "ERROR ch not found " << id << endl;
        if (id!=ch->id()) cout << "ERROR: got wrong chamber: Cerco camera " << id << " e trovo " << ch->id() << endl;
        for (int sl=1; sl<= 3 ; ++sl) {
          if (sl==2 && st==4) continue;
          DTSuperLayerId slid(id,sl);
          const DTSuperLayer* sl = pDD->superLayer(slid);
          if (!sl) cout << "ERROR sl not found " << slid << endl;
          if (slid!=sl->id()) cout << "ERROR: got wrong sl! Cerco sl " << slid << " e trovo " << sl->id() << endl;
          for (int l=1; l<=4; ++l) {
            DTLayerId lid(slid,l);
            const DTLayer* lay = pDD->layer(lid);
            if (!lay) cout << "ERROR lay not found " << lid << endl;
            if (lid!=lay->id()) cout << "ERROR: got wrong layer Cerco lay  " << lid << " e trovo " << lay->id() << endl;
          }
        }
      }
    }
  }
  cout << "END " << dashedLine_ << endl;

}

//define this as a plug-in
#include <FWCore/Framework/interface/MakerMacros.h>
DEFINE_FWK_MODULE(DTGeometryAnalyzer)
