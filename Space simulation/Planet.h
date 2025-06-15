#ifndef PLANET_H
#define PLANET_H

#include "Moon.h"

class Planet : public CelestialBody
{
private:
    String Environment;
    bool Life;
    Vector3 Rotation_Axis;
    float Rotation_Vel;
    Dynamic_array<Moon*> Moons;

public:
    friend class Star;
    Planet(String name, Vector3 p, Vector3 v, float m, float r, Color color,String env , bool life);
    Planet(String name, Vector3 p, Vector3 v, float m, float r, Color color);
    Planet(String name, Vector3 p, Vector3 v, float m, float r, Color color, String env, bool life,Vector3 rot_axis,float rot_vel);
    Planet(String name, Vector3 p, Vector3 v, float m, float r, Color color, Vector3 rot_axis, float rot_vel);

    void Add_Moon(Moon* moon);
    void Place_Moon_In_Orbit(Moon* moon, float distance_multiplier);
    void Check_Escape(Dynamic_array<CelestialBody*>& free_bodies);
    int Get_Number_Of_Moons();
    Moon* Get_Moon_At(int index);
    void Draw_Body() const override;
    virtual ~Planet();
    void CheckDelete(CelestialBody* B);
    void Add_Ring(int num);


};

#endif
