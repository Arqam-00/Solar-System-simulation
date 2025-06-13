#include "Collision.h"
#include "raymath.h"

const float G = 0.1f;

void Collision::Handle_Collision(CelestialBody& A, CelestialBody& B)
{
    if (A.Mass <= 0.0f || B.Mass <= 0.0f)
        return;

    float Distance = Vector3Distance(A.Pos, B.Pos);
    float Radius_Sum = A.Radius + B.Radius;

    if (Distance <= Radius_Sum)
    {
        Vector3 Relative_Velocity = Vector3Subtract(A.Vel, B.Vel);
        float Relative_Speed = Vector3Length(Relative_Velocity);
        Collision C;
        float reduced_mass = (A.Mass * B.Mass) / (A.Mass + B.Mass);
        float kinetic_energy = 0.5f * reduced_mass * Relative_Speed * Relative_Speed;
        float binding_energy = (3.0f / 5.0f) * G * (A.Mass * B.Mass) / Radius_Sum;

        if (kinetic_energy < 0.5f * binding_energy)
        {
            C.Merge(A, B, 1.0f);
        }
        else if (kinetic_energy < 2.0f * binding_energy)
        {
            C.Partial_Merge(A, B, 0.2f);
        }
        else
        {
            C.Destroy(A, B, 0.4f);
        }
    }
}

void Collision::Merge(CelestialBody& A, CelestialBody& B, float merge_fraction)
{
    float absorbed_mass = B.Mass * merge_fraction;
    float total_mass = A.Mass + absorbed_mass;

    A.Vel = Vector3Scale(Vector3Add(Vector3Scale(A.Vel, A.Mass), Vector3Scale(B.Vel, absorbed_mass)), 1.0f / total_mass);
    A.Pos = Vector3Scale(Vector3Add(Vector3Scale(A.Pos, A.Mass), Vector3Scale(B.Pos, absorbed_mass)), 1.0f / total_mass);
    A.Mass = total_mass;
    A.Radius = pow(A.Radius * A.Radius * A.Radius + B.Radius * B.Radius * B.Radius * merge_fraction, 1.0f / 3.0f);

    B.Mass -= absorbed_mass;
    B.Radius = pow(B.Radius * B.Radius * B.Radius * (1.0f - merge_fraction), 1.0f / 3.0f);
    if (B.Mass < 0.05f) { B.Mass = 0.0f; B.Radius = 0.0f; }
}

void Collision::Partial_Merge(CelestialBody& A, CelestialBody& B, float mass_loss_fraction)
{
    float mass_loss_A = A.Mass * mass_loss_fraction;
    float mass_loss_B = B.Mass * mass_loss_fraction;

    A.Mass -= mass_loss_A;
    B.Mass -= mass_loss_B;

    Merge(A, B, 0.5f);
}

void Collision::Destroy(CelestialBody& A, CelestialBody& B, float destruction_fraction)
{
    A.Mass *= (1.0f - destruction_fraction);
    B.Mass *= (1.0f - destruction_fraction);
    A.Radius = pow(A.Radius * A.Radius * A.Radius * (1.0f - destruction_fraction), 1.0f / 3.0f);
    B.Radius = pow(B.Radius * B.Radius * B.Radius * (1.0f - destruction_fraction), 1.0f / 3.0f);

    Vector3 combined_vel = Vector3Scale(Vector3Add(A.Vel, B.Vel), 0.5f);
    A.Vel = combined_vel;
    B.Vel = combined_vel;

    if (A.Mass < 0.05f) { A.Mass = 0.0f; A.Radius = 0.0f; }
    if (B.Mass < 0.05f) { B.Mass = 0.0f; B.Radius = 0.0f; }
}