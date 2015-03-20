#include "GEMRun.hh"
#include "G4SDManager.hh"

extern G4ThreadLocal G4Allocator<GEMDetectorHit>* GEMDetectorHitAllocator;

GEMRun::GEMRun(const std::vector<G4String> namevector, G4bool verbose) : G4Run()
{
    NameVector = namevector;

    G4SDManager* SDman = G4SDManager::GetSDMpointer();

    for (G4int i = 0; i < NameVector.size(); i++)
    {
        G4VSensitiveDetector* detector = SDman->FindSensitiveDetector(NameVector[i]);
        G4String collName = detector->GetCollectionName(0);
        G4int collectionID = SDman->GetCollectionID(collName);
        IDVector.push_back(collectionID);
        HitVectorVector.push_back(std::vector<GEMDetectorHit*> (0));
    }

    Verbose = verbose;
}

GEMRun::~GEMRun()
{
    for (G4int i = 0; i < HitVectorVector.size(); i++)
    {
        for (G4int j = 0; j < HitVectorVector[i].size(); j++)
            HitVectorVector[i].clear();
    }
}

void GEMRun::RecordEvent(const G4Event* aEvent)
{
    numberOfEvent++;

    G4HCofThisEvent* HCE = aEvent->GetHCofThisEvent();
    if(HCE!=NULL)
    {
        for (G4int i = 0; i < IDVector.size(); i++)
        {
            GEMDetectorHitsCollection* HC = (GEMDetectorHitsCollection*)(HCE -> GetHC(IDVector[i]));
            if(HC!=NULL)
            {
                if (Verbose) G4cout << "Collection ID processed is ... " << IDVector[i] << G4endl;
                this->AddHitToVector(HC, &HitVectorVector[i]);
            }
        }
    }
}

void GEMRun::AddHitToVector(GEMDetectorHitsCollection *HC, std::vector<GEMDetectorHit *> *vector)
{
    for (G4int j = 0; j < HC->entries(); j++)
    {
        GEMDetectorHit *hit = (GEMDetectorHit*)(HC->GetHit(j));
        if (Verbose)
        {
            G4cout << "Vector Initial: " << "j = "<< j << " Energy deposition is " << hit->GetEdep()
                   << " Position is" << hit->GetPos()[0] << G4endl;
        }
        GEMDetectorHit *copyHit = new GEMDetectorHit(*hit);
        vector->push_back((GEMDetectorHit*)(copyHit));
    }

    if (HC->entries() != 0)
    {
        if(Verbose)
        {
            for (G4int l = 0; l < vector->size(); l++)
            {
                G4cout << "HitsVector Recorded: " << "l =" << l << " Energy deposition is " << (*vector)[l]->GetEdep()
                       <<  " Position is " << (*vector)[l]->GetPos()[0] << G4endl;
            }
        }
    }
}

void GEMRun::Merge(const G4Run * aRun)
{
    const GEMRun *localRun = static_cast<const GEMRun*>(aRun);
    for (G4int i = 0; i < HitVectorVector.size(); i++)
    {
        for (G4int j = 0; j < localRun -> HitVectorVector[i].size(); j++)
            HitVectorVector[i].push_back(localRun -> HitVectorVector[i][j]);
    }

    G4Run::Merge(aRun);
}


