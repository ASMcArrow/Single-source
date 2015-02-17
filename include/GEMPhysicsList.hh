#ifndef GEMPHYSICSLIST_H
#define GEMPHYSICSLIST_H

#include "G4VModularPhysicsList.hh"
#include "globals.hh"

class GEMPhysicsList: public G4VModularPhysicsList
{
public:

  GEMPhysicsList();
  virtual ~GEMPhysicsList();

  void SetCuts();
  void AddParallelScoring();
  void ConstructProcess();

private:
  G4double cutForGamma;
  G4double cutForElectron;
  G4double cutForPositron;
};

#endif
