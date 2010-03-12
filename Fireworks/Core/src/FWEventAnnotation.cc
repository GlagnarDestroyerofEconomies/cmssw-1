#include "TGLIncludes.h"
#include "TROOT.h"
#include "TColor.h"
#include "TGLUtil.h"
#include "TGLCamera.h"
#include "TGLRnrCtx.h"
#include "TGLSelectRecord.h"
#include "TGLViewerBase.h"
#include "TObjString.h"
#include "TGLViewer.h"
#include "TMath.h"
#include "TImage.h"
#include <KeySymbols.h>

#include "Fireworks/Core/interface/FWEventAnnotation.h"
#include "Fireworks/Core/interface/FWGUIManager.h"
#include "Fireworks/Core/interface/BuilderUtils.h"
#include "Fireworks/Core/interface/FWConfiguration.h"

#include "DataFormats/FWLite/interface/Event.h"

FWEventAnnotation::FWEventAnnotation(TGLViewerBase *view):
   TGLAnnotation(view, "Event Info", 0.05, 0.95),
   m_event(0),
   m_level(1)
{
   SetRole(TGLOverlayElement::kViewer);
   SetUseColorSet(true);
   fAllowClose = false;
}

FWEventAnnotation::~FWEventAnnotation()
{
}

//______________________________________________________________________________

void
FWEventAnnotation::setLevel(long x)
{ 
   m_level = x;
   updateOverlayText();
}


void
FWEventAnnotation::setEvent()
{
   m_event = FWGUIManager::getGUIManager()->getCurrentEvent();
   updateOverlayText();
}

void
FWEventAnnotation::updateOverlayText()
{
   fText = "CMS Experiment at LHC, CERN";

   if (m_event && m_level)
   {
      fText += "\nData recorded: ";
      fText += fw::getLocalTime( *m_event );
      fText += "\nRun/Event: ";
      fText += m_event->id().run();
      fText += " / ";
      fText += m_event->id().event();
      if ( m_level > 1)
      {
         fText += "\nLumi section: ";
	 fText += m_event->luminosityBlock();
      }
      if ( m_level > 2)
      {
         fText += "\nOrbit/Crossing: ";
	 fText += m_event->orbitNumber();
	 fText += " / ";
	 fText += m_event->bunchCrossing();
      }
   }
   fParent->RequestDraw();
}

void
FWEventAnnotation::Render(TGLRnrCtx& rnrCtx)
{
   if (m_level)
      TGLAnnotation::Render(rnrCtx);
}

//______________________________________________________________________________

void
FWEventAnnotation::addTo(FWConfiguration& iTo) const
{
   std::stringstream s;
   s<<fTextSize;
   iTo.addKeyValue("EventInfoTextSize",FWConfiguration(s.str()));

   std::stringstream x;
   x<<fPosX;
   iTo.addKeyValue("EventInfoPosX",FWConfiguration(x.str()));

   std::stringstream y;
   y<<fPosY;
   iTo.addKeyValue("EventInfoPosY",FWConfiguration(y.str()));
}

void
FWEventAnnotation::setFrom(const FWConfiguration& iFrom) 
{
   const FWConfiguration* value;

   value = iFrom.valueForKey("EventInfoTextSize");
   if (value) fTextSize = atof(value->value().c_str());

   value = iFrom.valueForKey("EventInfoPosX");
   if (value) fPosX = atof(value->value().c_str());

   value = iFrom.valueForKey("EventInfoPosY");
   if (value) fPosY = atof(value->value().c_str());

}

