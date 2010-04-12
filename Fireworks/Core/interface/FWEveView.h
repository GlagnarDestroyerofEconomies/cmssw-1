// -*- C++ -*-
//
// Package:     Core
// Class  :     FWEveView
// 
// Implementation:
//     [Notes on implementation]
//
// Original Author:  Alja Mrak-Tadel
//         Created:  Thu Mar 16 14:11:32 CET 2010
// $Id: FWEveView.h,v 1.5 2010/04/09 17:23:57 amraktad Exp $
//


#ifndef Fireworks_Core_FWEveView_h
#define Fireworks_Core_FWEveView_h

// user include files
#include "Fireworks/Core/interface/FWViewBase.h"
#include "Fireworks/Core/interface/FWViewType.h"
#include "Fireworks/Core/interface/FWDoubleParameter.h"
#include "Fireworks/Core/interface/FWBoolParameter.h"
#include "Fireworks/Core/interface/FWLongParameter.h"
#include "Fireworks/Core/interface/FWEvePtr.h"

// forward declarations
class TGLViewer;
class TGLOrthoCamera;
class TGLPerspectiveCamera;
class TEveViewer;
class TEveElementList;
class TEveScene;
class TEveWindowSlot;

class FWEventAnnotation;
class CmsAnnotation;
class FWViewContextMenuHandlerGL;
class DetIdToMatrix;
class FWColorManager;

namespace fireworks
{
class Context;
}

class FWEveView : public FWViewBase
{
public:
   FWEveView(TEveWindowSlot*);
   virtual ~FWEveView();

   virtual void addTo(FWConfiguration&) const;
   virtual void setFrom(const FWConfiguration&);
   virtual FWViewContextMenuHandlerBase* contextMenuHandler() const;

   virtual void saveImageTo(const std::string& iName) const;
   virtual void setBackgroundColor(Color_t);
   virtual void eventEnd();

   virtual void setContext(fireworks::Context&) {}

   // ---------- const member functions --------------------- 

   // ---------- static member functions --------------------
   virtual const std::string& typeName() const;
   FWViewType::EType typeId() const { return m_type.id(); }
   
 //  const FWViewType& viewType() const { return m_viewType; }

   TEveViewer* viewer()      { return m_viewer; }
   TEveScene*  eventScene()  { return m_eventScene;  }
   TEveScene*  geoScene()    { return m_geoScene; }
   TGLViewer*  viewerGL() const;

protected:
   void setType(FWViewType::EType t);
   void setEventScene(TEveScene* s);

   virtual void resetCamera();
   virtual void lineWidthChanged();

   void addToOrthoCamera(TGLOrthoCamera*, FWConfiguration&) const;
   void setFromOrthoCamera(TGLOrthoCamera*, const FWConfiguration&);
   void addToPerspectiveCamera(TGLPerspectiveCamera*, const std::string&, FWConfiguration&) const;
   void setFromPerspectiveCamera(TGLPerspectiveCamera*,  const std::string&, const FWConfiguration&);

private:
   FWEveView(const FWEveView&);    // stop default
   const FWEveView& operator=(const FWEveView&);    // stop default
  

   // ---------- member data --------------------------------

   FWViewType           m_type;
   TEveViewer*          m_viewer;
   TEveScene*           m_eventScene;
   TEveScene*           m_geoScene;

   FWEventAnnotation*   m_overlayEventInfo;  
   CmsAnnotation*       m_overlayLogo;

   // parameters
#if ROOT_VERSION_CODE >= ROOT_VERSION(5,26,0)
   FWDoubleParameter   m_imageScale;
#endif
   FWLongParameter   m_eventInfoLevel;
   FWBoolParameter   m_drawCMSLogo;

#if ROOT_VERSION_CODE < ROOT_VERSION(5,26,0)
   FWDoubleParameter m_lineWidth;
#endif

   boost::shared_ptr<FWViewContextMenuHandlerGL>   m_viewContextMenu;
};


#endif
