/**
 * \file DummyUnpackingModule.h 
 * dummy module  for the test of  DaqFileInputService
 *   
 * 
 * $Date: 2005/09/30 08:17:48 $
 * $Revision: 1.1 $
 * \author N. Amapane - S. Argiro'
 *
*/

#include <cppunit/extensions/HelperMacros.h>

#include <FWCore/Framework/interface/MakerMacros.h>
#include <FWCore/Framework/interface/EDAnalyzer.h>
#include <FWCore/Framework/interface/Event.h>
#include <DataFormats/FEDRawData/interface/FEDRawDataCollection.h>
#include <DataFormats/FEDRawData/interface/FEDHeader.h>
#include <DataFormats/FEDRawData/interface/FEDTrailer.h>
#include <DataFormats/FEDRawData/interface/FEDNumbering.h>

#include <iostream>

using namespace edm;
using namespace std;

namespace test{

  class DummyUnpackingModule: public EDAnalyzer{
  
  private:
    unsigned int count_;
  
  public:

    DummyUnpackingModule(const ParameterSet& pset):count_(0){}
 
    void analyze(const Event & e, const EventSetup& c){
      
      ++count_;

      Handle<FEDRawDataCollection> rawdata;
      e.getByLabel("DaqRawData", rawdata);
      for (int i = 0; i<FEDNumbering::lastFEDId(); i++){
	FEDRawData& data = rawdata->FEDData(i);
	if(size_t size=data.size()) {
	  cout << "FED# " << i << " " << size << endl;
	  FEDHeader header(data.data());
	  CPPUNIT_ASSERT(header.check()==true);
	  FEDTrailer trailer(data.data()+size-8);
	  CPPUNIT_ASSERT(trailer.check()==true);
	  CPPUNIT_ASSERT(trailer.lenght()==data.size()/8);
	}
      }
//       if ( count_==1) {
// 	   CPPUNIT_ASSERT( rawdata->FEDData(619).size()==5560);
//         CPPUNIT_ASSERT( rawdata->FEDData(620).size()==5544);     
//       }  
    }
  };
DEFINE_FWK_MODULE(DummyUnpackingModule)
}

