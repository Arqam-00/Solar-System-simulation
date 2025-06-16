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
    Dust(String n, Vector3 Pos, Vector3 Vel, float Mass, float Radius, Color Body_Color);

    void Update_Position(float Delta_Time) override;

    void Draw_Body() const override;

    bool CheckDelete() override;
};

#endif
