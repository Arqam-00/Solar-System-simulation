#ifndef COLLISION_H
#define COLLISION_H

#include "CelestialBody.h"

class Collision
{

private:
    void Merge(CelestialBody& A, CelestialBody& B);
    void Partial_Merge(CelestialBody& A, CelestialBody& B, float absorb_ratio);
    void Elastic_Bounce(CelestialBody& A, CelestialBody& B);
public:
    void Handle_Collision(CelestialBody& A, CelestialBody& B);

};

#endif
