#ifndef MOON_H
#define MOON_H

#include "CelestialBody.h"

class Moon : public CelestialBody {
    String Environment;
    bool trail;
public:
    Moon();
    Moon(String n, Vector3 Pos, Vector3 Vel, float Mass, float Radius, Color clr);
    Moon(String n, Vector3 Pos, Vector3 Vel, float Mass, float Radius, Color clr, String env);
    void Toggle_Trail();
    void Draw_Trail() const override;
    friend class Planet;
    void Draw_Body() const override;
    virtual ~Moon();
};

#endif
