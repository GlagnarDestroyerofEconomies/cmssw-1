/*
  Oracle distributor class
  Copyright John Jones 2007
*/

// Prototype class definition
#include "OracleDistributor.hh"

#include <string>

// Various generic and type-specific exception implementations
#include "ICException.hh"
#include "HardwareAccessException.hh"
#include "MemoryAllocationException.hh"
//#include "ArgumentOutOfRangeException.hh"

// HCAL HLX namespace
namespace HCAL_HLX
{

  // Define namespaces in use
  using namespace ICCoreUtils;
  using namespace std;
  using namespace oracle::occi;

  // Location, username and password for DB access (hard coded)
  // cms_lumi_reader/everyoneneedsLUMI
  const string OracleDistributor::mOracleDBUserName = "cms_lumi_writer"; //CMS_LUMI_HF_WRITER";
  const string OracleDistributor::mOracleDBPassword = "July4th07_W"; //CMS_LUM_0RITER";
  const string OracleDistributor::mOracleDBLocation = "omds"; //cmscalir";
  const string OracleDistributor::mOracleSchema = "CMS_LUMI_OWNER"; //schema_owner

  u32 OracleDistributor::GetErrorCount() {
    return mErrorCount;
  }

  // Default constructor
  OracleDistributor::OracleDistributor() {
    try{
      mErrorCount=0;
      mOracleEnvironment = Environment::createEnvironment(Environment::DEFAULT);
      mOracleConnection = mOracleEnvironment->createConnection(mOracleDBUserName,
							       mOracleDBPassword,
							       mOracleDBLocation);
      //cout << hex << mOracleConnection << endl;

    } catch (SQLException & aExc) {
      HardwareAccessException lExc(aExc.getMessage());
      RAISE(lExc);
    }

  }

  // Destructor deletes the hardware interface
  OracleDistributor::~OracleDistributor() {
    // Terminate the connection
    if ( mOracleConnection ) {
      mOracleEnvironment->terminateConnection(mOracleConnection);
      mOracleConnection = 0;
    }
    if ( mOracleEnvironment ) {
      Environment::terminateEnvironment(mOracleEnvironment);
      mOracleEnvironment = 0;
    }
  }

  void OracleDistributor::ProcessSection(const LUMI_SECTION & lumiSection) {
    try {
      // SQL statement for luminosity header information
      std::string sqlStmt =
	"insert into " + mOracleSchema + ".lumi_sections(SECTION_ID,"
	"SET_VERSION_NUMBER, IS_DATA_TAKING, BEGIN_ORBIT_NUMBER,"
	"END_ORBIT_NUMBER, RUN_NUMBER, LUMI_SECTION_NUMBER,"
	"FILL_NUMBER, SEC_START_TIME, SEC_STOP_TIME, COMMENTS)"
	"VALUES (:sid, :setVnum, :dataTk, :bO, :eO,:run, :lsNum,"
	":fNum,:secStratT,:secStopT,:comm )";

      // Create the SQL statement
      cout << "Create SQL statement" << endl; 
      Statement *mOracleStatement =  mOracleConnection->createStatement(sqlStmt);
      //cout << mOracleStatement << endl;

      // Load the data into the payload
      cout << "Loading data" << endl;
      static int sectionId = 0;
      sectionId++;
      cout << "section id: " << dec << sectionId << endl;
      mOracleStatement->setInt(1,sectionId); // section id?
      mOracleStatement->setInt(2,0); // version number?
      // Is CMS taking data?
      mOracleStatement->setInt(3, static_cast<unsigned int>(lumiSection.hdr.bCMSLive));
      // Begin orbit number
      mOracleStatement->setInt(4, static_cast<unsigned int>(lumiSection.hdr.startOrbit));
      // End orbit number
      mOracleStatement->setInt(5, static_cast<unsigned int>(lumiSection.hdr.startOrbit+lumiSection.hdr.numOrbits-1));
      // Run number
      mOracleStatement->setInt(6, static_cast<unsigned int>(lumiSection.hdr.runNumber));      
      // Lumi section number
      mOracleStatement->setInt(7,sectionId);
      mOracleStatement->setInt(8,0); // Fill number???
      mOracleStatement->setInt(9,0); // Fill number???      
      mOracleStatement->setInt(10,0); // Fill number???
      const string tempString = "debug test of OracleDistributor";
      mOracleStatement->setString(11,tempString);

      // Execute the SQL statement
      cout << "Executing SQL request" << endl;
      mOracleStatement->executeUpdate();

      // Delete the SQL statement
      cout << "Terminate statement" << endl;
      mOracleConnection->terminateStatement(mOracleStatement);

    } catch (SQLException & aExc) {
      cout << "SQL exception caught:" << endl;
      cout << aExc.getMessage() << endl;
      mErrorCount++;
      //HardwareAccessException lExc(aExc.getMessage());
      //RAISE(lExc);
    } catch (...) {
      cout << "Unknown exception caught" << endl;
      mErrorCount++;
    }
  }


} // namespace HCAL_HLX
