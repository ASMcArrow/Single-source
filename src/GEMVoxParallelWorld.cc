#include "GEMVoxParallelWorld.hh"
#include "GEMDetectorSD.hh"

#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Box.hh"
#include "G4PVPlacement.hh"
#include "G4Tubs.hh"
#include "G4PVReplica.hh"
#include "G4VisAttributes.hh"
#include "G4SDManager.hh"

void GEMVoxParallelWorld::Construct()
{
    G4VPhysicalVolume *ghostWorld = GetWorld();
    G4LogicalVolume *worldLog = ghostWorld->GetLogicalVolume();

    // Place volumes in the parallel world here
    G4double detHalfDimension = 5.0*cm;
    G4double nDivisions = 100.0;
    G4double halfBoxWidth = (detHalfDimension/nDivisions);

    // Profile at zero depth
    G4Box *ghostDetectorZero = new G4Box("GhostDetectorBoxZero", detHalfDimension, detHalfDimension, 0.25*cm);
    GhostDetectorZeroLog = new G4LogicalVolume(ghostDetectorZero, 0, "GhostDetectorZeroLog");
    new G4PVPlacement(0, G4ThreeVector(0, 0, 0.25*cm - 10.9*cm), GhostDetectorZeroLog, "GhostDetectorZeroPhys", worldLog, 0, 0);

    G4VisAttributes* visAttributesZero = new G4VisAttributes;
    visAttributesZero->SetColor(0,0,1);
    GhostDetectorZeroLog->SetVisAttributes(visAttributesZero);

    // Divide along X coordinate at zero depth
    G4Box* ghostXBoxZero = new G4Box("ghostXBoxZero", halfBoxWidth, detHalfDimension, 0.25*cm);
    G4LogicalVolume *GhostXBoxZeroLog = new G4LogicalVolume(ghostXBoxZero, 0, "GhostXBoxZeroLog");
    new G4PVReplica("GhostXBoxZeroPhys", GhostXBoxZeroLog, GhostDetectorZeroLog, kXAxis, nDivisions, halfBoxWidth*2, 0);

    // Divide along Y coordinate at zero depth
    G4Box* ghostYBoxZero = new G4Box("ghostYBoxZero", halfBoxWidth, halfBoxWidth, 0.25*cm);
    GhostYBoxZeroLog = new G4LogicalVolume(ghostYBoxZero, 0, "GhostYBoxZeroLog");
    new G4PVReplica("GhostYBoxZeroPhys", GhostYBoxZeroLog, GhostXBoxZeroLog, kYAxis, nDivisions, halfBoxWidth*2, 0);

    // Profile at isocenter depth
    G4Box *ghostDetectorIso = new G4Box("GhostDetectorBoxIso", detHalfDimension, detHalfDimension, 0.25*cm);
    GhostDetectorIsoLog = new G4LogicalVolume(ghostDetectorIso, 0, "GhostDetectorIsoLog");
    new G4PVPlacement(0, G4ThreeVector(0, 0, 0*cm), GhostDetectorIsoLog, "GhostDetectorIsoPhys", worldLog, 0, 0);

    G4VisAttributes* visAttributesIso = new G4VisAttributes;
    visAttributesIso->SetColor(0,1,0);
    GhostDetectorZeroLog->SetVisAttributes(visAttributesIso);

    // Divide along X coordinate at isocenter depth
    G4Box* ghostXBoxIso = new G4Box("ghostXBoxIso", halfBoxWidth, detHalfDimension, 0.25*cm);
    G4LogicalVolume *GhostXBoxIsoLog = new G4LogicalVolume(ghostXBoxIso, 0, "GhostXBoxIsoLog");
    new G4PVReplica("GhostXBoxIsoPhys", GhostXBoxIsoLog, GhostDetectorIsoLog, kXAxis, nDivisions, halfBoxWidth*2, 0);

    // Divide along Y coordinate at isocenter depth
    G4Box* ghostYBoxIso = new G4Box("ghostYBoxIso", halfBoxWidth, halfBoxWidth, 0.25*cm);
    GhostYBoxIsoLog = new G4LogicalVolume(ghostYBoxIso, 0, "GhostYBoxIsoLog");
    new G4PVReplica("GhostYBoxIsoPhys", GhostYBoxIsoLog, GhostXBoxIsoLog, kYAxis, nDivisions, halfBoxWidth*2, 0);
}

void GEMVoxParallelWorld::ConstructSD()
{
    G4SDManager* sDman = G4SDManager::GetSDMpointer();

    ZeroDetector = new GEMDetectorSD("ProfileDetectorZero", "ProfileHitsCollectionZero");
    sDman->AddNewDetector(ZeroDetector);
    GhostYBoxZeroLog->SetSensitiveDetector(ZeroDetector);

    IsoDetector = new GEMDetectorSD("ProfileDetectorIso", "ProfileHitsCollectionIso");
    sDman->AddNewDetector(IsoDetector);
    // GhostDetectorIsoLog->SetSensitiveDetector(IsoDetector);
    GhostYBoxIsoLog->SetSensitiveDetector(IsoDetector);
}

