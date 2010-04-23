#ifndef Fireworks_Core_FWProxyBuilderBase_h
#define Fireworks_Core_FWProxyBuilderBase_h
// -*- C++ -*-
//
// Package:     Core
// Class  :     FWProxyBuilderBase
// 
/**\class FWProxyBuilderBase FWProxyBuilderBase.h Fireworks/Core/interface/FWProxyBuilderBase.h

 Description: [one line class summary]

 Usage:
    <usage>

*/
//
// Original Author:  Chris Jones, Matevz Tadel, Alja Mrak-Tadel
//         Created:  Thu Mar 18 14:12:12 CET 2010
// $Id: FWProxyBuilderBase.h,v 1.6 2010/04/20 20:49:41 amraktad Exp $
//

// system include files

// user include files

// user include files
#include "Fireworks/Core/interface/FWEvePtr.h"
#include "Fireworks/Core/interface/FWViewType.h"
#include "Fireworks/Core/interface/FWProxyBuilderFactory.h"
#include "Fireworks/Core/interface/FWModelChangeSignal.h"
#include "Fireworks/Core/interface/FWModelIdFromEveSelector.h"

// forward declarations

class FWEventItem;
class TEveElementList;
class TEveElement;
class FWModelId;
class FWViewContext;
class FWInteractionList;

namespace fireworks {
   class Context;
}

class FWProxyBuilderBase
{
public:

   FWProxyBuilderBase();
   virtual ~FWProxyBuilderBase();

   // ---------- const member functions ---------------------

   const fireworks::Context& context() const;
   const FWEventItem* item() const {
      return m_item;
   }
   // ---------- static member functions --------------------
   ///Used by the plugin system to determine how the proxy uses the data from FWEventItem
   static std::string typeOfBuilder();

   // ---------- member functions ---------------------------
   void setItem(const FWEventItem* iItem);
   void setHaveWindow(bool iFlag);
   void build();

   void modelChanges(const FWModelIds&);
   void itemChanged(const FWEventItem*);

   virtual bool canHandle(const FWEventItem&);//note pass FWEventItem to see if type and container match

   virtual TEveElementList* createProduct(FWViewType::EType, FWViewContext*);

   virtual void setInteractionList(FWInteractionList*, const std::string&);
   virtual void itemBeingDestroyed(const FWEventItem*);

   // const member functions   
   virtual bool haveSingleProduct() const { return true; }
   virtual bool havePerViewProduct() const { return false; }
   virtual bool willHandleInteraction() const { return false; }
   bool getHaveWindow() const { return m_haveWindow; }
   void setupElement(TEveElement* el, bool color = true) const;
   void setupAddElement(TEveElement* el, TEveElement* parent,  bool set_color = true) const;
   int  layer() const;

 
protected:

   struct Product
   {
      FWViewType::EType m_viewType;
      FWViewContext*    m_viewContext;
      TEveElementList*  m_elements;

      Product(FWViewType::EType t, FWViewContext* c) : m_viewType(t), m_viewContext(c), m_elements(0)
      {
         m_elements = new TEveElementList("ProxyProduct");
         m_elements->IncDenyDestroy();
      }
        
      ~Product()
      {
         m_elements->DecDenyDestroy();
         m_elements->DestroyElements();
      }
   };


   std::vector<FWModelIdFromEveSelector>& ids() {
      return m_ids;
   }
   
   //Override this if you need to special handle selection or other changes
   virtual bool specialModelChangeHandling(const FWModelId&, TEveElement*, FWViewType::EType);
   virtual void applyChangesToAllModels(TEveElement*, FWViewType::EType);

   virtual void modelChanges(const FWModelIds&, TEveElement*, FWViewType::EType);

   FWProxyBuilderBase(const FWProxyBuilderBase&); // stop default
   const FWProxyBuilderBase& operator=(const FWProxyBuilderBase&); // stop default

   virtual void build(const FWEventItem* iItem, TEveElementList* product);
   virtual void buildViewType(const FWEventItem* iItem,TEveElementList* product, FWViewType::EType);

   void clean();
   virtual void cleanLocal();

   // utility
   TEveCompound* createCompound(bool set_color = true, bool propagate_color_to_all_children = true) const;

   FWInteractionList*    m_interactionList;
private:
   void applyChangesToAllModels();
   void cleanProduct(Product* p);
   void setProjectionLayer(float);

   // ---------- member data --------------------------------
   typedef std::vector<Product*>::iterator Product_it;

   const FWEventItem* m_item;

   std::vector<FWModelIdFromEveSelector> m_ids;
   std::vector<Product*> m_products;

   bool m_modelsChanged;
   bool m_haveWindow;
   bool m_mustBuild;

   float m_layer;
};

#endif
