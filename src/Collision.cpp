#include "Collision.h"
#include "Meteor.h"
#include "Dust.h"
#include "raymath.h"
#include <iostream>
const float G = 0.1f;

void Collision::Handle_Collision(CelestialBody& A, CelestialBody& B, Dynamic_array<CelestialBody*>& bodies)
{
    if (A.Get_Mass() <= 0.0f || B.Get_Mass() <= 0.0f) return;

    float Distance = Vector3Distance(A.Get_Position(), B.Get_Position());
    float Radius_Sum = A.Get_Radius() + B.Get_Radius();
    if (Distance > Radius_Sum) return;

    CelestialBody* big = &A;
    CelestialBody* small = &B;
    if (B.Get_Mass() > A.Get_Mass()) {
        big = &B;
        small = &A;
    }

    Vector3 Relative_Velocity = Vector3Subtract(big->Get_Velocity(), small->Get_Velocity());
    float Relative_Speed = Vector3Length(Relative_Velocity);
    float reduced_mass = (big->Get_Mass() * small->Get_Mass()) / (big->Get_Mass() + small->Get_Mass());
    float kinetic_energy = 0.5f * reduced_mass * Relative_Speed * Relative_Speed;
    float binding_energy = (3.0f / 5.0f) * G * (big->Get_Mass() * small->Get_Mass()) / Radius_Sum;

    Vector3 impactDirection = Vector3Normalize(Vector3Subtract(small->Get_Position(), big->Get_Position()));

    big->Apply_Collision_Deformation(kinetic_energy);
    small->Apply_Collision_Deformation(kinetic_energy);

    if (kinetic_energy < 0.3f * binding_energy || big->Get_Mass() > small->Get_Mass() * 10)
    {
        Merge(*big, *small, 1.0f, bodies);
    }
    else if (kinetic_energy < 0.7f * binding_energy || big->Get_Mass() < small->Get_Mass() * 2)
    {
        Partial_Merge(*big, *small, bodies);
    }
    else if (kinetic_energy < 1.2f * binding_energy)
    {
        Semi_Elastic(*big, *small, bodies);
    }
    else if (kinetic_energy < 2.0f * binding_energy)
    {
        Glancing(*big, *small);
    }
    else
    {
        Destroy(*big, *small, bodies);
    }
}



void Collision::Merge(CelestialBody& A, CelestialBody& B, float merge_fraction, Dynamic_array<CelestialBody*>& bodies)
{
    cout << "Merge" <<A.Name.C_Str()<< endl;

    float absorbed_mass = B.Get_Mass() * merge_fraction;
    float total_mass = A.Get_Mass() + absorbed_mass;

    float velocity_scaling = B.Get_Mass() / total_mass;
    Vector3 delta_momentum = Vector3Scale(B.Get_Velocity(), absorbed_mass * velocity_scaling);
    Vector3 new_momentum = Vector3Add(Vector3Scale(A.Get_Velocity(), A.Get_Mass()), delta_momentum);
    A.Vel = Vector3Scale(new_momentum, 1.0f / total_mass);

    A.Mass = total_mass;
    A.Radius = pow(pow(A.Radius, 3) + pow(B.Radius, 3) * merge_fraction, 1.0f / 3.0f);

    B.Mass -= absorbed_mass;



    if (B.Mass < 0.05f) {
        B.Mass = 0.0f;
        B.Radius = 0.0f;
    }
    else {
        B.Radius = B.Radius * pow(B.Mass / (B.Mass + absorbed_mass), 1.0f / 3.0f);
    }
    if (A.Get_Mass() < B.Get_Mass()*10) {
        int meteor_count = static_cast<int>((A.Get_Mass()) / 5.0f);
        if (meteor_count > 10) meteor_count = 10;

        for (int i = 0; i < meteor_count; i++)
        {
            Vector3 rand_dir = { GetRandomValue(-10, 10), GetRandomValue(-10, 10), GetRandomValue(-10, 10) };
            rand_dir = Vector3Normalize(rand_dir);
            Vector3 spawn_pos = Vector3Add(B.Get_Position(), Vector3Scale(rand_dir, A.Get_Radius() + 2.0f));
            Vector3 spawn_vel = Vector3Add(B.Get_Velocity(), Vector3Scale(rand_dir, 30.0f));

            bodies.push(new Meteor("Meteor", spawn_pos, spawn_vel, (A.Get_Mass() ) / (meteor_count * 100), 1.5f, LIGHTGRAY));
        }
    }
    cout << ":::::::::::" << B.Name.C_Str() << endl;

}

void Collision::Partial_Merge(CelestialBody& A, CelestialBody& B, Dynamic_array<CelestialBody*>& bodies)
{
    cout << "Partial merge" << endl;
    cout << A.Name.C_Str()<<":::::::::::" << B.Name.C_Str() << endl;

    if (B.Get_Mass() > A.Get_Mass()) {
        swap(A, B);
    }

    float original_mass_A = A.Mass;
    float original_mass_B = B.Mass;
    float mass_loss = A.Mass * 0.055;

    A.Mass -= A.Mass * 0.005f;
    B.Mass -= A.Mass * 0.05f;

    if (B.Mass < 0.0f) B.Mass = 0.0f;
    if (A.Mass < 0.0f) A.Mass = 0.0f;

    A.Radius = A.Radius + pow(A.Mass / original_mass_A, 1.0f / 3.0f);
    B.Radius = B.Radius + pow(B.Mass / original_mass_B, 1.0f / 3.0f);

    Merge(A, B, 0.9f,bodies);
    Vector3 Relative_Velocity = Vector3Subtract(A.Get_Velocity(), B.Get_Velocity());

    Vector3 direction = Vector3Normalize(Vector3Subtract(B.Get_Position(), A.Get_Position()));
    float separation_distance = (A.Get_Radius() + B.Get_Radius()) * 0.5f;  
    float separation_speed = 5.0f;

    A.Pos = Vector3Add(A.Pos, Vector3Scale(direction, -separation_distance));
    B.Pos = Vector3Add(B.Pos, Vector3Scale(direction, separation_distance));

    A.Vel = Vector3Add(A.Vel, Vector3Scale(direction, separation_speed));
    B.Vel = Vector3Add(B.Vel, Vector3Scale(direction, -separation_speed));

    int meteor_count = static_cast<int>(mass_loss / 5.0f);
    if (meteor_count > 10) meteor_count = 10;

    for (int i = 0; i < meteor_count; i++)
    {
        Vector3 rand_dir = { GetRandomValue(-10, 10), GetRandomValue(-10, 10), GetRandomValue(-10, 10) };
        rand_dir = Vector3Normalize(rand_dir);
        Vector3 spawn_pos = Vector3Add(B.Get_Position(), Vector3Scale(rand_dir, A.Get_Radius() + 2.0f));
        Vector3 spawn_vel = Vector3Add(B.Get_Velocity(), Vector3Scale(rand_dir, 20.0f));

        bodies.push(new Meteor("Meteor", spawn_pos, spawn_vel, mass_loss / (meteor_count * 100), 1.5f, LIGHTGRAY));
    }
}

void Collision::Destroy(CelestialBody& A, CelestialBody& B, Dynamic_array<CelestialBody*>& bodies)
{
    cout << "Destroy" << endl;
    cout << A.Name.C_Str() << ":::::::::::" << B.Name.C_Str() << endl;

    A.Mass *= 0.6f;
    B.Mass *= 0.6f;

    A.Radius = pow(A.Radius * A.Radius * A.Radius * 0.6f, 1.0f / 3.0f);
    B.Radius = pow(B.Radius * B.Radius * B.Radius * 0.6f, 1.0f / 3.0f);

    float mass_ratio = B.Mass / (A.Mass + B.Mass);
    Vector3 combined_vel = Vector3Scale(Vector3Add(A.Vel, B.Vel), 0.5f * mass_ratio);
    A.Vel = combined_vel;
    B.Vel = combined_vel;

    if (A.Mass < 0.05f) { A.Mass = 0.0f; A.Radius = 0.0f; }
    if (B.Mass < 0.05f) { B.Mass = 0.0f; B.Radius = 0.0f; }

    float total_mass = A.Mass + B.Mass;
    int dust_count = static_cast<int>(total_mass / 5.0f);
    if (dust_count > 15) dust_count = 15;

    for (int i = 0; i < dust_count; i++)
    {
        Vector3 rand_dir = { GetRandomValue(-10, 10), GetRandomValue(-10, 10), GetRandomValue(-10, 10) };
        rand_dir = Vector3Normalize(rand_dir);
        Vector3 spawn_pos = Vector3Add(A.Get_Position(), Vector3Scale(rand_dir, A.Get_Radius() + 1.0f));
        Vector3 spawn_vel = Vector3Add(A.Get_Velocity(), Vector3Scale(rand_dir, 10.0f));

        bodies.push(new Dust("Dust", spawn_pos, spawn_vel, 0.5f, 0.5f, LIGHTGRAY));
    }
    int meteor_count = static_cast<int>(total_mass / 0.5f);
    if (meteor_count > 10) meteor_count = 10;

    for (int i = 0; i < meteor_count; i++)
    {
        Vector3 rand_dir = { GetRandomValue(-10, 10), GetRandomValue(-10, 10), GetRandomValue(-10, 10) };
        rand_dir = Vector3Normalize(rand_dir);
        Vector3 spawn_pos = Vector3Add(B.Get_Position(), Vector3Scale(rand_dir, A.Get_Radius() + 2.0f));
        Vector3 spawn_vel = Vector3Add(B.Get_Velocity(), Vector3Scale(rand_dir, 10.0f));

        bodies.push(new Meteor("Meteor", spawn_pos, spawn_vel, total_mass / (meteor_count * 100), 1.5f, LIGHTGRAY));
    }
}
void Collision::Semi_Elastic(CelestialBody& A, CelestialBody& B, Dynamic_array<CelestialBody*>& bodies)
{
    cout << "Semi-Elastic" << endl;

    float mass_loss_A = A.Get_Mass() * 0.01f;
    float mass_loss_B = B.Get_Mass() * 0.01f;
    float total_mass_loss = mass_loss_A + mass_loss_B;

    A.Mass -= mass_loss_A;
    B.Mass -= mass_loss_B;

    if (A.Mass < 0.05f) { A.Mass = 0.0f; A.Radius = 0.0f; }
    if (B.Mass < 0.05f) { B.Mass = 0.0f; B.Radius = 0.0f; }

    A.Radius = pow(A.Radius * A.Radius * A.Radius * (A.Mass / (A.Mass + mass_loss_A)), 1.0f / 3.0f);
    B.Radius = pow(B.Radius * B.Radius * B.Radius * (B.Mass / (B.Mass + mass_loss_B)), 1.0f / 3.0f);

    Vector3 normal = Vector3Normalize(Vector3Subtract(B.Get_Position(), A.Get_Position()));
    Vector3 relative = Vector3Subtract(A.Get_Velocity(), B.Get_Velocity());

    float velAlongNormal = Vector3DotProduct(relative, normal);
    if (velAlongNormal > 0) return;

    float e = 0.8f;
    float j = -(1 + e) * velAlongNormal / (1 / A.Get_Mass() + 1 / B.Get_Mass());
    Vector3 impulse = Vector3Scale(normal, j);

    A.Vel = Vector3Add(A.Get_Velocity(), Vector3Scale(impulse, 1 / A.Get_Mass()));
    B.Vel = Vector3Subtract(B.Get_Velocity(), Vector3Scale(impulse, 1 / B.Get_Mass()));

    int meteor_count = static_cast<int>(total_mass_loss / 2.0f);
    if (meteor_count > 10) meteor_count = 10;

    float meteor_mass = total_mass_loss / meteor_count;

    for (int i = 0; i < meteor_count; i++)
    {
        Vector3 rand_dir = Vector3Normalize({ (float)GetRandomValue(-10,10),(float)GetRandomValue(-10,10), (float)GetRandomValue(-10,10) });
        Vector3 spawn_pos = Vector3Add(A.Get_Position(), Vector3Scale(rand_dir, A.Get_Radius() + 1.0f));
        Vector3 spawn_vel = Vector3Add(A.Get_Velocity(), Vector3Scale(rand_dir, 3.0f));

        bodies.push(new Meteor("Meteor", spawn_pos, spawn_vel, meteor_mass, 1.0f, LIGHTGRAY));
    }
}
void Collision::Glancing(CelestialBody& A, CelestialBody& B)
{
    cout << "Glancing" << endl;

    Vector3 collision_axis = Vector3Normalize(Vector3Subtract(B.Get_Position(), A.Get_Position()));
    Vector3 relative = Vector3Subtract(A.Get_Velocity(), B.Get_Velocity());
    float velAlongNormal = Vector3DotProduct(relative, collision_axis);

    if (velAlongNormal > 0) return;

    float e = 0.95f;
    float j = -(1 + e) * velAlongNormal / (1 / A.Get_Mass() + 1 / B.Get_Mass());
    Vector3 impulse = Vector3Scale(collision_axis, j);

    A.Vel = Vector3Add(A.Get_Velocity(), Vector3Scale(impulse, 1 / A.Get_Mass()));
    B.Vel = Vector3Subtract(B.Get_Velocity(), Vector3Scale(impulse, 1 / B.Get_Mass()));
}
