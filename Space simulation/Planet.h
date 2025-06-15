#ifndef PLANET_H
#define PLANET_H

#include "Moon.h"

class Planet : public CelestialBody
{
private:
    String Environment;
    bool Life;
    String Note;
    Dynamic_array<Moon*> Moons;

public:
    friend class Star;
    Planet(String name, Vector3 p, Vector3 v, float m, float r, Color color,
        String env , bool life, String note);
    Planet(String name, Vector3 p, Vector3 v, float m, float r, Color color);
    void Add_Moon(Moon* moon);
    void Place_Moon_In_Orbit(Moon* moon, float distance_multiplier);
    void Check_Escape(Dynamic_array<CelestialBody*>& free_bodies);
    int Get_Number_Of_Moons();
    Moon* Get_Moon_At(int index);
    virtual ~Planet();
    void CheckDelete(CelestialBody* B);
    void Add_Ring(int num);


};

#endif
