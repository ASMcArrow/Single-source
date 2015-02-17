#ifndef GEMDETECTORHIT_H
#define GEMDETECTORHIT_H

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

class GEMDetectorHit : public G4VHit
{

public:
    GEMDetectorHit();
    ~GEMDetectorHit();
 
    GEMDetectorHit(const GEMDetectorHit &right);
    const GEMDetectorHit& operator = (const GEMDetectorHit &right);
    int operator == (const GEMDetectorHit &right) const;

    inline void *operator new(size_t);
    inline void operator delete(void *aHit);

    void Draw();
 // void Print() {} const;

    void SetEdep(G4double de)
    { EnergyDeposit = de; }

    G4double GetEdep() const
    { return EnergyDeposit; }

    void SetPos(G4ThreeVector xyz)
    { Position = xyz; }

    G4ThreeVector GetPos() const
    { return Position; }

private:
    G4ThreeVector Position; // Position
    G4double EnergyDeposit; // Energy deposit associated with the hit
};

typedef G4THitsCollection<GEMDetectorHit> GEMDetectorHitsCollection;
extern G4ThreadLocal G4Allocator<GEMDetectorHit>* GEMDetectorHitAllocator;

inline void* GEMDetectorHit::operator new(size_t)
{
    if(!GEMDetectorHitAllocator)
        GEMDetectorHitAllocator = new G4Allocator<GEMDetectorHit>;

    return (void *)GEMDetectorHitAllocator->MallocSingle();
}

inline void GEMDetectorHit::operator delete(void *aHit)
{
    GEMDetectorHitAllocator->FreeSingle((GEMDetectorHit*) aHit);
}

#endif


