#include "Geometry/Vector/interface/LocalPoint.h"
#include "Geometry/Vector/interface/LocalVector.h"

#include "SimG4Core/SensitiveDetector/interface/FrameRotation.h"
#include "SimG4Core/Notification/interface/TrackInformation.h"
#include "SimG4Core/Notification/interface/G4TrackToParticleID.h"
#include "SimG4Core/Geometry/interface/SDCatalog.h"
#include "SimG4Core/Physics/interface/G4ProcessTypeEnumerator.h"

#include "SimDataFormats/TrackingHit/interface/UpdatablePSimHit.h"
#include "SimDataFormats/SimHitMaker/interface/TrackingSlaveSD.h"

#include "SimG4CMS/Tracker/interface/TkAccumulatingSensitiveDetector.h"
#include "SimG4CMS/Tracker/interface/TrackingSlaveSDWithRenumbering.h"
#include "SimG4CMS/Tracker/interface/FakeFrameRotation.h"
#include "SimG4CMS/Tracker/interface/StandardFrameRotation.h"
#include "SimG4CMS/Tracker/interface/TrackerFrameRotation.h"
#include "SimG4CMS/Tracker/interface/TkSimHitPrinter.h"
#include "SimG4CMS/Tracker/interface/TrackerG4SimHitNumberingScheme.h"



#include "G4SDManager.hh"
#include "G4VProcess.hh"
#include "G4EventManager.hh"
#include "G4Event.hh"

#include <string>
#include <vector>

//#define FAKEFRAMEROTATION
#define DEBUG
//#define DEBUG_TRACKID

using std::cout;
using std::endl;
using std::vector;
using std::string;

TkAccumulatingSensitiveDetector::TkAccumulatingSensitiveDetector(string name, 
								 const DDCompactView & cpv,
								 edm::ParameterSet const & p) : 
  SensitiveTkDetector(name, cpv, p), myName(name), myRotation(0),  mySimHit(0),
  oldVolume(0), lastId(0), lastTrack(0), eventno(0) {

  edm::ParameterSet m_TrackerSD = p.getParameter<edm::ParameterSet>("TrackerSD");
  allowZeroEnergyLoss = m_TrackerSD.getParameter<bool>("ZeroEnergyLoss");
  neverAccumulate     = m_TrackerSD.getParameter<bool>("NeverAccumulate");
  printHits           = m_TrackerSD.getParameter<bool>("PrintHits");
  theSigma            = m_TrackerSD.getParameter<double>("ElectronicSigmaInNanoSeconds");



  std::cout<<" Constructing a TkAccumulatingSensitiveDetector with "<<name<<std::endl;


#ifndef FAKEFRAMEROTATION
  // No Rotation given in input, automagically choose one based upon the name
  if (name.find("TrackerHits") != string::npos) 
    {
      cout <<" TkAccumulatingSensitiveDetector: using TrackerFrameRotation for "<<myName<<endl;
      myRotation = new TrackerFrameRotation;
    }
  // Just in case (test beam etc)
  if (myRotation == 0) 
    {
      cout <<" TkAccumulatingSensitiveDetector: using StandardFrameRotation for "<<myName<<endl;
      myRotation = new StandardFrameRotation;
    }
#else
  myRotation = new FakeFrameRotation;
  cout << " WARNING - Using FakeFrameRotation in TkAccumulatingSensitiveDetector;" << endl 
       << "This is NOT producing a usable DB," 
       << " but should be used for debugging purposes only." << endl;
  
#endif
    slaveLowTof  = new TrackingSlaveSDWithRenumbering(name+"LowTof");
    slaveHighTof = new TrackingSlaveSDWithRenumbering(name+"HighTof");
  
    // Now attach the right detectors (LogicalVolumes) to me
    vector<string>  lvNames= SensitiveDetectorCatalog::instance()->logicalNames(name);
    this->Register();
    for (vector<string>::iterator it = lvNames.begin();  it != lvNames.end(); it++)
    {
	cout << name << " attaching LV " << *it << endl;
	this->AssignSD(*it);
    }

    tkG4SimHitNumberingScheme = new TrackerG4SimHitNumberingScheme;
    theG4ProcessTypeEnumerator = new G4ProcessTypeEnumerator;
    myG4TrackToParticleID = new G4TrackToParticleID;
}

TkAccumulatingSensitiveDetector::~TkAccumulatingSensitiveDetector() 
{ 
  delete slaveLowTof;
  delete slaveHighTof;
  delete tkG4SimHitNumberingScheme;
  delete theG4ProcessTypeEnumerator;
  delete myG4TrackToParticleID;
}


bool TkAccumulatingSensitiveDetector::ProcessHits(G4Step * aStep, G4TouchableHistory * ROhist)
{
#ifdef DEBUG
    cout << " Entering a new Step " << aStep->GetTotalEnergyDeposit() << " " 
	 << aStep->GetPreStepPoint()->GetPhysicalVolume()->GetLogicalVolume()->GetName() << endl;
#endif
    //TimeMe t1(theTimer, false);
    if (aStep->GetTotalEnergyDeposit()>0. || allowZeroEnergyLoss == true)
    {
	if (newHit(aStep) == true)
	{
	    sendHit();
	    createHit(aStep);
	}
	else
	{
	    updateHit(aStep);
	}
	return true;
    }
    return false;
}

uint32_t TkAccumulatingSensitiveDetector::setDetUnitId(G4Step * s)
{ 
  if(tkG4SimHitNumberingScheme){
    tkG4SimHitNumberingScheme = new TrackerG4SimHitNumberingScheme;
  }
#ifdef DEBUG
  cout<<"DetId "<<tkG4SimHitNumberingScheme->g4ToNumberingScheme(s->GetPreStepPoint()->GetTouchable())<<endl;
#endif    
}


int TkAccumulatingSensitiveDetector::tofBin(float tof)
{

    float threshold = 3. * theSigma;
    if (tof < threshold) return 1;
    return 2;
}

Local3DPoint TkAccumulatingSensitiveDetector::toOrcaRef(Local3DPoint in ,G4VPhysicalVolume * v)
{
    if (myRotation !=0) return myRotation->transformPoint(in,v);
    return (in);
}


void TkAccumulatingSensitiveDetector::sendHit()
{  
    if (mySimHit == 0) return;
#ifdef DEBUG
    cout << " Storing PSimHit: " << pname << " " << mySimHit->detUnitId() 
	 << " " << mySimHit->trackId() << " " << mySimHit->energyLoss() 
	 << " " << mySimHit->entryPoint() << " " << mySimHit->exitPoint() << endl;
#endif
    if (printHits)
    {
	TkSimHitPrinter thePrinter("TkHitPositionOSCAR.dat");
	thePrinter.startNewSimHit(myName,oldVolume->GetLogicalVolume()->GetName(),
				  mySimHit->detUnitId(),mySimHit->trackId(),eventno);
	thePrinter.printLocal(mySimHit->entryPoint(),mySimHit->exitPoint());
	thePrinter.printGlobal(globalEntryPoint,globalExitPoint);
	thePrinter.printHitData(mySimHit->energyLoss(),mySimHit->timeOfFlight());
	thePrinter.printMomentumOfTrack(mySimHit->pabs(),pname,
					thePrinter.getPropagationSign(globalEntryPoint,globalExitPoint));
	thePrinter.printGlobalMomentum(px,py,pz);
    }
    
    if (tofBin(mySimHit->timeOfFlight()) == 1)
	slaveLowTof->processHits(*mySimHit);  // implicit conversion (slicing) to PSimHit!!!
    else
	slaveHighTof->processHits(*mySimHit); // implicit conversion (slicing) to PSimHit!!!
    //
    // clean up
    delete mySimHit;
    mySimHit = 0;
    lastTrack = 0;
    lastId = 0;
}

void TkAccumulatingSensitiveDetector::createHit(G4Step * aStep)
{
    if (mySimHit != 0) 
    {
	delete mySimHit;
	mySimHit=0;
    }
    
    G4Track * theTrack  = aStep->GetTrack(); 
    G4VPhysicalVolume * v = aStep->GetPreStepPoint()->GetPhysicalVolume();
    Local3DPoint theEntryPoint = toOrcaRef(SensitiveDetector::InitialStepPosition(aStep,LocalCoordinates),v);  
    Local3DPoint theExitPoint  = toOrcaRef(SensitiveDetector::FinalStepPosition(aStep,LocalCoordinates),v); 
    //
    //	This allows to send he skipEvent if it is outside!
    //
    checkExitPoint(theExitPoint);
    float thePabs             = aStep->GetPreStepPoint()->GetMomentum().mag()/GeV;
    float theTof              = aStep->GetPreStepPoint()->GetGlobalTime()/nanosecond;
    float theEnergyLoss       = aStep->GetTotalEnergyDeposit()/GeV;
    short theParticleType     = myG4TrackToParticleID->particleID(theTrack);
    uint32_t theDetUnitId     = setDetUnitId(aStep);
  
    globalEntryPoint = SensitiveDetector::InitialStepPosition(aStep,WorldCoordinates);
    globalExitPoint = SensitiveDetector::FinalStepPosition(aStep,WorldCoordinates);
    pname = theTrack->GetDynamicParticle()->GetDefinition()->GetParticleName();
  
    if (theDetUnitId == 0)
    {
	cout << " Error: theDetUnitId is not valid." << endl;
	abort();
    }
    unsigned int theTrackID = theTrack->GetTrackID();
  
    // To whom assign the Hit?
    // First iteration: if the track is to be stored, use the current number;
    // otherwise, get to the mother
    int theTrackIDInsideTheSimHit=theTrackID;
    G4VUserTrackInformation * info = theTrack->GetUserInformation();
    if (info == 0) cout << " Error: no UserInformation available " << endl;
    else
    {
	TrackInformation * temp = dynamic_cast<TrackInformation* >(info);
	if (temp ==0) cout << " Error:G4VUserTrackInformation is not a TrackInformation." << endl;
	if (temp->storeTrack() == false) 
	{
	    // Go to the mother!
#ifdef DEBUG_TRACKID
	    cout << " TkAccumulatingSensitiveDetector:createHit(): setting the TrackID from "
		 << theTrackIDInsideTheSimHit;
#endif
	    theTrackIDInsideTheSimHit = theTrack->GetParentID();
#ifdef DEBUG_TRACKID
	    cout << " to the mother one " << theTrackIDInsideTheSimHit << " " << theEnergyLoss << endl;
#endif
	}
	else
	{
#ifdef DEBUG_TRACKID
	    cout << " TkAccumulatingSensitiveDetector:createHit(): leaving the current TrackID " 
		 << theTrackIDInsideTheSimHit << endl;
#endif
	}
    }
  
    px  = aStep->GetPreStepPoint()->GetMomentum().x()/GeV;
    py  = aStep->GetPreStepPoint()->GetMomentum().y()/GeV;
    pz  = aStep->GetPreStepPoint()->GetMomentum().z()/GeV;
  
    G4ThreeVector gmd  = aStep->GetPreStepPoint()->GetMomentumDirection();
    // convert it to local frame
    G4ThreeVector lmd = ((G4TouchableHistory *)(aStep->GetPreStepPoint()->GetTouchable()))->GetHistory()
			->GetTopTransform().TransformAxis(gmd);
    Local3DPoint lnmd = toOrcaRef(ConvertToLocal3DPoint(lmd),v);
    float theThetaAtEntry = lnmd.theta();
    float thePhiAtEntry = lnmd.phi();
  
    mySimHit = new UpdatablePSimHit(theEntryPoint,theExitPoint,thePabs,theTof,
				    theEnergyLoss,theParticleType,theDetUnitId,
				    theTrackIDInsideTheSimHit,theThetaAtEntry,thePhiAtEntry,
				    theG4ProcessTypeEnumerator->processId(theTrack->GetCreatorProcess()));  
#ifdef DEBUG
    cout << " Created PSimHit: " << pname << " " << mySimHit->detUnitId() << " " << mySimHit->trackId()
	 << " " << mySimHit->energyLoss() << " " << mySimHit->entryPoint() 
	 << " " << mySimHit->exitPoint() << endl;
#endif
    lastId = theDetUnitId;
    lastTrack = theTrackID;
    oldVolume = v;
}

void TkAccumulatingSensitiveDetector::updateHit(G4Step * aStep)
{
    G4VPhysicalVolume * v = aStep->GetPreStepPoint()->GetPhysicalVolume();
    Local3DPoint theExitPoint = toOrcaRef(SensitiveDetector::FinalStepPosition(aStep,LocalCoordinates),v); 
    //
    // This allows to send he skipEvent if it is outside!
    //
    checkExitPoint(theExitPoint);
    float theEnergyLoss = aStep->GetTotalEnergyDeposit()/GeV;
    mySimHit->setExitPoint(theExitPoint);
#ifdef DEBUG
    cout << " Before : " << mySimHit->energyLoss() << endl;
#endif
    mySimHit->addEnergyLoss(theEnergyLoss);
    globalExitPoint = SensitiveDetector::FinalStepPosition(aStep,WorldCoordinates);
#ifdef DEBUG
    cout << " Updating: new exitpoint " << pname << " " << theExitPoint 
	 << " new energy loss " << theEnergyLoss << endl;
    cout << " Updated PSimHit: " << mySimHit->detUnitId() << " " << mySimHit->trackId()
	 << " " << mySimHit->energyLoss() << " " << mySimHit->entryPoint() 
	 << " " << mySimHit->exitPoint() << endl;
#endif
}

bool TkAccumulatingSensitiveDetector::newHit(G4Step * aStep)
{
    if (neverAccumulate == true) return true;
    G4Track * theTrack = aStep->GetTrack(); 
    uint32_t theDetUnitId = setDetUnitId(aStep);
    unsigned int theTrackID = theTrack->GetTrackID();
#ifdef DEBUG
    cout << " OLD (d,t) = (" << lastId << "," << lastTrack 
	 << "), new = (" << theDetUnitId << "," << theTrackID << ") return "
	 << ((theTrackID == lastTrack) && (lastId == theDetUnitId)) << endl;
#endif
    if ((mySimHit != 0) && (theTrackID == lastTrack) && (lastId == theDetUnitId) && closeHit(aStep))
	return false;
    return true;
}

bool TkAccumulatingSensitiveDetector::closeHit(G4Step * aStep)
{
    if (mySimHit == 0) return false; 
    const float tolerance = 0.05 * mm; // 50 micron are allowed between the exit 
    // point of the current hit and the entry point of the new hit
    G4VPhysicalVolume * v = aStep->GetPreStepPoint()->GetPhysicalVolume();
    Local3DPoint theEntryPoint = toOrcaRef(SensitiveDetector::InitialStepPosition(aStep,LocalCoordinates),v);  
#ifdef DEBUG
    cout << " closeHit: distance = " << (mySimHit->exitPoint()-theEntryPoint).mag() << endl;
#endif
    if ((mySimHit->exitPoint()-theEntryPoint).mag()<tolerance) return true;
    return false;
}

void TkAccumulatingSensitiveDetector::EndOfEvent(G4HCofThisEvent *)
{
#ifdef DEBUG
    cout << " Saving the last hit in a ROU " << myName << endl;
#endif
    if (mySimHit == 0) return;
    sendHit();
    // FAKE FOR THE MOMENT WAITING FOR SIGNALS   
    /*
    BeginOfEvent* theBegin= new BeginOfEvent(0);
    upDate(theBegin);
    delete theBegin;
    */
}

void TkAccumulatingSensitiveDetector::update(const BeginOfEvent * i)
{
    clearHits();
    eventno = (*i)()->GetEventID();
    mySimHit = 0;
}

void TkAccumulatingSensitiveDetector::clearHits()
{
    slaveLowTof->Initialize();
    slaveHighTof->Initialize();
}

void TkAccumulatingSensitiveDetector::checkExitPoint(Local3DPoint p)
{
    double z = p.z();
    if (std::abs(z)<0.3*mm) return;
    bool sendExc = false;
    //static SimpleConfigurable<bool> sendExc(false,"TrackerSim:ThrowOnBadHits");
    cout << " ************ Hit outside the detector ; Local z " << z 
	 << "; skipping event = " << sendExc << endl;
    if (sendExc == true)
    {
	G4EventManager::GetEventManager()->AbortCurrentEvent();
	G4EventManager::GetEventManager()->GetNonconstCurrentEvent()->SetEventAborted();
    }
}

void TkAccumulatingSensitiveDetector::fillHits(edm::PSimHitContainer& c, std::string n){
  //
  // do it once for low, once for High
  //

  if (slaveLowTof->name() == n)  c=slaveLowTof->hits();
  if (slaveHighTof->name() == n) c=slaveHighTof->hits();

}

std::vector<string> TkAccumulatingSensitiveDetector::getNames(){
  std::vector<string> temp;
  temp.push_back(slaveLowTof->name());
  temp.push_back(slaveHighTof->name());
  return temp;
}



