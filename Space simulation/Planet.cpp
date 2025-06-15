#include "Planet.h"


Planet::Planet(String name, Vector3 p, Vector3 v, float m, float r, Color color,String env, bool life)
    : CelestialBody(name, p, v, m, r, color),Environment(env), Life(life), 
    Rotation_Axis({ 0.0f,1.0f,0.0f }),Rotation_Vel(20.0f){} 

Planet::Planet(String name, Vector3 p, Vector3 v, float m, float r, Color color):
        CelestialBody(name,p,v,m,r,color),Environment("Unknown"),  Life (false),
    Rotation_Axis({ 0.0f,1.0f,0.0f }), Rotation_Vel(20.0f) {
}

Planet::Planet(String name, Vector3 p, Vector3 v, float m, float r, Color color, String env, bool life,Vector3 rot_axis,float rot_Vel)
    : CelestialBody(name, p, v, m, r, color), Environment(env), Life(life), Rotation_Axis(rot_axis)
    , Rotation_Vel(rot_Vel) {}

Planet::Planet(String name, Vector3 p, Vector3 v, float m, float r, Color color, Vector3 rot_axis, float rot_Vel) :
    CelestialBody(name, p, v, m, r, color), Environment("Unknown"), Life(false), Rotation_Axis(rot_axis)
    , Rotation_Vel(rot_Vel) {}

void Planet::Add_Moon(Moon* moon)
{
    Moons.push(moon);
}

void Planet::Place_Moon_In_Orbit(Moon* moon, float distance_multiplier)
{
    float desired_distance = this->Get_Radius() * distance_multiplier + moon->Get_Radius();
    Vector3 orbit_axis = Vector3Normalize(Vector3CrossProduct(this->Vel, { 0, 1, 0 }));
    if (Vector3Length(orbit_axis) < 0.001f) orbit_axis = { 0, 0, 1 };
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
            if (dist < (min_separation + moon->Get_Radius() + Moons[i]->Get_Radius()))
            {
                position_ok = false;
                break;
            }
        }

        if (position_ok)
        {
            moon->Pos = test_pos;

            float speed = sqrt(G * (this->Mass + moon->Mass) / desired_distance);
            Vector3 tangent = Vector3Normalize(Vector3CrossProduct(orbit_direction, orbit_axis));
            moon->Vel = Vector3Add(this->Vel, Vector3Scale(tangent, speed));

            this->Add_Moon(moon);
        }
    }
}

void Planet::Check_Escape(Dynamic_array<CelestialBody*>& free_bodies)
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

int Planet::Get_Number_Of_Moons() { return Moons.size(); }
Moon* Planet::Get_Moon_At(int index) { return Moons[index]; }

Planet:: ~Planet() {
    Moons.clear();
}
void Planet::Draw_Body() const {
    if (Textured) {
        float Rotation = GetTime() * Rotation_Vel;
        Matrix transform = MatrixMultiply(
            MatrixRotate(Rotation_Axis, Rotation * DEG2RAD),
            MatrixTranslate(Pos.x, Pos.y, Pos.z)
        );
        DrawModelEx(SphereModel, Pos, Rotation_Axis, Rotation, { 1.0f, 1.0f, 1.0f }, WHITE);
    }
    else {
        DrawSphere(Pos, Radius, Body_Color);
    }

}

void Planet::CheckDelete(CelestialBody* B) {

    for (int i = Moons.size() - 1; i >= 0; i--) {
        if (Moons[i] == B) {
            Moons.delete_at(i);

        }

    }


}
void Planet::Add_Ring(int num) {

    float ring_dis;
    for (int i = 0; i < num; i++) {
        Moon* m = new Moon("RingParticle", { 0,0,0 }, { 0,0,0 }, 0.01f, 0.5f, LIGHTGRAY);
        m->Toggle_Trail();
        ring_dis = GetRandomValue(3.0f, 3.8f);
        this->Place_Moon_In_Orbit(m, 3.5);
    }
}
