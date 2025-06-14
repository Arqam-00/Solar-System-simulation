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
        Max_Trail_Length = 500;
        RotationSpeed = GetRandomValue(20, 100) / 100.0f;
        RotationAxis = Vector3Normalize({ (float)GetRandomValue(-100, 100) / 100.0f,
                                          (float)GetRandomValue(-100, 100) / 100.0f,
                                          (float)GetRandomValue(-100, 100) / 100.0f });
        ScaleFactor = GetRandomValue(50, 150) / 100.0f;
    }
    virtual void Update_Position(float Delta_Time) {
        Vel = Vector3Add(Vel, Vector3Scale(Acc, Delta_Time));
        Pos = Vector3Add(Pos, Vector3Scale(Vel, Delta_Time));

        Radius = Lerp(Radius, StableRadius, 0.02f);
        DeformationAmount *= 0.98f;

        float rippleEffect = sin(GetTime() * 5.0f) * DeformationAmount * 0.5f;
        Radius += rippleEffect;

        Trail.push(Pos);
        if (Trail.size() > Max_Trail_Length) {
            Trail.delete_at(0);
        }
    }

    void Draw_Body() const override{
        DrawSphereEx(Pos, Radius * ScaleFactor, 12, 12, GetColor());
    }

    Color GetColor() const { return Body_Color; }
};

#endif
