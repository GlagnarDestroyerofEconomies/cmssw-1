#ifndef DataFormats_Common_EDProductGetter_h
#define DataFormats_Common_EDProductGetter_h
// -*- C++ -*-
//
// Package:     EDProduct
// Class  :     EDProductGetter
// 
/**\class EDProductGetter EDProductGetter.h DataFormats/Common/interface/EDProductGetter.h

 Description: Abstract base class used internally by the RefBase to obtain the EDProduct from the Event

 Usage:
    This is used internally by the edm::Ref classes.

*/
//
// Original Author:  Chris Jones
//         Created:  Tue Nov  1 15:06:31 EST 2005
// $Id: EDProductGetter.h,v 1.7 2008/12/18 04:53:07 wmtan Exp $
//

// system include files
#include "boost/utility.hpp"

// user include files
#include "DataFormats/Provenance/interface/ProductID.h"

// forward declarations

namespace edm {
   class EDProduct;
   class EDProductGetter : private boost::noncopyable {

   public:
   
      EDProductGetter();
      virtual ~EDProductGetter();

      // ---------- const member functions ---------------------
      virtual EDProduct const* getIt(ProductID const&) const = 0;
      
      // ---------- member functions ---------------------------

      ProductID oldToNewProductID(ProductID const& oldProductID) const {
	if (oldProductID.oldID() == 0) return oldProductID;
	return oldToNewProductID_(oldProductID);
      }
private:
      virtual ProductID oldToNewProductID_(ProductID const& oldProductID) const;
      /**This does not take ownership of the argument, so it is up to the caller to be
         sure that the object lifetime is greater than the time for which it is set*/
      static EDProductGetter const* set(EDProductGetter const*);
      // ---------- member data --------------------------------
      
   };

   EDProductGetter const*
   mustBeNonZero(EDProductGetter const* prodGetter, std::string refType, ProductID const& productID);
}

#endif
