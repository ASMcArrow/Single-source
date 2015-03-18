#include "GEMRunAction.hh"
#include "GEMRun.hh"
#include "GEMDetectorHit.hh"
#include "GEMDetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4MTRunManager.hh"
#include "G4THitsMap.hh"
#include "G4SDManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4Run.hh"
#include "G4TransportationManager.hh"
#include "G4PropagatorInField.hh"
#include "G4Navigator.hh"
#include "G4UImanager.hh"
#include "DebugUISession.hh"

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#define _USE_MATH_DEFINES

GEMRunAction::GEMRunAction(const G4String detectorName1, const G4String detectorName2, const G4String detectorName3) : G4UserRunAction()
{
    //    G4UImanager* UImanager = G4UImanager::GetUIpointer();
    //    DebugUISession * LoggedSession = new DebugUISession;
    //    UImanager->SetCoutDestination(LoggedSession);

    ProfileDetectorName1 = detectorName1;
    ProfileDetectorName2 = detectorName2;
    ICBMDetectorName = detectorName3;

    for (int i = 0; i < 100; i++)
    {
        for (int j = 0; j < 100; j++)
        {
            Cells1[i][j] = 0;
            Cells2[i][j] = 0;
            Cells3[i][j] = 0;
        }

        Depth[i] = 0;
    }

    for (int i = 0; i < 5; i++)
    {
        Cells4[i] = 0;
    }

    CurrentNHits = 0;
    PreviousNHits = 0;
}

GEMRunAction::~GEMRunAction()
{}

G4Run* GEMRunAction::GenerateRun()
{
    return new GEMRun(ProfileDetectorName1, ProfileDetectorName2, ICBMDetectorName, 0);
}

void GEMRunAction::BeginOfRunAction(const G4Run* aRun)
{
    G4Navigator* navigator = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
    navigator->SetPushVerbosity(false);
    navigator->SetVerboseLevel(0);
}

void GEMRunAction::EndOfRunAction(const G4Run* aRun)
{
    GEMRun *gemRun = (GEMRun*)aRun;

    G4cout << "GEMRunAction: Number of events in this run " << gemRun->GetNumberOfEventToBeProcessed() << G4endl;
    G4cout << "GEMRunAction: Number of hits in this run in ZeroProfile detector " << gemRun->GetNumberOfHits(ICBMDetectorName) << G4endl;

    if(!IsMaster()) return;

    G4int hitNum = gemRun->GetNumberOfHits("ICBMDetector");
    for (G4int i = 0; i < hitNum; i++)
    {
        GEMDetectorHit* hit = (GEMDetectorHit*)(gemRun->GetHit("ICBMDetector", i));

        if(hit != NULL)
        {
            G4int j = hit->GetPos()[0];
            Cells4[j] = Cells4[j]+hit->GetEdep();
        }
    }

    std::ofstream ICBMFile("ICBM.txt");
    for (G4int i = 0; i < 6; i++)
    {
        ICBMFile << -5*1.5*mm -5*1.25*mm + (2*1.25*mm + 3*mm)*i << " " << Cells4[i] << "\n";
    }

    this->DumpProfile("ZeroMT", gemRun);
    this->DumpProfile("IsoMT", gemRun);
}

void GEMRunAction::DumpProfile(G4String type, GEMRun *gemRun)
{
    G4int hitNum;
    G4String detectorName;
    G4String fileName;

    if (type == "ZeroMT")
    {
        hitNum = gemRun->GetNumberOfHits("ProfileDetectorZero");
        G4cout << "Hits in ProfileDetectorZero = " << hitNum << G4endl;
        detectorName = "ProfileDetectorZero";
        fileName = "/media/large2/SingleSource/SingleProfileZeroMT.txt";
    }
    else if (type == "IsoMT")
    {
        hitNum = gemRun->GetNumberOfHits("ProfileDetectorIso");
        G4cout << "Hits in ProfileDetectorIso = " << hitNum << G4endl;
        detectorName = "ProfileDetectorIso";
        fileName = "/media/large2/SingleSource/SingleProfileIsoMT.txt";
    }

    for (G4int i = 0; i < hitNum; i++)
    {
        GEMDetectorHit* hit = (GEMDetectorHit*)(gemRun->GetHit(detectorName, i));

        if(hit != NULL)
        {
            G4int j = hit->GetPos()[0];
            G4int k = hit->GetPos()[1];
            if (type == "ZeroMT")
                Cells1[j][k] = Cells1[j][k]+hit->GetEdep();
            else if (type == "IsoMT")
                Cells2[j][k] = Cells2[j][k]+hit->GetEdep();
        }
    }

    std::ofstream profileFile(fileName);

    G4double horizontal[101], vertical[101];
    for (G4int i = 0; i < 100; i++)
    {
        profileFile << "\n";
        for (G4int j = 0; j < 100; j++)
        {
            if (type == "ZeroMT")
            {
                if ((i == 100)||(j == 100))
                {
                    profileFile << (G4double)(i*10.0)/100.0 << " " << (G4double)(j*10.0/100.0) << " 0 \n";
                }
                else
                    profileFile << i*10.0/100.0 << " " << j*10.0/100.0 << " " << Cells1[i][j] << "\n";

                if (i == 50)
                    horizontal[j] = Cells1[i][j];

                if (j == 50)
                    vertical[i] = Cells1[i][j];
            }

            else if (type == "IsoMT")
            {
                if ((i == 100)||(j == 100))
                {
                    profileFile << (G4double)(i*10.0)/100.0 << " " << (G4double)(j*10.0/100.0) << " 0 \n";
                }
                else
                    profileFile << i*10.0/100.0 << " " << j*10.0/100.0 << " " << Cells2[i][j] << "\n";

                if (i == 50)
                    horizontal[j] = Cells2[i][j];

                if (j == 50)
                    vertical[i] = Cells2[i][j];
            }
        }
    }

    horizontal[100] = 0;
    vertical[100] = 0;

    G4String name1 = "/media/large2/SingleSource/MTSingleProfile_H_"+type+".txt";
    G4String name2 = "/media/large2/SingleSource/MTSingleProfile_V_"+type+".txt";

    std::ofstream profileFileH(name1);
    std::ofstream profileFileV(name2);

    for (G4int box = 0; box <= 100; box++)
    {
        profileFileH << (G4double)(box*10.0/100.0) << " " << horizontal[box] << "\n";
        profileFileV << (G4double)(box*10.0/100.0) << " " << vertical[box] << "\n";
    }
}

