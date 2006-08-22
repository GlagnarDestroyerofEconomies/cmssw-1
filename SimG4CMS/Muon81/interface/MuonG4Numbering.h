#ifndef MuonG4Numbering_h
#define MuonG4Numbering_h

/** \class MuonG4Numbering
 *
 * class to handle the conversion to MuonBaseNumber from tree of 
 * G4 copy numbers extracted from G4VPhysicalVolume;
 * needs DDD compatible format of G4 copy numbers; the format
 * itself may in a later version also be defined by DDD constants
 *
 * in the xml muon constant section one has to define
 * level, super and base constants (eg. 1000,100,1) and
 * the start value of the copy numbers (0 or 1)
 *  
 * \author Arno Straessner, CERN <arno.straessner@cern.ch>
 *
 * Modification:
 *
 */

class G4Step;
class MuonBaseNumber;

class MuonG4Numbering {
 public:

  MuonG4Numbering();
  ~MuonG4Numbering(){};
  
  MuonBaseNumber PhysicalVolumeToBaseNumber(const G4Step* aStep);
  
 private:

  const int getCopyNoLevel(const int);
  const int getCopyNoSuperNo(const int);
  const int getCopyNoBaseNo(const int);
  const bool copyNoRelevant(const int);

  int theLevelPart;
  int theSuperPart;
  int theBasePart;
  int theStartCopyNo;

};

#endif
