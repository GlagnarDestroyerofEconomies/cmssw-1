
/*----------------------------------------------------------------------
$Id: Worker.cc,v 1.12 2006/06/20 23:13:27 paterno Exp $
----------------------------------------------------------------------*/

#include <iostream>
#include "FWCore/Framework/src/Worker.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/Utilities/interface/EDMException.h"
#include "FWCore/Framework/interface/Actions.h"
#include "FWCore/Framework/interface/EventPrincipal.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "boost/signal.hpp"

namespace edm
{
  namespace
  {
    class CallPrePost
    {
    public:
      CallPrePost(Worker::Sigs& s, ModuleDescription& md):s_(&s),md_(&md)
      { (*(s_->preModuleSignal))(*md_); }
      ~CallPrePost()
      { (*(s_->postModuleSignal))(*md_); }
    private:
      Worker::Sigs* s_;
      ModuleDescription* md_;
    };
    
    cms::Exception& exceptionContext(const ModuleDescription& iMD, const EventPrincipal& iEp, cms::Exception& iEx) {
      iEx << iMD.moduleName_ << "/" << iMD.moduleLabel_ 
      << " " << iEp.id() << "\n";
      return iEx;
    }
  }
  
  static ActivityRegistry::PreModule defaultPreModuleSignal;
  static ActivityRegistry::PostModule defaultPostModuleSignal;
  
  Worker::Sigs::Sigs() : preModuleSignal( &defaultPreModuleSignal ),
  postModuleSignal( &defaultPostModuleSignal ) {}
  Worker::Worker(const ModuleDescription& iMD, 
		 const WorkerParams& iWP):
    stopwatch_(new RunStopwatch::StopwatchPointer::element_type),
    timesRun_(),
    timesVisited_(),
    timesPassed_(),
    timesFailed_(),
    timesExcept_(),
    state_(Ready),
    md_(iMD),
    actions_(iWP.actions_),
    cached_exception_()
  {
  }

  Worker::~Worker()
  {
  }

  void Worker::connect(ActivityRegistry::PreModule& pre,
		       ActivityRegistry::PostModule& post)
  {
    sigs_.preModuleSignal= &pre;
    sigs_.postModuleSignal= &post;
  }

  bool Worker::doWork(EventPrincipal& ep, EventSetup const& es,
		      CurrentProcessingContext const* cpc)
  {
    using namespace std;
    RunStopwatch stopwatch(stopwatch_);

    bool rc = false;
    ++timesVisited_;

    switch(state_)
      {
      case Ready: break;
      case Pass: return true;
      case Fail: return false;
      case Exception:
	{
	  // rethrow the cached exception again
	  // only cms::Exceptions can be cached and contributing to
	  // actions or processing routing.  It seems impossible to
	  // get here a second time until a cms::Exception has been 
	  // thrown prviously.
	  LogWarning("repeat") << "A module has been invoked a second "
			       << "time even though it caught an "
			       << "exception during the previous "
			       << "invocation.\n"
			       << "This may be an indication of a "
			       << "configuration problem.\n";

	  throw *cached_exception_;
	}
      }

    ++timesRun_;

    try
      {

	CallPrePost cpp(sigs_,md_);

	rc = implDoWork(ep,es,cpc);

	if(rc)
	  {
	    ++timesPassed_;
	    state_ = Pass;
	  }
	else
	  {
	    ++timesFailed_;
	    state_ = Fail;
	  }
      }

    catch(cms::Exception& e)
      {
      
	// NOTE: the warning printed as a result of ignoring or failing
	// a module will only be printed during the full true processing
	// pass of this module

	switch(actions_->find(e.rootCause()))
	  {
	  case actions::IgnoreCompletely:
	    {
	      rc=true;
	      ++timesPassed_;
	      state_=Pass;
	      LogWarning("IgnoreCompletely")
		<< "Module ignored an exception\n"
                <<e.what()<<"\n";
	      break;
	    }

	  case actions::FailModule:
	    {
	      LogWarning("FailModule")
              << "Module failed an event due to exception\n"
              <<e.what()<<"\n";
	      ++timesFailed_;
	      state_=Fail;
	      break;
	    }
	    
	  default:
	    {

	      // we should not need to include the event/run/module names
	      // the exception because the error logger will pick this
	      // up automatically.  I'm leaving it in until this is 
	      // verified

	      // here we simply add a small amount of data to the
	      // exception to add some context, we could have rethrown
	      // it as something else and embedded with this exception
	      // as an argument to the constructor.

	      ++timesExcept_;
	      state_ = Exception;
	      e << "cms::Exception going through module ";
              exceptionContext(md_,ep,e);
	      cached_exception_.reset(new cms::Exception(e));
	      throw;
	    }
	  }
      }
    
    catch(seal::Error& e)
      {
	++timesExcept_;
	state_ = Exception;
	cached_exception_.reset(new cms::Exception("SealError"));
	*cached_exception_
	  << "A seal::Error occurred during a call to the module ";
        exceptionContext(md_,ep,*cached_exception_)<< "and cannot be repropagated.\n"
	  << "Previous information:\n" << e.explainSelf();
	throw *cached_exception_;
      }
    catch(std::exception& e)
      {
	++timesExcept_;
	state_ = Exception;
	cached_exception_.reset(new cms::Exception("StdException"));
	*cached_exception_
	  << "An std::exception occurred during a call to the module ";
        exceptionContext(md_,ep,*cached_exception_)<< "module and cannot be repropagated.\n"
	  << "Previous information:\n" << e.what();
	throw *cached_exception_;
      }
    catch(std::string& s)
      {
	++timesExcept_;
	state_ = Exception;
	cached_exception_.reset(new cms::Exception("BadExceptionType","std::string"));
	*cached_exception_
	  << "An std::string thrown as an exception occurred during a call to the module ";
        exceptionContext(md_,ep,*cached_exception_)<< "and cannot be repropagated.\n"
	  << "Previous information:\n string = " << s;
	throw *cached_exception_;
      }
    catch(const char* c)
      {
	++timesExcept_;
	state_ = Exception;
	cached_exception_.reset(new cms::Exception("BadExceptionType","const char*"));
	*cached_exception_
	  << "A const char* thrown as an exception occurred during a call to the module ";
        exceptionContext(md_,ep,*cached_exception_)<< "and cannot be repropagated.\n"
	  << "Previous information:\n const char* = " << c<<"\n";
	throw *cached_exception_;
      }
    catch(...)
      {
	++timesExcept_;
	state_ = Exception;
	cached_exception_.reset(new cms::Exception("repeated"));
	*cached_exception_
	  << "An unknown occurred during a previous call to the module ";
        exceptionContext(md_,ep,*cached_exception_)<< " and cannot be repropagated.\n";
        throw;
      }

    return rc;
  }

  void Worker::beginJob(EventSetup const& es)
  {
    using namespace std;
    
    try
      {
	implBeginJob(es);
      }
    catch(cms::Exception& e)
      {
	// should event id be included?
	LogError("BeginJob")
	  << "A cms::Exception is going through "<< workerType()<<":\n";

	e << "A cms::Exception is going through "<< workerType()<<":\n"
	  << description();
	throw;
      }
    catch(seal::Error& e)
      {
	LogError("BeginJob")
	  << "A seal::Error is going through "<< workerType()<<":\n"
	  << description() << "\n";
	throw;
      }
    catch(std::exception& e)
      {
	LogError("BeginJob")
	  << "An std::exception is going through "<< workerType()<<":\n"
	  << description() << "\n";
	throw;
      }
    catch(std::string& s)
      {
	LogError("BeginJob") 
	  << "module caught an std::string during beginJob\n";

	throw cms::Exception("BadExceptionType","std::string") 
	  << "string = " << s << "\n"
	  << description() << "\n" ;
      }
    catch(const char* c)
      {
	LogError("BeginJob") 
	  << "module caught an const char* during beginJob\n";

	throw cms::Exception("BadExceptionType","const char*") 
	  << "cstring = " << c << "\n"
	  << description() ;
      }
    catch(...)
      {
	LogError("BeginJob")
	  << "An unknown Exception occured in\n" << description() << "\n";
	throw;
      }
  }
  
  void Worker::endJob()
  {
    using namespace std;
    
    try
      {
	implEndJob();
      }
    catch(cms::Exception& e)
      {
	LogError("EndJob")
	  << "A cms::Exception is going through "<< workerType()<<":\n";

	// should event id be included?
	e << "A cms::Exception is going through "<< workerType()<<":\n"
	  << description();
	throw;
      }
    catch(seal::Error& e)
      {
	LogError("EndJob")
	  << "A seal::Error is going through "<< workerType()<<":\n"
	  << description() << "\n";
	throw;
      }
    catch(std::exception& e)
      {
	LogError("EndJob")
	  << "An std::exception is going through "<< workerType()<<":\n"
	  << description() << "\n";
	throw;
      }
    catch(std::string& s)
      {
	LogError("EndJob") 
	  << "module caught an std::string during endJob\n";

	throw cms::Exception("BadExceptionType","std::string") 
	  << "string = " << s << "\n"
	  << description() << "\n";
      }
    catch(const char* c)
      {
	LogError("EndJob") 
	  << "module caught an const char* during endJob\n";

	throw cms::Exception("BadExceptionType","const char*") 
	  << "cstring = " << c << "\n"
	  << description() << "\n";
      }
    catch(...)
      {
	LogError("EndJob")
	  << "An unknown Exception occured in\n" << description() << "\n";
	throw;
      }
  }
  
}
