#ifndef CELESTIALBODY_H
#define CELESTIALBODY_H

#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include <math.h>
#include "Dynamic_Array.h"
using namespace std;

struct CelestialBody
{
    Vector3 Pos;
    Vector3 Vel;
    float Mass;
    float Radius;
    Color Body_Color;

    CelestialBody()
    {
        Pos = { 0.0f, 0.0f, 0.0f };
        Vel = { 0.0f, 0.0f, 0.0f };
        Mass = 0.0f;
        Radius = 0.0f;
        Body_Color = WHITE;
    }

    CelestialBody(Vector3 Pos, Vector3 Vel, float Mass, float Radius, Color Body_Color)
    {
        this->Pos = Pos;
        this->Vel = Vel;
        this->Mass = Mass;
        this->Radius = Radius;
        this->Body_Color = Body_Color;
    }
};

const float G = 0.1f;

Vector3 Compute_Gravity(const CelestialBody& A, const CelestialBody& B)
{
    Vector3 Direction = Vector3Subtract(B.Pos, A.Pos);
    float Distance_Squared = Vector3LengthSqr(Direction) + 0.000001f;
    float Force = (G * A.Mass * B.Mass) / Distance_Squared;
    Direction = Vector3Normalize(Direction);
    return Vector3Scale(Direction, Force / A.Mass);
}

void Update_Bodies(Dynamic_array<CelestialBody>& Bodies, float Delta_Time)
{
    int N = Bodies.size();
    Dynamic_array<Vector3> Accelerations;

    for (int i = 0; i < N; i++)
    {
        Accelerations.push({ 0.0f, 0.0f, 0.0f });
    }

    for (int i = 0; i < N; i++)
    {
        for (int j = i + 1; j < N; j++)
        {
            Vector3 Ai = Compute_Gravity(Bodies[i], Bodies[j]);
            Vector3 Aj = Compute_Gravity(Bodies[j], Bodies[i]);
            Accelerations[i] = Vector3Add(Accelerations[i], Ai);
            Accelerations[j] = Vector3Add(Accelerations[j], Aj);
        }
    }

    for (int i = 0; i < N; i++)
    {
        Bodies[i].Vel = Vector3Add(Bodies[i].Vel, Vector3Scale(Accelerations[i], Delta_Time));
        Bodies[i].Pos = Vector3Add(Bodies[i].Pos, Vector3Scale(Bodies[i].Vel, Delta_Time));
    }
}

#endif
