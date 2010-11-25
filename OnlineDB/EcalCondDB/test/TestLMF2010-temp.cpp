#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <time.h>
#include <cstdlib>
#include <limits.h>
#include "OnlineDB/EcalCondDB/interface/EcalCondDBInterface.h"
#include "OnlineDB/EcalCondDB/interface/all_lmf_types.h"

using namespace std;

class CondDBApp {
public:

  /**
   *   App constructor; Makes the database connection
   */
  CondDBApp(string sid, string user, string pass, run_t r)
  {
    try {
      cout << "Making connection..." << flush;
      econn = new EcalCondDBInterface( sid, user, pass );
      run = r;
      cout << "Done." << endl;
    } catch (runtime_error &e) {
      cerr << e.what() << endl;
      exit(-1);
    }
  }

  /**
   *  App destructor;  Cleans up database connection
   */
  ~CondDBApp() 
  {
    delete econn;
  }

  void doRun() {
    vector<EcalLogicID> ecid_vec;
    int sm_min=1;
    int sm_max=36;
    int ch_min=0;
    int ch_max=1;

    // get the list of all logic ids
    ecid_vec = econn->getEcalLogicIDSetOrdered("EB_LM_side", sm_min, sm_max,
                                               ch_min, ch_max,
                                               EcalLogicID::NULLID, 
					       EcalLogicID::NULLID,
                                               "EB_crystal_number", 1234 );
    cout << "Got " << ecid_vec.size() << " logic id's" << endl;
    // create a LMFIOV
    LMFIOV iov(econn);
    iov.debug();
    Tm start;
    Tm stop;
    start.setToString("2010-11-10 10:56:12");
    stop.setToString("2010-11-11 12:16:17");
    iov.setStart(start);
    iov.setStop(stop);
    econn->insertLmfIOV(&iov);
    // create the corresponding LMFLmrSubIOV
    LMFLmrSubIOV subiov1(econn);
    LMFLmrSubIOV subiov2(econn);
    subiov1.setLMFIOV(iov);
    subiov2.setLMFIOV(iov);
    Tm t1;
    Tm t2;
    Tm t3;
    // the following times are just random times. This is just for testing
    // purposes
    t1 = start;
    t2.setToString("2010-11-10 23:23:58");
    t3 = stop;
    subiov1.setTimes(t1, t2, t3);
    t1 = t2;
    t2 = t3;
    t3.setToString("2010-11-20 12:56:32");
    subiov2.setTimes(t1, t2, t3);
    econn->insertLmfLmrSubIOV(&subiov1);
    econn->insertLmfLmrSubIOV(&subiov2);
    subiov1.dump();
    subiov2.dump();
    cout << "Filling with data" << endl;
    LMFCorrCoefDat coeff(econn);
    coeff.debug();
    // create random data
    vector<EcalLogicID>::const_iterator i = ecid_vec.begin();
    vector<EcalLogicID>::const_iterator e = ecid_vec.end();
    int count = 0;
    while (i != e) {
      if (rand() > RAND_MAX*0.5) {
	coeff.setP123(subiov1, *i, 1., 2., 3., 0.1, 0.2, 0.3);
	coeff.setFlag(subiov1, *i, 0);
      } else {
	coeff.setP123(subiov2, *i, 1., 2., 3., 0.1, 0.2, 0.3);
	coeff.setFlag(subiov2, *i, 0);
      }
      i++;
      count++; 
    }
    std::cout << "Data stored for " << count << " xtals" << std::endl;
    coeff.dump();
    coeff.writeDB();
  }

private:
  CondDBApp();  // hidden default constructor
  EcalCondDBInterface* econn;
  run_t run;
};

int main (int argc, char* argv[])
{
  string sid;
  string user;
  string pass;

  if (argc != 5) {
    cout << "Usage:" << endl;
    cout << "  " << argv[0] << " <SID> <user> <pass> <run>" << endl;
    exit(-1);
  }

  sid = argv[1];
  user = argv[2];
  pass = argv[3];
  int run = atoi(argv[4]);

  try {
    CondDBApp app(sid, user, pass, run);
    app.doRun();
  } catch (exception &e) {
    cout << "ERROR:  " << e.what() << endl;
  } catch (...) {
    cout << "Unknown error caught" << endl;
  }

  cout << "All Done." << endl;

  return 0;
}
