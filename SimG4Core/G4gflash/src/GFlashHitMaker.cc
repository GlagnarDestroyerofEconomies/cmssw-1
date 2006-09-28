//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: GFlashHitMaker.cc,v 1.7 2006/06/29 19:14:12 gunter Exp $
// GEANT4 tag $Name: geant4-08-01-patch-01 $
//
//
// ------------------------------------------------------------
// GEANT 4 class implementation
//
//      ---------------- GFlashHitMaker ----------------
//
// Authors: E.Barberio & Joanna Weng 
// ------------------------------------------------------------

#include "G4ios.hh"
#include "G4TransportationManager.hh"
#include "G4VSensitiveDetector.hh"
#include "G4TouchableHandle.hh"
#include "G4VGFlashSensitiveDetector.hh"

#include "GFlashHitMaker.hh"
#include "G4GFlashSpot.hh"

GFlashHitMaker::GFlashHitMaker()
{
  fTouchableHandle   = new G4TouchableHistory(); // talk to ?@@@
  fpNavigator        = new G4Navigator();
  fNaviSetup         = false;
}

GFlashHitMaker::~GFlashHitMaker()
{
  delete fpNavigator;
}

void GFlashHitMaker::make(GFlashEnergySpot * aSpot, const G4FastTrack * aT)
{
  // Locate the spot
  if (!fNaviSetup)
  {
    fpNavigator->
      SetWorldVolume(G4TransportationManager::GetTransportationManager()->
                     GetNavigatorForTracking()->GetWorldVolume() );
    fpNavigator->
      LocateGlobalPointAndUpdateTouchable(aSpot->GetPosition(),
                                          fTouchableHandle(), false);
    fNaviSetup = true;
  }
  else
  {
    fpNavigator->
      LocateGlobalPointAndUpdateTouchable(aSpot->GetPosition(),
                                          fTouchableHandle());
  }
  
  //--------------------------------------
  // Fills attribute of the G4Step needed
  // by our sensitive detector:
  //-------------------------------------
  // set spot information:
  G4GFlashSpot theSpot(aSpot, aT, fTouchableHandle);
  ///Navigator
  //--------------------------------------
  // Produce Hits
  // call sensitive part: taken/adapted from the stepping:
  // Send G4Step information to Hit/Dig if the volume is sensitive
  //--------------G4TouchableHistory----------------------------------------
  
  G4VPhysicalVolume* pCurrentVolume = fTouchableHandle()->GetVolume();    
  G4VSensitiveDetector* pSensitive;
  if( pCurrentVolume != 0 )
  {
    pSensitive = pCurrentVolume->GetLogicalVolume()->GetSensitiveDetector();
    G4VGFlashSensitiveDetector * gflashSensitive = 
                   dynamic_cast<G4VGFlashSensitiveDetector * > (pSensitive);
    if( gflashSensitive )
    {
      gflashSensitive->Hit(&theSpot);
    }
    else if ( (!gflashSensitive ) && 
             ( pSensitive ) && 
             ( pCurrentVolume->GetLogicalVolume()->GetFastSimulationManager() )
            ) // Using gflash without implementing the 
              // gflashSensitive detector interface -> not allowed!
    
    {    
      G4cerr << "ERROR - GFlashHitMaker::make()" << G4endl
             << "        It is required to implement the "<< G4endl
             << "        G4VGFlashSensitiveDetector interface in "<< G4endl
             << "        addition to the usual SensitiveDetector class."
             << G4endl;
      G4Exception("GFlashHitMaker::make()", "InvalidSetup", FatalException, 
                  "G4VGFlashSensitiveDetector interface not implemented.");
    }
  }
  else
  {     
    #ifdef GFLASH_DEBUG
    G4cout << "GFlashHitMaker::Out of volume  "<< G4endl;
    #endif
  }
}

bool GFlashHitMaker::makesensitive(GFlashEnergySpot * aSpot, const G4FastTrack * aT)
{
        bool filter = false;
        if (!fNaviSetup){
                fpNavigator->
                SetWorldVolume(G4TransportationManager::GetTransportationManager()->
                GetNavigatorForTracking()->GetWorldVolume() );
                fpNavigator->
                LocateGlobalPointAndUpdateTouchable(aSpot->GetPosition(), fTouchableHandle(), false);
                fNaviSetup = true;
        }
        else    {
                fpNavigator->
                LocateGlobalPointAndUpdateTouchable(aSpot->GetPosition(), fTouchableHandle());
        }
        G4VPhysicalVolume* pCurrentVolume = fTouchableHandle()->GetVolume();
        G4VSensitiveDetector* pSensitive;
        if( pCurrentVolume != 0 ){
                //   std::cout << "GFlashHitMaker::curent vol" << pCurrentVolume->GetLogicalVolume()->GetName()   << std::endl;
                pSensitive = pCurrentVolume->GetLogicalVolume()->GetSensitiveDetector();
                G4VGFlashSensitiveDetector * gflashSensitive = dynamic_cast<G4VGFlashSensitiveDetector * > (pSensitive);
                // don't deposit -> not sensitve
                if( !gflashSensitive )           {
                        filter= false;
                }
                                                                                                                                                              
                if( gflashSensitive ){
                        G4GFlashSpot theSpot(aSpot, aT, fTouchableHandle);
                        // std::cout << "GFlashHitMaker:: MAke Spot" <<  theSpot.GetEnergySpot()->GetEnergy()/GeV<<std::endl;
                        gflashSensitive->Hit(&theSpot);
                        filter =true;
                }
                                                                                                                                                              
                // interface not correctly implemented
                if ( (!gflashSensitive ) &&
                        ( pSensitive ) &&
                ( pCurrentVolume->GetLogicalVolume()->GetFastSimulationManager() )
                ) // Using gflash without implementing the
                // gflashSensitive detector interface -> not allowed!
                {
                        std::cout<<"When using GFlash in geant4, please implement the "<<std::endl;
                        std::cout<<"G4VGFlashSensitiveDetector interface in addition to the"<<std::endl;
                        std::cout<<"G4VSensitiveDetector interface in the relevant sensitive detector."<<std::endl;
                        G4Exception("GFlashHitMaker: G4VGFlashSensitiveDetector interface not implemented");
                }
        }   //  pCurrentVolume != 0 )
                                                                                                                                                              
                                                                                                                                                              
        else
        {
                #ifdef GFLASH_DEBUG
                std::cout << "GFlashHitMaker::Out of volume  "<<endl;
                #endif
        }
        return filter;
}

// checks if in sensitive Volume
bool GFlashHitMaker::check(  const G4FastTrack * aT){
         
        bool select=false;
        G4TouchableHistory* touch = (G4TouchableHistory*)(aT->GetPrimaryTrack()->GetTouchable());
        G4VPhysicalVolume* pCurrentVolume = touch->GetVolume();
        G4VSensitiveDetector* pSensitive;
        if( pCurrentVolume != 0) {
                //        std::cout << "GFlashShowerModel " << pCurrentVolume->GetLogicalVolume()->GetName()   << std::endl;
                pSensitive = pCurrentVolume->GetLogicalVolume()->GetSensitiveDetector();
                G4VGFlashSensitiveDetector * gflashSensitive =
                dynamic_cast<G4VGFlashSensitiveDetector * > (pSensitive);
                if( !gflashSensitive ) select = false;
                if ( gflashSensitive)  select = true;
        }
        return select;
}
 
