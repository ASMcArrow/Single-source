#include "GEMRunAction.hh"
#include "GEMActionInitialization.hh"
#include "GEMPrimaryGeneratorAction.hh"

GEMActionInitialization::GEMActionInitialization()
    : G4VUserActionInitialization()
{
    NameVector.push_back("ProfileDetectorZero");
    NameVector.push_back("ProfileDetectorIso");
    NameVector.push_back("ICBMDetector");
}

GEMActionInitialization::~GEMActionInitialization()
{}

void GEMActionInitialization::BuildForMaster() const
{
    SetUserAction(new GEMRunAction(NameVector));
}

void GEMActionInitialization::Build() const
{
    SetUserAction(new GEMPrimaryGeneratorAction);
    SetUserAction(new GEMRunAction(NameVector));
}
