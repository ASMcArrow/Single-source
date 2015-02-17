#ifndef DEBUGUISESSION_HH
#define DEBUGUISESSION_HH

#include "G4UIsession.hh"
#include "G4Threading.hh"

class DebugUISession : public G4UIsession
{
public:

    G4int ReceiveG4cout(const G4String& coutString);
    //G4int ReceiveG4cerr(const G4String& cerrString);
};

#endif // DEBUGUISESSION_HH
