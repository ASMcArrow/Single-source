#ifndef GEMDETECTORSD_H
#define GEMDETECTORSD_H

#include "G4VSensitiveDetector.hh"
#include "GEMDetectorHit.hh"
#include "globals.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class GEMDetectorSD : public G4VSensitiveDetector
{

public:
  GEMDetectorSD(G4String name, G4String collname);
  ~GEMDetectorSD();

  void Initialize(G4HCofThisEvent* ); 
  G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* obsolete);
  void EndOfEvent(G4HCofThisEvent* HCE);
 
private:
  GEMDetectorHitsCollection* HitsCollection;
  G4String SensitiveDetectorName;
};

#endif


