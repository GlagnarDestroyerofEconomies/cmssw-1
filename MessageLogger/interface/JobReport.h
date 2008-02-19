#ifndef FWCore_MessageLogger_JobReport_h
#define FWCore_MessageLogger_JobReport_h
// -*- C++ -*-
//
// Package:     MessageLogger
// Class  :     JobReport
// 
/**\class JobReport JobReport.h FWCore/MessageLogger/interface/JobReport.h

Description: A service that collections job handling information.

Usage:
The JobReport service collects 'job handling' information (currently
file handling) from several sources, collates the information, and
at appropriate intervales, reports the information to the job report,
through the MessageLogger.

*/

//
// Original Author:  Marc Paterno
// $Id$
//

#include <cstddef>
#include <string>
#include <ostream>
#include <set>
#include <map>
#include <vector>


#include "boost/scoped_ptr.hpp"

namespace edm {

    class JobReport {
    public:
      typedef unsigned int RunNumber;
      typedef std::size_t Token;

      /**\struct InputFile
  
      Description: Holds information about an InputFile.
  
      Usage: The struct InputFile is a collection of the information that
      Data Handling wishes to accumulate about the use of a file that has
      been opened for input.
      */
  
      struct InputFile {
        typedef std::set<RunNumber> RunNumberCollection;
        typedef std::vector<std::string>   StringVector;
  
        std::string     logicalFileName;
        std::string     physicalFileName;
        std::string     catalog;
        std::string     inputSourceClassName; // class which created the file
        std::string     moduleLabel;   // name of class instance
        std::string     guid;
        RunNumberCollection runsSeen;
        size_t          numEventsRead;
        StringVector    branchNames;
        bool            fileHasBeenClosed;
      };

      /**\struct LumiSectionReport
  
      Description: Holds information about a Lumi section associated to a
      file
  
      Usage: struct contains parameters describing a Lumi Section, OutputFile
             object stores a vector of these for each file
      */

      struct LumiSectionReport {
	unsigned int runNumber;
	unsigned int lumiSectionId;
	/// So far we are proceeding without extra information, but
	/// this may be added in the future...
	///unsigned int startEvent;
	///unsigned int lastEvent;
	///std::string lumiStartTime;
	///std::string lumiEndTime;
	
      };

  
      /**\struct OutputFile 
  
      Description: Holds information about an OutputFile.
  
      Usage: The struct OutputFile is a collection of the information that
      Data Handling wishes to accumulate about the use of a file that has
      been opened for output.
      */


      struct OutputFile {
        typedef InputFile::RunNumberCollection RunNumberCollection;
        typedef InputFile::StringVector        StringVector;
  
        std::string     logicalFileName;
        std::string     physicalFileName;
        std::string     catalog;
        std::string     outputModuleClassName;
        std::string     moduleLabel;   // name of class instance
        std::string     guid;
        std::string     dataType; 
	std::string     branchHash;
        RunNumberCollection runsSeen;
        size_t          numEventsWritten;
        StringVector    branchNames;
        std::vector<Token> contributingInputs;
	std::vector<LumiSectionReport> lumiSections;
        bool            fileHasBeenClosed;
      };
  
      struct JobReportImpl {
        // Helper functions to be called from the xFileClosed functions,
        // after the appropriate logging is done.
        /*
         * Note: We want to keep input files, so we can resolve tokens
         * when we close output files
         *
         * Note2: Erasing the Output files on close invalidates the Tokens
         * for existing output files since they are based on position in 
         * the vector, so I have left off implementing these methods for now
         */
        void removeInputFileRecord_(Token t); 
        void removeOutputFileRecord_(Token t);
  
        InputFile& getInputFileForToken(Token t);
        OutputFile& getOutputFileForToken(Token t);
        /*
         * Add the input file token provided to every output 
         * file currently available.
         * Used whenever a new input file is opened, it's token
         * is added to all open output files as a contributor
         */
        void insertInputForOutputs(Token t);
  
        /*
         * get a vector of Tokens for all currently open
         * input files. 
         * Used when a new output file is opened, all currently open
         * input file tokens are used to initialise its list of contributors
         */
        std::vector<Token> openInputFiles(void);

	/*
	 * Get a vector of Tokens for all currently open output files
	 * Used to add lumi sections to open files
	 */
        std::vector<Token> openOutputFiles(void);
  
	/*
	 * Associate a Lumi Section to all open output files
	 *
	 */
	void associateLumiSection(const LumiSectionReport&  rep);

        /*
         * Write an InputFile object to the Logger 
         * Generate XML string for InputFile instance and dispatch to 
         * job report via MessageLogger
         */
        void writeInputFile(const InputFile & f);
        /*
         * Write an OutputFile object to the Logger 
         * Generate an XML string for the OutputFile provided and
         * dispatch it to the logger
         * Contributing input tokens are resolved to the input LFN and PFN
         * 
         * TODO: We have not yet addressed the issue where we cleanup not
         * contributing input files. 
         * Also, it is possible to get fake input to output file mappings
         * if an input file is open already when a new output file is opened
         * but the input gets closed without contributing events to the
         * output file due to filtering etc.
         *
         */
        void writeOutputFile(const OutputFile & f);
        

	/*
	 * Add Generator info to the map of gen info stored in this 
	 * instance.
	 */
	void addGeneratorInfo(const std::string& name, const std::string& value);
	
	/*
	 * Write out generator info to the job report
	 */
	void writeGeneratorInfo(void);
	

        /*
         *  Flush all open files to logger in event of a problem.
         */
        void flushFiles(void);
        
        JobReportImpl(std::ostream* iOst): ost_(iOst) {}
	
        std::vector<InputFile> inputFiles_;
        std::vector<OutputFile> outputFiles_;
	std::map<std::string, std::string> generatorInfo_;
        std::ostream* ost_;
      };

      JobReport();
      //Does not take ownership of pointer
      JobReport(std::ostream* outputStream);
      
      ~JobReport();
         
      /// Report that an input file has been opened.
      /// The returned Token should be used for later identification
      /// of this file.
      Token inputFileOpened(std::string const& physicalFileName,
			    std::string const& logicalFileName,
			    std::string const& catalog,
			    std::string const& inputSourceClassName,
			    std::string const& moduleLabel,
                            std::string const& guid,
			    std::vector<std::string> const& branchNames);
      
     Token inputFileOpened(std::string const& physicalFileName,
			    std::string const& logicalFileName,
			    std::string const& catalog,
			    std::string const& inputSourceClassName,
			    std::string const& moduleLabel,
			    std::vector<std::string> const& branchNames);
      /// Report that the event with the given id has been read from
      /// the file identified by the given Token.
      void eventReadFromFile(Token fileToken, unsigned int run, unsigned int event);

      ///
      /// Report the data type of a file after it has been opened
      /// Needed since the data type isnt known until an event has been
      /// read
      void reportDataType(Token fileToken, std::string const& dataType);


      /// Report that the input file identified by the given Token has
      /// been closed. An exception will be thrown if the given Token
      /// was not obtained from inputFileOpened.
      void inputFileClosed(Token fileToken);

      /// Report that an output file has been opened.
      /// The returned Token should be used for later identification
      /// of this file.
      Token outputFileOpened(std::string const& physicalFileName,
			     std::string const& logicalFileName,
			     std::string const& catalog,
			     std::string const& outputModuleClassName,
			     std::string const& moduleLabel,
			     std::string const& guid,
			     std::string const& dataType,
			     std::string const& branchHash,
			     std::vector<std::string> const& branchNames);

      Token outputFileOpened(std::string const& physicalFileName,
			     std::string const& logicalFileName,
			     std::string const& catalog,
			     std::string const& outputModuleClassName,
			     std::string const& moduleLabel,
			     std::string const& guid,
			     std::string const& dataType,
			     std::vector<std::string> const& branchNames);
      
      /*      Token outputFileOpened(std::string const& physicalFileName,
			     std::string const& logicalFileName,
			     std::string const& catalog,
			     std::string const& outputModuleClassName,
			     std::string const& moduleLabel,
			     std::string const& dataType,
			     std::vector<std::string> const& branchNames);
      */
      
      Token outputFileOpened(std::string const& physicalFileName,
			     std::string const& logicalFileName,
			     std::string const& catalog,
			     std::string const& outputModuleClassName,
			     std::string const& moduleLabel,
			     std::string const& guid,
			     std::vector<std::string> const& branchNames);
      
      
      Token outputFileOpened(std::string const& physicalFileName,
			     std::string const& logicalFileName,
			     std::string const& catalog,
			     std::string const& outputModuleClassName,
			     std::string const& moduleLabel,
			     std::vector<std::string> const& branchNames);
			     
      

      /// Report that the event with the given id has been written to
      /// the file identified by the given Token.
      void eventWrittenToFile(Token fileToken, unsigned int run, unsigned int event);
      
      /// Report that the output file identified by the given Token has
      /// been closed. An exception will be thrown if the given Token
      /// was not obtained from outputFileOpened.
      void outputFileClosed(Token fileToken);

      ///
      /// For use by fast merge: Since the event by event counter cant
      /// be used for fast merges, use this method to forcibly set the
      /// events written count for an output file before reporting it 
      /// closed
      void overrideEventsWritten(Token fileToken, const int eventsWritten);
      ///
      /// For use by fast merge: Since the event by event counter cant
      /// be used for fast merges, use this method to forcibly set the
      /// events read count for an input file before reporting it 
      /// closed
      void overrideEventsRead(Token fileToken, const int eventsRead);

      void reportSkippedEvent(unsigned int run, unsigned int event);


      ///
      /// API for reporting a Lumi Section to the job report.
      /// 
      ///
      ///
      void reportLumiSection(unsigned int run, unsigned int lumiSectId);


      ///
      /// Report an exception, providing details of the problem as
      /// a short description (Eg "XXXError") and a long description 
      /// (Eg "XXX crashed because...")
      /// Also overload this method to accept an optional standard exit code
      void  reportError(std::string const& shortDesc,
			std::string const& longDesc);
      
      void reportError(std::string const& shortDesc,
		       std::string const& longDesc,
		       int const& exitCode);

      ///
      /// Report Skipped File
      ///
      /// Report that a file has been skipped due to it not being
      /// found.
      void reportSkippedFile(std::string const& pfn, std::string const& lfn);
	
      void reportAnalysisFile(std::string const& fileName, 
			      std::map<std::string, std::string> const& fileData) ;
      ///
      /// Report Timing statistics
      /// Invoked by the Timing service to send an end of job 
      /// summary about time taken for inclusion in the job report
      ///
      void reportTimingInfo(std::map<std::string, double> const & timingData);

      /// Report Storage Statistics
      void reportStorageStats(std::string const & data); 

      /// Override the list of input files seen by an output file
      /// for use with EdmFastMerge 
      void overrideContributingInputs(Token outputToken, std::vector<Token> const& inputTokens);
      
      /// Report key/value style generator/lumi information
      /// Eg:  reportGeneratorInfo("CrossSection" , "ValueHere")
      /// No special chars in the value string. 
      void reportGeneratorInfo(std::string const&  name, std::string const&  value);
      
      ///
      /// Report the name of the random engine persistency file
      ///
      void reportRandomStateFile(std::string const& name);

      ///
      /// Report PSetHash
      ///
      ///
      void reportPSetHash(std::string const& hashValue);
      

      ///
      /// Performance Reports
      ///
      /// Two categories:  Summary for entire job and module
      /// for a specific module
      /// Each one requires a performance metric class such 
      /// as Timing, Memory, CPU, Trigger etc.
      void reportPerformanceSummary(std::string const&  metricClass,
				    std::map<std::string, std::string> const& metrics);
      
      void reportPerformanceForModule(std::string const&  metricClass,
				      std::string const&  moduleName,
				      std::map<std::string, std::string> const& metrics);

      

      /// debug/test util
      std::string dumpFiles(void);

   protected:
      boost::scoped_ptr<JobReportImpl>& impl() {return impl_;}

   private:
      boost::scoped_ptr<JobReportImpl> impl_;

   };

   std::ostream& operator<< (std::ostream& os, JobReport::InputFile const& f);
   std::ostream& operator<< (std::ostream& os, JobReport::OutputFile const& f);
   std::ostream& operator<< (std::ostream& os, JobReport::LumiSectionReport const& rep);

    /*
     * Note that output formatting is spattered across these classes
     * If something outside these classes requires access to the 
     * same formatting then we need to refactor it into a common library
     */
  template <typename S, typename T>
  S& formatFile(const T& f, S& os) {

    if (f.fileHasBeenClosed) {
      os << "\n<State  Value=\"closed\"/>";
    } else {
      os << "\n<State  Value=\"open\"/>";
    }
    os << "\n<LFN>" << f.logicalFileName << "</LFN>";
    os << "\n<PFN>" << f.physicalFileName << "</PFN>";
    os << "\n<Catalog>" << f.catalog << "</Catalog>";
    os << "\n<ModuleLabel>" << f.moduleLabel << "</ModuleLabel>";
    os << "\n<GUID>" << f.guid << "</GUID>";
    os << "\n<Runs>";
    std::set<JobReport::RunNumber>::const_iterator iRun;
    for ( iRun = f.runsSeen.begin(); iRun != f.runsSeen.end(); iRun++) {
      os << "\n  <Run>" << *iRun << "</Run>";
    }
    os << "\n</Runs>";
    os << "\n<Branches>";
    std::vector<std::string>::const_iterator iBranch;
    for (iBranch = f.branchNames.begin(); 
        iBranch != f.branchNames.end(); 
        iBranch++) {
      os << "\n  <Branch>" << *iBranch << "</Branch>";
    }
    os << "\n</Branches>";
    return os;
  }
}

#endif
