#ifndef COLLISION_H
#define COLLISION_H

#include "CelestialBody.h"

class Collision
{
private:
    void Merge(CelestialBody& A, CelestialBody& B, float merge_fraction);
    void Partial_Merge(CelestialBody& A, CelestialBody& B, float mass_loss_fraction);
    void Destroy(CelestialBody& A, CelestialBody& B, float destruction_fraction);
public:
    static void Handle_Collision(CelestialBody& A, CelestialBody& B);
};

#endif
