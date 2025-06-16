#ifndef COLLISION_H
#define COLLISION_H

#include "CelestialBody.h"
#include "Dynamic_Array.h"

class Collision
{
public:
    void Handle_Collision(CelestialBody& A, CelestialBody& B, Dynamic_array<CelestialBody*>& bodies);

private:
    void Merge(CelestialBody& A, CelestialBody& B, float merge_fraction, Dynamic_array<CelestialBody*>& bodies);
    void Partial_Merge(CelestialBody& A, CelestialBody& B, Dynamic_array<CelestialBody*>& bodies);
    void Destroy(CelestialBody& A, CelestialBody& B, Dynamic_array<CelestialBody*>& bodies);
    void Semi_Elastic(CelestialBody& A, CelestialBody& B, Dynamic_array<CelestialBody*>& bodies);
    void Glancing(CelestialBody& A, CelestialBody& B);
};

#endif
