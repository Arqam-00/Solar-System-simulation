#ifndef MOON_H
#define MOON_H

#include "CelestialBody.h"

class Moon : public CelestialBody {
public:
    Moon() :CelestialBody() {
        Max_Trail_Length = 550;
    }
    Moon(String n, Vector3 Pos, Vector3 Vel, float Mass, float Radius, Color clr)
        : CelestialBody(n, Pos, Vel, Mass, Radius, clr) {
        Max_Trail_Length = 550;
    }
    friend class Planet;
    void Draw_Body() const override {
        DrawSphere(Pos, Radius, Body_Color);
    }
    ~Moon() {}
};

#endif
