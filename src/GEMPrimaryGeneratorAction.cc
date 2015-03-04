#include "GEMPrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4SingleParticleSource.hh"
#include "G4SPSAngDistribution.hh"
#include "G4Proton.hh"

using namespace CLHEP;

GEMPrimaryGeneratorAction::GEMPrimaryGeneratorAction()
{
    CircleSource = new G4SingleParticleSource();
    CircleSource->SetParticleDefinition(G4Proton::Definition());

    G4SPSEneDistribution* energy = CircleSource->GetEneDist();
    energy->SetEnergyDisType("Gauss");
    energy->SetBeamSigmaInE(1 *MeV);
    energy->SetMonoEnergy(152.3 *MeV);

    G4SPSPosDistribution* position = CircleSource->GetPosDist();
    position->SetPosDisType("Beam");
    position->SetCentreCoords(G4ThreeVector(0.0*m, 0.0*m, -0.1*cm/*-256.0*cm*/));
    position->SetBeamSigmaInR(6*mm);

    G4SPSAngDistribution* angular = CircleSource->GetAngDist();
    angular->SetAngDistType("beam1d");
    angular->SetParticleMomentumDirection(G4ThreeVector(0.0,0.0,1.0));
    angular->DefineAngRefAxes("angref1", G4ThreeVector(-1.0,0.0,0.0));
    angular->SetBeamSigmaInAngR(0.000);

    CircleSource->SetNumberOfParticles(1);
}

GEMPrimaryGeneratorAction::~GEMPrimaryGeneratorAction()
{
    delete CircleSource;
}

void GEMPrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
{
    CircleSource->GeneratePrimaryVertex(event);
    std::ofstream file;
    file.open("PrimaryVertex_X.txt", std::ios_base::app | std::ios_base::out);
    file << event->GetPrimaryVertex()->GetPosition().getX() << "\n";

    file.open("PrimaryVertex_X_cut.txt", std::ios_base::app | std::ios_base::out);
    if ((event->GetPrimaryVertex()->GetPosition().getY() < 1)&&(event->GetPrimaryVertex()->GetPosition().getY() > (-1)))
        //file << event->GetPrimaryVertex()->GetPosition().getX() << "\n";
        G4cout << "Hit!" << G4endl;
}
