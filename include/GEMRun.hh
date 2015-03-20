#ifndef GEMRUN_H
#define GEMRUN_H

#include "G4Run.hh"
#include "G4Event.hh"

#include "GEMDetectorHit.hh"
#include <vector>

class GEMRun : public G4Run
{

public:
    GEMRun(const std::vector<G4String> namevector, G4bool verbose);
    virtual ~GEMRun();

    virtual void RecordEvent(const G4Event*);
    virtual void Merge(const G4Run*);

    inline G4int GetNumberOfHits(G4String detectorName) const
    {
        for (G4int i = 0; i < NameVector.size(); i++)
        {
            if (detectorName == NameVector[i])
                return HitVectorVector[i].size();
        }
    }

    inline GEMDetectorHit* GetHit(G4String detectorName, G4int i)
    {
        for (G4int j = 0; j < NameVector.size(); j++)
        {
            if (detectorName == NameVector[j])
                return HitVectorVector[j][i];
        }
    }

private:
    void AddHitToVector(GEMDetectorHitsCollection* HC, std::vector<GEMDetectorHit *> *vector);

    std::vector<G4int> IDVector;
    std::vector<G4String> NameVector;
    std::vector< std::vector<GEMDetectorHit*> > HitVectorVector;
    G4bool Verbose;
};

#endif // GEMRUN_H


