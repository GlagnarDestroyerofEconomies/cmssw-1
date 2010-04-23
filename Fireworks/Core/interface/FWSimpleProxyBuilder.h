#ifndef Fireworks_Core_FWSimpleProxyBuilder_h
#define Fireworks_Core_FWSimpleProxyBuilder_h
// -*- C++ -*-
//
// Package:     Core
// Class  :     FWSimpleProxyBuilder
//
/**\class FWSimpleProxyBuilder FWSimpleProxyBuilder.h Fireworks/Core/interface/FWSimpleProxyBuilder.h

   Description: <one line class summary>

   Usage:s
    <usage>

 */
//
// Original Author:  Chris Jones, AljaMrak-Tadel
//         Created:  Tue March 28  2 09:46:36 EST 2010
// $Id: FWSimpleProxyBuilder.h,v 1.3 2010/04/20 20:49:41 amraktad Exp $
//

// system include files
#include <typeinfo>

// user include files
#include "Fireworks/Core/interface/FWProxyBuilderBase.h"
#include "Fireworks/Core/interface/FWSimpleProxyHelper.h"

// forward declarations

class FWSimpleProxyBuilder : public FWProxyBuilderBase {

public:
   FWSimpleProxyBuilder(const std::type_info& iType);
   virtual ~FWSimpleProxyBuilder();

   // ---------- const member functions ---------------------

   // ---------- static member functions --------------------
   ///Used by the plugin system to determine how the proxy uses the data from FWEventItem
   static std::string typeOfBuilder();

   // ---------- member functions ---------------------------

protected:
   virtual void build(const FWEventItem* iItem, TEveElementList* product);
   virtual void buildViewType(const FWEventItem* iItem, TEveElementList* product, FWViewType::EType viewType);

private:
   FWSimpleProxyBuilder(const FWSimpleProxyBuilder&); // stop default

   const FWSimpleProxyBuilder& operator=(const FWSimpleProxyBuilder&); // stop default

   virtual void itemChangedImp(const FWEventItem*);
   
   virtual bool specialModelChangeHandling(const FWModelId&, TEveElement*, FWViewType::EType);

   //called once for each item in collection, the void* points to the
   // object properly offset in memory
   virtual void build(const void*, unsigned int iIndex, TEveElement& iItemHolder) = 0;
   virtual void buildViewType(const void*, unsigned int iIndex, TEveElement& iItemHolder, FWViewType::EType) = 0;

   // ---------- member data --------------------------------
   FWSimpleProxyHelper m_helper;

};


#endif
