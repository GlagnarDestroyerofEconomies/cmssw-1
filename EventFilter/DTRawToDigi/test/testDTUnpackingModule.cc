/**
   \file
   Test suit for DTUnpackingModule

   \author Stefano ARGIRO
   \version $Id: testDTUnpackingModule.cc,v 1.4 2005/08/23 09:32:00 argiro Exp $
   \date 29 Jun 2005

   \note these tests are not testing anything but the thing not crashing
        
*/

static const char CVSId[] = "$Id: testDTUnpackingModule.cc,v 1.4 2005/08/23 09:32:00 argiro Exp $";

#include <cppunit/extensions/HelperMacros.h>
#include <FWCore/Framework/interface/EventProcessor.h>
#include "FWCore/Framework/interface/ProblemTracker.h"
#include "FWCore/Utilities/interface/Exception.h"
#include <iostream>
#include <cstdlib>

using namespace std;


string releasetop(getenv("SCRAMRT_LOCALRT"));
string testfileLocation= releasetop + "/src/EventFilter/DTRawToDigi/test/";

class testDTUnpackingModule: public CppUnit::TestFixture {

  CPPUNIT_TEST_SUITE(testDTUnpackingModule);

  CPPUNIT_TEST(testUnpacker);
  CPPUNIT_TEST(testPoolIO);

  CPPUNIT_TEST_SUITE_END();

public:


  void setUp(){
    char * ret = getenv("SCRAMRT_LOCALRT");
    if (!ret) {
      cerr<< "env variable SCRAMRT_LOCALRT not set, try eval `scramv1 runt -csh`"<< endl;
      exit(1);
    }
  }

  void tearDown(){}  
  void testUnpacker();
  void writeOut();
  void testPoolIO();
 
}; 

///registration of the test so that the runner can find it
CPPUNIT_TEST_SUITE_REGISTRATION(testDTUnpackingModule);


void testDTUnpackingModule::testUnpacker(){

  const std::string config=
    "process TEST = { \n"
     "module dtunpacker = DTUnpackingModule{ }\n"
     "module hit = DummyHitFinderModule{ }\n"
     "path p = {dtunpacker, hit}\n"
     "source = DAQFileInputService{ string fileName =\""  + testfileLocation+ "dtraw.raw" +"\" }\n"
    "}\n";
 
  edm::AssertHandler ah;
  int rc=0;

  try{
    edm::EventProcessor proc(config);
    proc.run();
  } catch (seal::Error& e){
    std::cerr << "Exception caught: "
	      << e.explainSelf()
	      << std::endl;
    rc=1;
  }
  
  CPPUNIT_ASSERT(rc==0);

}

void testDTUnpackingModule::writeOut(){

 const std::string config=
    "process TEST = { \n"
     "module dtunpacker = DTUnpackingModule{ }\n"
     "module out = PoolOutputModule {\n"
     "                   untracked string fileName =\"" + testfileLocation+ "dtdigis.root" +"\"} \n"
     "path p = {dtunpacker, out}\n" 
     "source = DAQFileInputService{ string fileName =\""  + testfileLocation+ "dtraw.raw" +"\" }\n"
    "}\n";
 edm::AssertHandler ah;
 int rc=0;
 try {
   edm::EventProcessor proc(config);
   proc.run();
 } catch (seal::Error& e){
   std::cerr << "Exception caught:  " 
	     << e.explainSelf()
	      << std::endl;
   rc=1;
 }
 
 CPPUNIT_ASSERT(rc==0);

}

void testDTUnpackingModule::testPoolIO(){

  writeOut();

   const std::string config=
    "process TEST = { \n"
    " module hit = DummyHitFinderModule{ }\n"
    " path p = {hit}\n"
    " source = PoolInputService{ string fileName =\""  + testfileLocation+ "dtdigis.root" +"\"} \n"
    "}\n";
   edm::AssertHandler ah;
   int rc=0;
   try{
     edm::EventProcessor proc(config);
     proc.run();
   } catch (seal::Error& e){
     std::cerr << "Exception caught: "
	       << e.explainSelf()
	       << std::endl;
     rc=1;
   }
   
   CPPUNIT_ASSERT(rc==0);
}
