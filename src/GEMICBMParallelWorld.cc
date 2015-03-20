#include "GEMICBMParallelWorld.hh"
#include "GEMDetectorSD.hh"

#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Box.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4VisAttributes.hh"
#include "G4SDManager.hh"

void GEMICBMParallelWorld::Construct()
{
    G4VPhysicalVolume *ghostWorld = GetWorld();
    G4LogicalVolume *worldLog = ghostWorld->GetLogicalVolume();

    // Profile at zero depth
    G4Box *ghostXStrip = new G4Box("GhostXStrip", 1.25*mm, 2.5*cm, 0.0125*mm);
    GhostXBoxLog = new G4LogicalVolume(ghostXStrip, 0, "GhostXStripLog");
    for( unsigned int i = 0; i < 15; i++ )
    {
        G4ThreeVector offset(-3*mm*7+i*3*mm, 0, -280*cm-8*0.0125*mm-4*1.5*mm+(1.5*mm+2*0.0125*mm)*1);
        new G4PVPlacement(0, offset, GhostXBoxLog, "GhostXBoxPhys", worldLog, false, i);
    }

    G4VisAttributes* visAttributesZero = new G4VisAttributes;
    visAttributesZero->SetColor(1,0,0);
    GhostXBoxLog->SetVisAttributes(visAttributesZero);
}

void GEMICBMParallelWorld::ConstructSD()
{
    G4SDManager* sDman = G4SDManager::GetSDMpointer();

    ICBMDetector = new GEMDetectorSD("ICBMDetector", "ICBMHitsCollection");
    sDman->AddNewDetector(ICBMDetector);
    GhostXBoxLog->SetSensitiveDetector(ICBMDetector);
}
