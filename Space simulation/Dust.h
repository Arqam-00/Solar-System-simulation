#ifndef DUST_H
#define DUST_H

#include "CelestialBody.h"
#include "raylib.h"
#include "raymath.h"

class Dust : public CelestialBody {
private:
    float Lifetime;
    float FadeSpeed;

public:
    Dust(String n, Vector3 Pos, Vector3 Vel, float Mass, float Radius, Color Body_Color)
        : CelestialBody(n, Pos, Vel, Mass, Radius, Body_Color)
    {
        Max_Trail_Length = 100;
        Lifetime = 3.0f; 
        FadeSpeed = 1.0f;
    }

    void Update_Position(float Delta_Time) override{
        Lifetime -= Delta_Time;
        if (Lifetime < 0) Lifetime = 0;

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
        if (Lifetime <= 0) return;

        float alpha = Lifetime / 3.0f;
        Color faded_color = { Body_Color.r, Body_Color.g, Body_Color.b, (unsigned char)(alpha * 255) };
        DrawSphere(Get_Position(), Get_Radius(), faded_color);
    }

    bool IsDead() const {
        return Lifetime <= 0.0f;
    }
};

#endif
