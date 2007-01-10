#ifndef StreamerInputModule_h
#define StreamerInputModule_h

#include "DataFormats/Common/interface/Wrapper.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "IOPool/Streamer/interface/ClassFiller.h"
#include "FWCore/Framework/interface/EventPrincipal.h"

#include "DataFormats/Streamer/interface/StreamedProducts.h"
#include "DataFormats/Common/interface/ProductRegistry.h"
#include "FWCore/Utilities/interface/DebugMacros.h"
#include "IOPool/Streamer/interface/StreamerFileIO.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/ProductRegistry.h"
#include "FWCore/Framework/interface/InputSource.h"

#include "IOPool/Streamer/interface/InitMessage.h"
#include "IOPool/Streamer/interface/EventMessage.h"
#include "IOPool/Streamer/interface/StreamDeserializer.h"
#include "IOPool/Streamer/interface/Utilities.h"

#include "DataFormats/Common/interface/ProcessConfiguration.h"

#include <memory>
#include <string>
#include <fstream>
#include <vector>
#include <utility>
#include <iostream>
#include <algorithm> 
#include <iterator>

namespace edm
{
  template <class Producer>
  class StreamerInputModule : public InputSource
  {
  /**
     Requires the Producer class to provide following functions
           const InitMsgView* getHeader();
           const EventMsgView* getNextEvent();
  */
  public:  
    explicit StreamerInputModule(ParameterSet const& pset,
                 InputSourceDescription const& desc);
    virtual ~StreamerInputModule();
    virtual std::auto_ptr<EventPrincipal> read();

  private:
    void mergeWithRegistry(const SendDescs& descs,ProductRegistry&);
    void declareStreamers(const SendDescs& descs);
    void buildClassCache(const SendDescs& descs);

    //ProductRegistry const* prod_reg_;
    Producer* pr_; 
    StreamDeserializer deserializer_;
  }; //end-of-class-def

template <class Producer>
StreamerInputModule<Producer>::~StreamerInputModule()
   {
      delete pr_;
   }

template <class Producer>
StreamerInputModule<Producer>::StreamerInputModule(
                    ParameterSet const& pset,
                    InputSourceDescription const& desc):
    InputSource(pset, desc),
    //prod_reg_(&productRegistry()), 
    pr_(new Producer(pset)),
    deserializer_()
    {
      //Get header/init from Producer
      const InitMsgView* header = pr_->getHeader();
      std::auto_ptr<SendJobHeader> p = deserializer_.deserializeRegistry(*header); 
      SendDescs & descs = p->descs_;
      mergeWithRegistry(descs, productRegistry());

      // jbk - the next line should not be needed
      declareStreamers(descs);
      buildClassCache(descs);
      loadExtraClasses();
    }

template <class Producer>
std::auto_ptr<EventPrincipal> StreamerInputModule<Producer>::read()
  {
    const EventMsgView* eview = pr_->getNextEvent();

    if (pr_->newHeader()) {   
        FDEBUG(6) << "A new file has been opened and we must compare Heraders here !!"<<endl;
        // A new file has been opened and we must compare Heraders here !!
        //Get header/init from Producer
        const InitMsgView* header = pr_->getHeader();
        std::auto_ptr<SendJobHeader> p = deserializer_.deserializeRegistry(*header);
        if (!registryIsSubset(*p, productRegistry()) ) {
            std::cout << "\n\nUn matching Init Message Headers found.\n";
            throw cms::Exception("read","StreamerInputModule")
                 << "Un matching Headers found.\n";
        }
    } 
    if (eview == 0)  
    {
        return  std::auto_ptr<EventPrincipal>();
    }
    std::auto_ptr<EventPrincipal> pEvent(deserializer_.deserializeEvent(*eview, productRegistry()));
    return pEvent;
  }

template <class Producer>
void StreamerInputModule<Producer>::mergeWithRegistry(const SendDescs& descs,ProductRegistry& reg)
  {
    SendDescs::const_iterator i(descs.begin()), e(descs.end());

    // the next line seems to be not good.  what if the productdesc is
    // already there? it looks like I replace it.  maybe that it correct

    FDEBUG(6) << "mergeWithRegistry: Product List: " << endl;
    std::string processName;
    if( i != e ) {
       processName = i->processName();
    }
    for(; i != e; ++i) {
        reg.copyProduct(*i);
        FDEBUG(6) << "StreamInput prod = " << i->className() << endl;

	if(processName != i->processName()) {
	   throw cms::Exception("MultipleProcessNames")
	      <<"at least two different process names ('"
	      <<processName
	      <<"', '"
	      <<i->processName()
	      <<"' found in JobHeader. We can only support one.";
	}
    }
  }

template <class Producer>
void StreamerInputModule<Producer>::declareStreamers(const SendDescs& descs)
  {
    SendDescs::const_iterator i(descs.begin()), e(descs.end());

    for(; i != e; ++i) {
        //pi->init();
        std::string real_name = wrappedClassName(i->className());
        FDEBUG(6) << "declare: " << real_name << endl;
        loadCap(real_name);
    }
  }


template <class Producer>
void StreamerInputModule<Producer>::buildClassCache(const SendDescs& descs)
  { 
    SendDescs::const_iterator i(descs.begin()), e(descs.end());

    for(; i != e; ++i) {
        //pi->init();
        std::string real_name = wrappedClassName(i->className());
        FDEBUG(6) << "BuildReadData: " << real_name << endl;
        doBuildRealData(real_name);
    }
  }

} // end of namespace-edm
  
#endif
