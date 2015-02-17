#include "GEMDetectorHit.hh"

#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

G4ThreadLocal G4Allocator<GEMDetectorHit>* GEMDetectorHitAllocator = 0;

GEMDetectorHit::GEMDetectorHit()
{
    EnergyDeposit = 0;
    Position.set(0,0,0);
}

GEMDetectorHit::~GEMDetectorHit()
{}

GEMDetectorHit::GEMDetectorHit(const GEMDetectorHit &right)
    :G4VHit()
{
   Position = right.Position;
   EnergyDeposit = right.EnergyDeposit;
}

const GEMDetectorHit&
GEMDetectorHit::operator=(const GEMDetectorHit &right)
{
    Position = right.Position;
    EnergyDeposit = right.EnergyDeposit;

    return *this;
}

void GEMDetectorHit::Draw()
{
    G4Circle circle(Position);
    circle.SetScreenDiameter(10.0);
    circle.SetFillStyle (G4Circle::filled);
    G4Colour colour(1.,0.,0.);
    G4VisAttributes attribs(colour);
    circle.SetVisAttributes(attribs);
}

int GEMDetectorHit::operator==(const GEMDetectorHit &right) const
{
    return((Position==right.Position)&&(EnergyDeposit==right.EnergyDeposit));
}


