#ifndef CELESTIALBODY_H
#define CELESTIALBODY_H

using namespace std;

#include "raylib.h"
#include "raymath.h"
#include "math.h"
#include <iostream>
#include "Dynamic_array.h"
#include "String.h"

class CelestialBody{
private:
    const float G = 0.1f;
    String Name;
    Vector3 Pos;
    Vector3 Vel;
    Vector3 Acc;
    float Mass;
    float Radius;
    Color Body_Color;
    Dynamic_array<Vector3> Trail;
    int Max_Trail_Length = 50;

public:

    CelestialBody()
    {
        Name = "none";
        Pos = { 0.0f, 0.0f, 0.0f };
        Vel = { 0.0f, 0.0f, 0.0f };
        Acc = { 0.0f, 0.0f, 0.0f };
        Mass = 0.0f;
        Radius = 0.0f;
        Body_Color = WHITE;
    }

    CelestialBody(const String n,Vector3 Pos, Vector3 Vel,const float Mass,const float Radius, Color Body_Color)
    {
        this->Name = n;
        this->Pos = Pos;
        this->Vel = Vel;
        this->Mass = Mass;
        this->Radius = Radius;
        this->Body_Color = Body_Color;
        this->Acc = { 0.0f, 0.0f, 0.0f };
    }
    CelestialBody& operator=(const CelestialBody& other)
    {
        if (this != &other)
        {
            Name = other.Name;
            Pos = other.Pos;
            Vel = other.Vel;
            Acc = other.Acc;
            Mass = other.Mass;
            Radius = other.Radius;
            Body_Color = other.Body_Color;
            Trail = other.Trail;
        }
        return *this;
    }


    void Reset_Acceleration()
    {
        Acc = { 0.0f, 0.0f, 0.0f };
    }

    void Compute_Gravity_From(const CelestialBody& Other)
    {
        Vector3 Direction = Vector3Subtract(Other.Pos, Pos);
        float Distance_Squared = Vector3LengthSqr(Direction) + 0.000001f;
        float Force = (G * Mass * Other.Mass) / Distance_Squared;
        Direction = Vector3Normalize(Direction);
        Vector3 Acceleration_Contribution = Vector3Scale(Direction, Force / Mass);
        Acc = Vector3Add(Acc, Acceleration_Contribution);
    }

    void Update_Position(float Delta_Time)
    {
        Vel = Vector3Add(Vel, Vector3Scale(Acc, Delta_Time));
        Pos = Vector3Add(Pos, Vector3Scale(Vel, Delta_Time));

        Trail.push(Pos);
        if (Trail.size() > Max_Trail_Length)
        {
            Trail.delete_at(0);
        }
    }

    void Draw_Body() const
    {
        DrawSphere(Pos, Radius, Body_Color);
    }

    void Draw_Trail() const
    {
        for (int i = 1; i < Trail.size(); i++)
        {
            DrawLine3D(Trail[i - 1], Trail[i], Body_Color);
        }
    }

    const Vector3& Get_Position() const { return Pos; }
    const float Get_Mass() const { return Mass; }
};

#endif
