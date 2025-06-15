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
        Environment(env), Life(life), Note(note), Parent(parent){}

    void Add_Moon(Moon* moon)
    {
        Moons.push(moon);
    }

    void Place_Moon_In_Orbit(Moon* moon, float distance_multiplier)
    {
        float desired_distance = this->Get_Radius() * distance_multiplier + moon->Get_Radius();
        Vector3 orbit_axis = Vector3Normalize(Vector3CrossProduct(this->Vel, { 0, 1, 0 }));
        if (Vector3Length(orbit_axis) < 0.001f) orbit_axis = { 0, 1, 0 };
        float min_separation = moon->Get_Radius() * 2.0f;

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
                if (dist < ( min_separation + moon->Get_Radius() + Moons[i]->Get_Radius()))
                {
                    position_ok = false;
                    break;
                }
            }

            if (position_ok)
            {
                moon->Pos = test_pos;

                float speed = sqrt(G * (this->Mass + moon->Mass) / desired_distance);
                Vector3 tangent = Vector3Normalize(Vector3CrossProduct(orbit_direction,orbit_axis));
                moon->Vel = Vector3Add(this->Vel, Vector3Scale(tangent, speed));

                this->Add_Moon(moon);
            }
        }
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


   
    int Get_Number_Of_Moons() { return Moons.size(); }
    Moon* Get_Moon_At(int index) { return Moons[index]; }

    ~Planet() {
        Moons.clear();
    }
    void Orbital_Stabilizer(float delta_time)
    {

    }



    bool CheckDelete() override {

        for (int i = 0; i < Moons.size(); i++) {
            if (Moons[i] == nullptr) {
                Moons.delete_at(i);

            }
            else if (Moons[i]->CheckDelete()) {
                Moons.delete_at(i);
            }
        }
        return CelestialBody::CheckDelete();

    }
    void Orbit_Stabilizer(float delta_time)
    {
        const float correction_strength = 0.08f;
        const float min_separation_factor = 1.5f;

        for (int i = 0; i < Moons.size(); i++)
        {
            Moon* moon = Moons[i];

            Vector3 direction = Vector3Normalize(Vector3Subtract(moon->Pos, Pos));
            float current_distance = Vector3Distance(moon->Pos, Pos);
            float desired_distance = Radius * 2.5f + moon->Get_Radius() + (i * 8.0f);

            float distance_error = (current_distance - desired_distance);

            // Apply only a radial correction to keep the moon at the correct orbital distance
            float radial_accel_magnitude = distance_error * correction_strength;
            Vector3 radial_accel = Vector3Scale(direction, -radial_accel_magnitude);
            moon->Acc = Vector3Add(moon->Acc, radial_accel);

            // Prevent moon collisions by ensuring separation between moons
            for (int j = 0; j < Moons.size(); j++)
            {
                if (i == j) continue;

                float moon_dist = Vector3Distance(moon->Pos, Moons[j]->Pos);
                float min_separation = min_separation_factor * (moon->Get_Radius() + Moons[j]->Get_Radius());

                if (moon_dist < min_separation)
                {
                    Vector3 repulsion = Vector3Normalize(Vector3Subtract(moon->Pos, Moons[j]->Pos));
                    moon->Acc = Vector3Add(moon->Acc, Vector3Scale(repulsion, correction_strength));
                }
            }

            // Adjust tangential velocity for orbital stability
            Vector3 rel_vel = Vector3Subtract(moon->Vel, Vel);
            Vector3 orbit_axis = Vector3CrossProduct(direction, rel_vel);
            if (Vector3Length(orbit_axis) < 0.001f)
                orbit_axis = { 0, 1, 0 };

            Vector3 tangent = Vector3Normalize(Vector3CrossProduct(orbit_axis, direction));

            float current_tangential_speed = Vector3DotProduct(rel_vel, tangent);
            float target_speed = sqrt(G * (Mass + moon->Mass) / current_distance);

            float tangential_error = target_speed - current_tangential_speed;
            float tangential_accel_magnitude = tangential_error * correction_strength;
            Vector3 tangential_accel = Vector3Scale(tangent, tangential_accel_magnitude);
            moon->Acc = Vector3Add(moon->Acc, tangential_accel);
        }
    }

};

#endif
