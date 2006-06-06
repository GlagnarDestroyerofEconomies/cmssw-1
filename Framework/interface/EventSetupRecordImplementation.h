#ifndef Framework_EventSetupRecordImplementation_h
#define Framework_EventSetupRecordImplementation_h
// -*- C++ -*-
//
// Package:     Framework
// Class  :     EventSetupRecordImplementation
// 
/**\class EventSetupRecordImplementation EventSetupRecordImplementation.h FWCore/Framework/interface/EventSetupRecordImplementation.h

 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Chris Jones
// Created:     Fri Apr  1 16:50:49 EST 2005
// $Id$
//

// system include files
#include <string>
#include <iostream>

// user include files
#include "FWCore/Framework/interface/EventSetupRecord.h"
#include "FWCore/Framework/interface/EventSetupRecordKey.h"
#include "FWCore/Framework/interface/DataKey.h"

// forward declarations
namespace edm {
   namespace eventsetup {
     class ComponentDescription;
     
template<class T>
class EventSetupRecordImplementation : public EventSetupRecord
{

   public:
      //virtual ~EventSetupRecordImplementation();

      // ---------- const member functions ---------------------
      template< typename HolderT>
         void get(HolderT& iHolder) const {
            const typename HolderT::value_type* value;
            const ComponentDescription* desc;
            this->getImplementation(value, "",desc);
                                                      
            iHolder = HolderT(value,desc);
         }

      template< typename HolderT>
      void get(const char* iName, HolderT& iHolder) const {
         const typename HolderT::value_type* value;
         const ComponentDescription* desc;
         this->getImplementation(value, iName,desc);
         iHolder = HolderT(value,desc);
      }
      template< typename HolderT>
      void get(const std::string& iName, HolderT& iHolder) const {
         const typename HolderT::value_type* value;
         const ComponentDescription* desc;
         this->getImplementation(value, iName.c_str(),desc);
         iHolder = HolderT(value,desc);
      }
      
      virtual EventSetupRecordKey key() const {
         return EventSetupRecordKey::makeKey<T>();
      }

      virtual bool doGet(const DataKey& aKey) const {
         const DataProxy* proxy = find(aKey);
         if(0 != proxy) {
            proxy->doGet(*this, aKey);
         }
         return 0 != proxy;
      }
      
      // ---------- static member functions --------------------
      static EventSetupRecordKey keyForClass()  {
         return EventSetupRecordKey::makeKey<T>();
      }
   
      // ---------- member functions ---------------------------
 
   protected:
      EventSetupRecordImplementation() {}

   private:
      EventSetupRecordImplementation(const EventSetupRecordImplementation&); // stop default

      const EventSetupRecordImplementation& operator=(const EventSetupRecordImplementation&); // stop default
      
      template < typename DataT > 
         void getImplementation(DataT const *& iData ,
                                const char* iName,
                                const ComponentDescription*& iDesc) const;       // ---------- member data --------------------------------

};
   }
}
#include "FWCore/Framework/interface/recordGetImplementation.icc"

#endif
