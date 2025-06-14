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
        String env = "Unknown", bool life = false, String note = "nothing", CelestialBody* parent = nullptr)
        : CelestialBody(name, p, v, m, r, color),
        Environment(env), Life(life), Note(note), Parent(parent)
    {
    }

    void Add_Moon(Moon* moon)
    {
        Moons.push(moon);
    }

    void Place_Moon_In_Orbit(Moon* moon, float distance_multiplier)
    {
        float desired_distance = this->Get_Radius() * distance_multiplier + moon->Get_Radius();
        Vector3 orbit_axis = { 0, 1, 0 };
        const float min_separation = 5.0f; 

        bool position_ok = false;

        while (!position_ok)
        {
            float inclination = DEG2RAD * GetRandomValue(-10, 10);
            float phase = DEG2RAD * GetRandomValue(0, 360);

            Vector3 reference = (fabs(orbit_axis.y) < 0.99f) ? Vector3{ 0, 1, 0 } : Vector3{ 1, 0, 0 };
            Vector3 base_direction = Vector3Normalize(Vector3CrossProduct(orbit_axis, reference));

            Matrix phase_rot = MatrixRotate(orbit_axis, phase);
            Vector3 orbit_direction = Vector3Transform(base_direction, phase_rot);

            Vector3 incl_axis = Vector3Normalize(Vector3CrossProduct(orbit_axis, orbit_direction));
            Matrix incl_rot = MatrixRotate(incl_axis, inclination);
            orbit_direction = Vector3Transform(orbit_direction, incl_rot);

            Vector3 test_pos = Vector3Add(this->Pos, Vector3Scale(orbit_direction, desired_distance));

            position_ok = true;
            for (int i = 0; i < Moons.size(); i++)
            {
                float dist = Vector3Distance(test_pos, Moons[i]->Pos);
                if (dist < min_separation + moon->Get_Radius() + Moons[i]->Get_Radius())
                {
                    position_ok = false;
                    break;
                }
            }

            if (position_ok)
            {
                moon->Pos = test_pos;

                float speed = sqrt(G * (this->Mass + moon->Mass) / desired_distance);
                Vector3 tangent = Vector3Normalize(Vector3CrossProduct(orbit_axis, orbit_direction));
                moon->Vel = Vector3Add(this->Vel, Vector3Scale(tangent, speed));

                this->Add_Moon(moon);
            }
        }
    }


    void Stabilize_Orbit(int i)
    {
        if (Moons.size() <= i || i < 0) {
            cout << "Invalid moon index.\n";
            return;
        }

        

        Vector3 orbit_axis = Vector3Normalize(Vector3CrossProduct(Pos, Vel));
        if (Vector3Length(orbit_axis) < 0.0001f)
            orbit_axis = { 0.0f, 1.0f, 0.0f };
        const float desired_distance = Radius * 2.5f + Moons[i]->Radius;

        Vector3 direction = Vector3Normalize(Vector3Subtract(Moons[i]->Pos, Pos));
        Vector3 rotated_direction = Vector3RotateByAxisAngle(direction, orbit_axis, 9);
        Moons[i]->Pos = Vector3Add(Pos, Vector3Scale(rotated_direction, desired_distance));
        Vector3 tangent = Vector3Normalize(Vector3CrossProduct(orbit_axis, rotated_direction));
        float speed = sqrt(G * (Mass + Moons[i]->Get_Mass()) / desired_distance);
        Moons[i]->Vel = Vector3Add(Vel, Vector3Scale(tangent, speed));
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
