#ifndef GEMICBMPARALLELWORLD_HH
#define GEMICBMPARALLELWORLD_HH

#include "GEMDetectorSD.hh"

#include "globals.hh"
#include "G4VUserParallelWorld.hh"
#include "CLHEP/Units/SystemOfUnits.h"

using namespace CLHEP;

class GEMICBMParallelWorld : public G4VUserParallelWorld
{
public:
    GEMICBMParallelWorld(G4String worldName):G4VUserParallelWorld(worldName) {}
    virtual ~GEMICBMParallelWorld()
    {
        if (ICBMDetector != NULL)
            delete ICBMDetector;
    }

    virtual void Construct();
    virtual void ConstructSD();

private:
    G4LogicalVolume *GhostYBoxLog, *GhostXBoxLog;
    G4VSensitiveDetector *ICBMDetector;
};

#endif // GEMICBMPARALLELWORLD_HH

