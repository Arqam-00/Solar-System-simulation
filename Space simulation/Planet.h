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
    CelestialBody* Parent;

public:
    friend class Star;
    Planet(String name, Vector3 p, Vector3 v, float m, float r, Color color,
        String env = "Unknown", bool life = false, String note = "", CelestialBody* parent = nullptr)
        : CelestialBody(name, p, v, m, r, color),
        Environment(env), Life(life), Note(note), Parent(parent)
    {
    }

    void Add_Moon(Moon* moon)
    {
        Moons.push(moon);
    }

    void Stabilize_Orbit(int i)
    {
        if (Moons.size() <= i || i < 0) {
            cout << "Invalid moon index.\n";
            return;
        }

        const float desired_distance = Radius + Moons[i]->Radius + 10.0f; // for example, 10 units above planet's surface

        Vector3 direction = Vector3Normalize(Vector3Subtract(Moons[i]->Pos, Pos));
        Moons[i]->Pos = Vector3Add(Pos, Vector3Scale(direction, desired_distance));

        float speed = sqrt(G * Mass / desired_distance);
        Vector3 tangent = Vector3Normalize(Vector3CrossProduct(direction, { 0, 0, 1 }));
        Vector3 velocity = Vector3Add(Vel, Vector3Scale(tangent, speed));
        Moons[i]->Vel = velocity;
    }



    void Check_Escape(Dynamic_array<CelestialBody*>& free_bodies)
    {
        for (int i = 0; i < Moons.size(); )
        {
            float dist = Vector3Distance(Pos, Moons[i]->Get_Position());

            float escape_velocity = sqrt((2.0f * G * Mass) / dist);
            Vector3 rel_velocity = Vector3Subtract(Moons[i]->Get_Velocity(), Vel);
            float current_speed = Vector3Length(rel_velocity);

            if (current_speed > escape_velocity)
            {
                Moons[i]->Name += "(No longer orbiting)";
                free_bodies.push(Moons[i]);
                Moons.delete_at(i);
            }
            else
            {
                i++;
            }
        }
    }



    void Draw_Body() const override
    {
        CelestialBody::Draw_Body();
        for (int i = 0; i < Moons.size(); i++)
        {
            Moons[i]->Draw_Body();
        }
    }

   
    int Get_Number_Of_Moons() { return Moons.size(); }
    Moon* Get_Moon_At(int index) { return Moons[index]; }

    ~Planet() {
        Moons.clear();
    }

};

#endif
