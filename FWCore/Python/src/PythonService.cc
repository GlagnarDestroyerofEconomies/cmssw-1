// -*- C++ -*-
//
// Package:     Python
// Class  :     PythonService
// 
// Implementation:
//     <Notes on implementation>
//
// Original Author:  Benedikt Hegner
//         Created:  Sun Jul 23 11:31:33 CEST 2006
//

// system include files

// user include files
#include "FWCore/Python/src/PythonService.h"

#include "DataFormats/Provenance/interface/ModuleDescription.h"
#include "DataFormats/Provenance/interface/EventID.h"
#include "DataFormats/Provenance/interface/Timestamp.h"

//
// constructor
//
PythonService::PythonService(const edm::ParameterSet& iConfig, edm::ActivityRegistry& iRegistry):
    handle_(PythonManager::handle())
{
    
    std::cout << "Start preparing PythonService" << std::endl;
    const std::string fileName = iConfig.getParameter<std::string>("fileName");
	
    using namespace boost::python;
	
    command_ = "from "+fileName + " import *\n";
    object main_module(( boost::python::handle<>(borrowed(PyImport_AddModule(const_cast<char *>("__main__"))))));
    object main_namespace = main_module.attr("__dict__");
    try {
      object result((boost::python::handle<>(PyRun_String(command_.c_str(),
                                           Py_file_input,
                                           main_namespace.ptr(),
                                           main_namespace.ptr()))));
      service_ = main_namespace["service"];
    } catch( ... ) {
      throw cms::Exception("Configuration") <<"No 'service' python variable defined in given fileName parameter.\n Please create an instance of the python class you want to use and pass that instance to the variable named 'service'.";
    }
	
    // connect methods and signals
    // later on here will be a check what python methods are present
    // for now we expect postBeginJob, postEndJob and postProcessEvent
	
    iRegistry.watchPostBeginJob(this,&PythonService::postBeginJob);
    iRegistry.watchPostEndJob(this,&PythonService::postEndJob);
    iRegistry.watchPostProcessEvent(this,&PythonService::postProcessEvent);
}

//
// destructor
//
PythonService::~PythonService()
{
}


//
// member functions
//
void PythonService::postBeginJob()
{
	using namespace boost::python;
    object main_module((boost::python::handle<>(borrowed(PyImport_AddModule(const_cast<char *>("__main__"))))));   
    object main_namespace = main_module.attr("__dict__");
    main_namespace["tempService"] = service_;

    try {
      object result((boost::python::handle<>(PyRun_String("tempService.postBeginJob()",
					   Py_eval_input,
					   main_namespace.ptr(),
					   main_namespace.ptr()))));      
    }catch( error_already_set ) {
      pythonToCppException("RuntimeError");
    }
}


void PythonService::postEndJob()
{
    using namespace boost::python;
    object main_module((boost::python::handle<>(borrowed(PyImport_AddModule(const_cast<char *>("__main__"))))));   
    object main_namespace = main_module.attr("__dict__");
    main_namespace["tempService"] = service_;

    try {
       object result((boost::python::handle<>(PyRun_String("tempService.postEndJob()",
					   Py_eval_input,
					   main_namespace.ptr(),
					   main_namespace.ptr()))));      
    }catch( error_already_set ) {
       pythonToCppException("RuntimeError");
    }
}


void PythonService::preProcessEvent(const edm::EventID& iID, const edm::Timestamp& iTime)
{
   
}


void PythonService::postProcessEvent(const edm::Event&, const edm::EventSetup&)
{
    using namespace boost::python;
    object main_module((boost::python::handle<>(borrowed(PyImport_AddModule(const_cast<char *>("__main__"))))));   
    object main_namespace = main_module.attr("__dict__");
    main_namespace["tempService"] = service_;

    try {
       object result((boost::python::handle<>(PyRun_String("tempService.postProcessEvent()",
					   Py_eval_input,
					   main_namespace.ptr(),
					   main_namespace.ptr()))));      
    }catch( error_already_set ) {
       pythonToCppException("RuntimeError");
    }

}
