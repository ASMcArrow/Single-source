#ifndef GEMRUNACTION_H
#define GEMRUNACTION_H

#include "GEMRun.hh"

#include "G4UserRunAction.hh"
#include "globals.hh"
#include <vector>

class GEMRunAction : public G4UserRunAction
{
public:

    GEMRunAction(const G4String detectorName1, const G4String detectorName2);
    virtual ~GEMRunAction();

    virtual G4Run* GenerateRun();
    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);

private:
    void DumpProfile(G4String type, GEMRun *gemRun);

    G4String DepthDetectorName, ProfileDetectorName1, ProfileDetectorName2;
    G4double Cells1[100][100], Cells2[100][100], Cells3[100][100];
    G4double Depth[100];
    G4int ScanVertical, ScanHorizontal;
    G4int CurrentNHits, PreviousNHits;
};

#endif




