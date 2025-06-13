#ifndef METEOR_H
#define METEOR_H

#include "CelestialBody.h"
#include "raylib.h"
#include "raymath.h"
#include <cmath>

class Meteor : public CelestialBody {
private:
    float RotationSpeed;
    Vector3 RotationAxis;
    float ScaleFactor;

public:
    Meteor(String n, Vector3 Pos, Vector3 Vel, float Mass, float Radius, Color Body_Color)
        : CelestialBody(n, Pos, Vel, Mass, Radius, Body_Color)
    {
        RotationSpeed = GetRandomValue(20, 100) / 100.0f;
        RotationAxis = Vector3Normalize({ (float)GetRandomValue(-100, 100) / 100.0f,
                                          (float)GetRandomValue(-100, 100) / 100.0f,
                                          (float)GetRandomValue(-100, 100) / 100.0f });
        ScaleFactor = GetRandomValue(50, 150) / 100.0f;
    }

    void Draw_Body() const {
        DrawSphereEx(Pos, Radius * ScaleFactor, 12, 12, GetColor());
    }

    Color GetColor() const { return Body_Color; }
};

#endif
