#ifndef SimG4Core_DummyPhysics_H
#define SimG4Core_DummyPhysics_H

#include "SimG4Core/Physics/interface/PhysicsList.h"
 
#include "FWCore/ParameterSet/interface/ParameterSet.h"

class DummyPhysics : public PhysicsList
{
public:
  DummyPhysics(G4LogicalVolumeToDDLogicalPartMap&, const edm::ParameterSet &);
  virtual ~DummyPhysics();
};
 
#endif
