#ifndef GEMACTIONINITIASLIZATION_HH
#define GEMACTIONINITIASLIZATION_HH

#include "G4VUserActionInitialization.hh"
#include <vector>
#include "globals.hh"

class GEMActionInitialization : public G4VUserActionInitialization
{
  public:

    GEMActionInitialization();
    virtual ~GEMActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;

private:
    std::vector<G4String> NameVector;
};

#endif // GEMACTIONINITIASLIZATION_HH
