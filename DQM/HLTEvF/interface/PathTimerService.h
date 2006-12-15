#ifndef Services_PATHTIMERSERVICE_h
#define Services_PATHTIMERSERVICE_h
// -*- C++ -*-
//
// Package:     Services
// Class  :     Timing
// 
//
// Original Author:  David Lange
// $Id: Timing.h,v 1.3 2006/08/16 13:36:28 chrjones Exp $
//
#include "sigc++/signal.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/ActivityRegistry.h"
#include "DataFormats/Common/interface/EventID.h"
//#include "DataFormats/HLTReco/interface/PathTiming.h"
#include "DataFormats/HLTReco/interface/HLTPerformanceInfo.h"

namespace edm {
  namespace service {
    class PathTimerService
    {
    public:
      PathTimerService(const ParameterSet&,ActivityRegistry&);
      ~PathTimerService();

      sigc::signal<void, const edm::ModuleDescription&, double> newMeasurementSignal;

      std::auto_ptr<HLTPerformanceInfo> getInfo() { return std::auto_ptr<HLTPerformanceInfo>(new HLTPerformanceInfo(*_perfInfo));}

    private:
      void postBeginJob();
      void postEndJob();
      
      void preEventProcessing(const edm::EventID&, const edm::Timestamp&);
      void postEventProcessing(const Event&, const EventSetup&);
      void postPathProcessing(const std::string&, const HLTPathStatus&);
      void preModule(const ModuleDescription&);
      void postModule(const ModuleDescription&);
      //      void prePath(const PathDescription&);
      //void postPath(const PathDescription&);

      edm::EventID curr_event_;
      double curr_job_; // seconds
      double curr_event_time_;  // seconds
      double curr_module_time_; // seconds
      
        //
       // Min Max and average event times for summary
      //  at end of job
      double max_event_time_;    // seconds
      double min_event_time_;    // seconds
      int total_event_count_; 

      ParameterSet _myPS;
      std::map<std::string, std::string> _moduleList;
      std::map<std::string, double> _moduleTime;
      std::map<int, std::string> _pathMapping;
      std::map<std::string, unsigned int> _lastModuleToRun;
      std::auto_ptr<HLTPerformanceInfo> _perfInfo;

    };
  }
}



#endif
