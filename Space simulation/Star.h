#ifndef STAR_H
#define STAR_H

#include "Planet.h"

class Star : public CelestialBody
{
private:
    Dynamic_array<Planet*> Planets;

public:
    Star(String name, Vector3 p, Vector3 v, float m, float r, Color color)
        : CelestialBody(name, p, v, m, r, color)
    {
    }

    void Add_Planet(Planet* planet)
    {
        Planets.push(planet);
    }

    void Stabilize_Orbit(int i)
    {
        cout << " check ::" << i << endl;

        if (Planets.size() <= i || i < 0) {
            cout << "whong index\n";
            return;
        }
        Vector3 dir = Vector3Subtract(Pos, Planets[i]->Get_Position());
        float dist = Vector3Length(dir);
        float speed = sqrt(G * Mass / dist);
        Vector3 tangent = Vector3Normalize(Vector3CrossProduct(dir, { 0, 0, 1 }));
        Vector3 vel = Vector3Scale(tangent, speed);
        Planets[i]->Vel = Vector3Add(Vel, vel);
    }

    void Check_Escape(Dynamic_array<CelestialBody*>& free_bodies)
    {
        for (int i = 0; i < Planets.size(); )
        {
            float dist = Vector3Distance(Pos, Planets[i]->Get_Position());

            float escape_velocity = sqrt((2.0f * G * Mass) / dist);
            Vector3 rel_velocity = Vector3Subtract(Planets[i]->Get_Velocity(), Vel);
            float current_speed = Vector3Length(rel_velocity);

            if (current_speed > escape_velocity*2)
            {
                Planets[i]->Name += "(No longer orbiting)";
                free_bodies.push(Planets[i]);
                Planets.delete_at(i);
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
        for (int i = 0; i < Planets.size(); i++)
        {
            Planets[i]->Draw_Body();
        }
    }
    int Get_Number_Of_Planets() { return Planets.size(); }
    Planet* Get_Planet_At(int index) { return Planets[index]; }
    ~Star() {
        Planets.clear();
    }
};

#endif
