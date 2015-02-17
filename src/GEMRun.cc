#include "GEMRun.hh"
#include "G4SDManager.hh"

GEMRun::GEMRun(const G4String detectorName1, const G4String detectorName2, G4bool verbose) : G4Run()
{
    G4SDManager* SDman = G4SDManager::GetSDMpointer();
    G4VSensitiveDetector* detector1 = SDman->FindSensitiveDetector(detectorName1);
    G4VSensitiveDetector* detector2 = SDman->FindSensitiveDetector(detectorName2);

    CollName1 = detector1->GetCollectionName(0);
    CollName2 = detector2->GetCollectionName(0);

    CollectionID1 = SDman->GetCollectionID(CollName1);
    CollectionID2 = SDman->GetCollectionID(CollName2);

    Verbose = verbose;
}

GEMRun::~GEMRun()
{
    HitVector1.clear();
    HitVector2.clear();
}

void GEMRun::RecordEvent(const G4Event* aEvent)
{
    numberOfEvent++;

    G4HCofThisEvent* HCE = aEvent->GetHCofThisEvent();
    if(HCE!=NULL)
    {
        GEMDetectorHitsCollection* HC1 = (GEMDetectorHitsCollection*)(HCE -> GetHC(CollectionID1));
        if(HC1!=NULL)
        {
            if (Verbose) G4cout << CollectionID1 << G4endl;
            this->AddHitToVector(HC1, &HitVector1);
        }

        GEMDetectorHitsCollection* HC2 = (GEMDetectorHitsCollection*)(HCE -> GetHC(CollectionID2));
        if(HC2!=NULL)
        {
            if (Verbose) G4cout << CollectionID2 << G4endl;
            this->AddHitToVector(HC2, &HitVector2);
        }


//        GEMDetectorHitsCollection* HC3 = (GEMDetectorHitsCollection*)(HCE -> GetHC(CollectionID3));
//        if(HC3!=NULL)
//        {
//            if (Verbose) G4cout << CollectionID3 << G4endl;
//            this->AddHitToVector(HC3, &HitVector3);
//        }
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

//    if (HC->entries() != 0)
//    {
//        if(Verbose)
//        {
//            for (G4int l = 0; l < vector->size(); l++)
//            {
//                G4cout << "HitsVector Recorded: " << "l =" << l << " Energy deposition is " << vector[l]->GetEdep()
//                       <<  " Position is " << vector[l]->GetPos()[0] << G4endl;
//            }
//        }
//    }
}

void GEMRun::Merge(const G4Run * aRun)
{
    const GEMRun *localRun = static_cast<const GEMRun*>(aRun);
    for (G4int i = 0; i < localRun -> HitVector1.size(); i++)
        HitVector1.push_back(localRun -> HitVector1[i]);
    for (G4int j = 0; j < localRun -> HitVector2.size(); j++)
        HitVector2.push_back(localRun -> HitVector2[j]);
//    for (G4int k = 0; k < localRun -> HitVector3.size(); k++)
//        HitVector3.push_back(localRun -> HitVector3[k]);

    G4Run::Merge(aRun);
}


