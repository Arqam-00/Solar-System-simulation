#include "Collision.h"
#include "math.h"

void Collision::Handle_Collision(CelestialBody& A, CelestialBody& B)
{
    if (A.Mass == 0.0f || B.Mass == 0.0f)
        return;

    float Distance = Vector3Distance(A.Pos, B.Pos);
    float Radius_Sum = A.Radius + B.Radius;

    if (Distance > Radius_Sum)
        return;

    Vector3 Relative_Velocity = Vector3Subtract(A.Vel, B.Vel);
    float V_rel = Vector3Length(Relative_Velocity);

    float G = A.G;
    float Total_Mass = A.Mass + B.Mass;
    float V_esc = sqrtf((2.0f * G * Total_Mass) / (Radius_Sum));

    if (V_rel < 0.5f * V_esc)
    {
        Merge(A, B);
    }
    else if (V_rel < 1.2f * V_esc)
    {
        Partial_Merge(A, B, 0.5f);
    }
    else if (V_rel < 3.0f * V_esc)
    {
        Partial_Merge(A, B, 0.2f);
    }
    else
    {
        Elastic_Bounce(A, B);
    }
}

void Collision::Merge(CelestialBody& A, CelestialBody& B)
{
    float Total_Mass = A.Mass + B.Mass;

    A.Pos = Vector3Scale(Vector3Add(Vector3Scale(A.Pos, A.Mass), Vector3Scale(B.Pos, B.Mass)), 1.0f / Total_Mass);
    A.Vel = Vector3Scale(Vector3Add(Vector3Scale(A.Vel, A.Mass), Vector3Scale(B.Vel, B.Mass)), 1.0f / Total_Mass);
    A.Mass = Total_Mass;
    A.Radius = powf(A.Radius * A.Radius * A.Radius + B.Radius * B.Radius * B.Radius, 1.0f / 3.0f);

    B.Mass = 0.0f;
    B.Radius = 0.0f;
}

void Collision::Partial_Merge(CelestialBody& A, CelestialBody& B, float absorb_ratio)
{
    float Transfer_Mass = absorb_ratio * B.Mass;
    A.Mass += Transfer_Mass;
    B.Mass -= Transfer_Mass;

    A.Vel = Vector3Scale(Vector3Add(Vector3Scale(A.Vel, A.Mass), Vector3Scale(B.Vel, Transfer_Mass)), 1.0f / (A.Mass + Transfer_Mass));
    A.Pos = Vector3Scale(Vector3Add(Vector3Scale(A.Pos, A.Mass), Vector3Scale(B.Pos, Transfer_Mass)), 1.0f / (A.Mass + Transfer_Mass));

    A.Radius = powf(A.Radius * A.Radius * A.Radius + powf(Transfer_Mass, 1.0f), 1.0f / 3.0f);

    if (B.Mass < 1.0f)
    {
        B.Mass = 0.0f;
        B.Radius = 0.0f;
    }
}

void Collision::Elastic_Bounce(CelestialBody& A, CelestialBody& B)
{
    Vector3 Temp = A.Vel;
    A.Vel = B.Vel;
    B.Vel = Temp;
}
