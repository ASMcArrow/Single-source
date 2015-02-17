#ifndef GEMPRIMARYGENERATORACTION_H
#define GEMPRIMARYGENERATORACTION_H

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4SingleParticleSource.hh"

class G4Event;

class GEMPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{

public:
    GEMPrimaryGeneratorAction();
    ~GEMPrimaryGeneratorAction();

    void GeneratePrimaries(G4Event* event);

private:
    G4SingleParticleSource* CircleSource;
};

#endif


