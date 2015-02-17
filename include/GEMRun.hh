#ifndef GEMRUN_H
#define GEMRUN_H

#include "G4Run.hh"
#include "G4Event.hh"

#include "GEMDetectorHit.hh"

#include <vector>

class GEMRun : public G4Run
{

public:
    GEMRun(const G4String detectorName1, const G4String detectorName2, G4bool verbose);
    virtual ~GEMRun();

    // virtual method from G4Run.
    // The method is overriden in this class for scoring.
    virtual void RecordEvent(const G4Event*);
    virtual void Merge(const G4Run*);

    inline G4int GetNumberOfHits(G4String detectorName) const
    {
        if (detectorName == "ProfileDetectorZero")
            return HitVector1.size();
        else if (detectorName == "ProfileDetectorIso")
            return HitVector2.size();
    }

    inline GEMDetectorHit* GetHit(G4String detectorName, G4int i)
    {
        if (detectorName == "ProfileDetectorZero")
            return HitVector1[i];
        else if (detectorName == "ProfileDetectorIso")
            return HitVector2[i];
    }

private:
    void AddHitToVector(GEMDetectorHitsCollection* HC, std::vector<GEMDetectorHit *> *vector);

    G4String CollName1, CollName2;
    G4int CollectionID1, CollectionID2;
    std::vector<GEMDetectorHit*> HitVector1, HitVector2;
    G4bool Verbose;
};

#endif // GEMRUN_H


