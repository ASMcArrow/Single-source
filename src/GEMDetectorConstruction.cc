#include "GEMDetectorConstruction.hh"
#include "GEMDetectorSD.hh"

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4VisAttributes.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4VSolid.hh"
#include "G4UniformMagField.hh"
#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4Navigator.hh"
#include "G4SDManager.hh"

using namespace CLHEP;

G4VPhysicalVolume* GEMDetectorConstruction::Construct()
{
    this->InitializeMaterials();

    G4VisAttributes* visAttributes = new G4VisAttributes;
    visAttributes->SetForceWireframe(true);

    // World
    G4Box* world = new G4Box("World", 4.0*m, 4.0*m, 4.0*m);
    G4LogicalVolume *worldLogic = new G4LogicalVolume(world, MaterialMap["Air"],"WorldLogic");
    G4VPhysicalVolume *worldPhys = new G4PVPlacement(0, G4ThreeVector(), worldLogic, "WorldPhys", 0, false, 0);
    worldLogic->SetVisAttributes(visAttributes);

    // ICBM
    G4Box* mylarStrip = new G4Box("MylarStripSheet", 1.25*mm, 1.5*cm, 0.0125*mm);
    MylarStripLogic = new G4LogicalVolume(mylarStrip, MaterialMap["Mylar"], "MylarStripLogic");

    for( unsigned int i = 0; i < 6; i++ )
    {
        G4ThreeVector offset(-5*1.5*mm -5*1.25*mm + (2*1.25*mm + 3*mm)*i, 0, -256.0*cm);
        G4VPhysicalVolume* mylarStripPhys = new G4PVPlacement(0, offset, MylarStripLogic, "MylarStripPhys", worldLogic, false, i);
    }

    // Scanning Magnet
    G4Box* magnet = new G4Box("Magnet", 2.54*cm, 2.54*cm, 25*cm);
    MagnetLogic = new G4LogicalVolume(magnet, MaterialMap["Air"], "MagnetLogic");
    G4VPhysicalVolume* magnetPhys = new G4PVPlacement(0, G4ThreeVector(0, 0, -230*cm), MagnetLogic, "MagnetPhys", worldLogic, false, 0);
    /*G4Tubs* magnet = new G4Tubs("Magnet", 0*cm, 2.54*cm, 25*cm, 0, CLHEP::twopi);
    MagnetLogic = new G4LogicalVolume(magnet, MaterialMap["Air"], "MagnetLogic");
    G4VPhysicalVolume* magnetPhys = new G4PVPlacement(0, G4ThreeVector(0, 0, -250*cm), MagnetLogic, "MagnetPhys", worldLogic, false, 0);*/

    // PDM Detector
    G4Box* mylarSheet = new G4Box("MylarSheet", 12.5*cm, 12.5*cm, 0.0125*mm);
    G4LogicalVolume* mylarLogic = new G4LogicalVolume(mylarSheet, MaterialMap["Mylar"], "MylarLogic");

    for( unsigned int i = 0; i < 9; i++ )
    {
        G4ThreeVector offset(0, 0, -118*cm+0.0125*mm+((0.0125*mm*2+1.6*mm)*i));
        G4VPhysicalVolume* mylarPhys = new G4PVPlacement(0, offset, mylarLogic, "MylarPhys", worldLogic, false, 0);
    }

    // Aperture
    /*G4Tubs* aperture = new G4Tubs("Aperture", 2.5*cm, 50*cm, 3.25*cm, 0, CLHEP::twopi);
    G4LogicalVolume* apertureLogic = new G4LogicalVolume(aperture, MaterialMap["Brass"], "ApertureLogic");
    G4VPhysicalVolume* aperturePhys = new G4PVPlacement(0, G4ThreeVector(0, 0, -30*cm-3.25*cm), apertureLogic, "AperturePhys", worldLogic, false, 0);*/

    // Phantom
    //    G4Box* phantom = new G4Box("Phantom", 15*cm, 15*cm, 11*cm);
    //    PhantomLogic = new G4LogicalVolume(phantom, MaterialMap["Water"], "PhantomLogic");
    //    G4VPhysicalVolume* phantomPhys = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), PhantomLogic, "PhantomPhys", worldLogic, false, 0);

    return worldPhys;
}

GEMDetectorConstruction::~GEMDetectorConstruction()
{
    std::map<std::string, G4Material*>::iterator i;
    for (i = MaterialMap.begin(); i != MaterialMap.end(); i++)
    {
        delete i->second;
        MaterialMap.erase(i);
    }
}

void GEMDetectorConstruction::ConstructSDandField()
{
    G4SDManager* sDman = G4SDManager::GetSDMpointer();

    GEMDetectorSD* ICBMDetector = new GEMDetectorSD("ICBMDetector", "ICBMHitsCollection");
    sDman->AddNewDetector(ICBMDetector);
    MylarStripLogic->SetSensitiveDetector(ICBMDetector);
}


void GEMDetectorConstruction::InitializeMaterials()
{
    G4NistManager* nistManager = G4NistManager::Instance();

    G4Element* H = nistManager->FindOrBuildElement(1);
    G4Element* C = nistManager->FindOrBuildElement(6);
    G4Element* O = nistManager->FindOrBuildElement(8);
    G4Element* N = nistManager->FindOrBuildElement(7);
    G4Element* Mg = nistManager->FindOrBuildElement(12);
    G4Element* P = nistManager->FindOrBuildElement(15);
    G4Element* S = nistManager->FindOrBuildElement(16);
    G4Element* Ca = nistManager->FindOrBuildElement(20);
    G4Element* Zn = nistManager->FindOrBuildElement(30);
    G4Element* Na = nistManager->FindOrBuildElement(11);
    G4Element* K = nistManager->FindOrBuildElement(19);
    G4Element* Cl = nistManager->FindOrBuildElement(17);
    G4Element* Fe = nistManager->FindOrBuildElement(26);
    G4Element* Si = nistManager->FindOrBuildElement(14);
    G4Element* Cr = nistManager->FindOrBuildElement(24);
    G4Element* Mn = nistManager->FindOrBuildElement(12);
    G4Element* Ni = nistManager->FindOrBuildElement(28);
    G4Element* Cu = nistManager->FindOrBuildElement(29);
    G4Element* Pb = nistManager->FindOrBuildElement(82);

    G4Material* Air = nistManager->FindOrBuildMaterial("G4_AIR");
    MaterialMap["Air"] = Air;

    G4Material* Water = nistManager->FindOrBuildMaterial("G4_WATER");
    MaterialMap["Water"] = Water;

    G4Material* Mylar = nistManager->FindOrBuildMaterial("G4_MYLAR");
    MaterialMap["Mylar"] = Mylar;

    G4Material* Brass = new G4Material("Brass", 8.50*g/cm3, 4);
    Brass->AddElement(Zn, 0.354);
    Brass->AddElement(Cu, 0.6175);
    Brass->AddElement(Pb, 0.025);
    Brass->AddElement(Fe, 0.0035);
    MaterialMap["Brass"] = Brass;

    G4Material* PMMA = new G4Material("PMMA", 1.17*g/cm3, 3);
    PMMA -> AddElement(H, 8);
    PMMA -> AddElement(C, 5);
    PMMA -> AddElement(O, 2);
    MaterialMap["PMMA"] = PMMA;

    G4Material* StainlessSteel = new G4Material("StainlessSteel", 8.06*g/cm3, 6);
    StainlessSteel->AddElement(C, 0.001);
    StainlessSteel->AddElement(Si, 0.007);
    StainlessSteel->AddElement(Cr, 0.18);
    StainlessSteel->AddElement(Mn, 0.01);
    StainlessSteel->AddElement(Fe, 0.712);
    StainlessSteel->AddElement(Ni, 0.09);
    MaterialMap["StainlessSteel"] = StainlessSteel;
}


