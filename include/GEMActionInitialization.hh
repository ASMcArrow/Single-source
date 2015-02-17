#ifndef GEMACTIONINITIASLIZATION_HH
#define GEMACTIONINITIASLIZATION_HH

#include "G4VUserActionInitialization.hh"

class GEMActionInitialization : public G4VUserActionInitialization
{
  public:

    GEMActionInitialization();
    virtual ~GEMActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;
};

#endif // GEMACTIONINITIASLIZATION_HH
