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
        Lifetime = 3.0f; 
        FadeSpeed = 1.0f;
    }

    void Update_Position(float Delta_Time) {
        Lifetime -= Delta_Time;
        if (Lifetime < 0) Lifetime = 0;

        CelestialBody::Update_Position(Delta_Time);
    }

    void Draw_Body() const {
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
