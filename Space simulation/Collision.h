#ifndef COLLISION_H
#define COLLISION_H

#include "CelestialBody.h"
#include "raymath.h"
#include <cmath>

class Collision
{
public:
    void Handle_Collision(CelestialBody& A, CelestialBody& B);
};
#endif
