#ifndef MOON_H
#define MOON_H

#include "CelestialBody.h"

class Moon : public CelestialBody {
    String Environment;
    bool trail;
public:
    Moon() :CelestialBody(),Environment("unknown"),trail(1) {
        Max_Trail_Length = 550;
    }
    Moon(String n, Vector3 Pos, Vector3 Vel, float Mass, float Radius, Color clr)
        : CelestialBody(n, Pos, Vel, Mass, Radius, clr), Environment("unknown"), trail(1) {
        Max_Trail_Length = 550;
    }
    Moon(String n, Vector3 Pos, Vector3 Vel, float Mass, float Radius, Color clr,String env)
        : CelestialBody(n, Pos, Vel, Mass, Radius, clr), Environment(env), trail(1) {
        Max_Trail_Length = 550;
    }
    void Toggle_Trail() {
        trail = !trail;
    }
    void Draw_Trail() const override {
        if (trail) {
            for (int i = 1; i < Trail.size(); i++) {
                DrawLine3D(Trail[i - 1], Trail[i], Body_Color);
            }
        }
    }
    friend class Planet;
    void Draw_Body() const override {
        DrawSphere(Pos, Radius, Body_Color);
    }
    virtual ~Moon() {}
};

#endif
