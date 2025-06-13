#ifndef CELESTIALBODY_H
#define CELESTIALBODY_H

using namespace std;

#include "raylib.h"
#include "raymath.h"
#include "math.h"
#include <iostream>
#include "Dynamic_array.h"
#include "String.h"

class CelestialBody {
protected:
    const float G = 0.1f;
    String Name;
    Vector3 Pos;
    Vector3 Vel;
    Vector3 Acc;
    float Mass;
    float Radius;
    float StableRadius;
    Color Body_Color;
    Dynamic_array<Vector3> Trail;
    int Max_Trail_Length = 5000;
    float DeformationAmount = 0.0f;

public:
    friend class Collision;

    CelestialBody() {
        Name = "none";
        Pos = { 0.0f, 0.0f, 0.0f };
        Vel = { 0.0f, 0.0f, 0.0f };
        Acc = { 0.0f, 0.0f, 0.0f };
        Mass = 0.0f;
        Radius = 0.0f;
        StableRadius = Radius;
        Body_Color = WHITE;
    }

    CelestialBody(String n, Vector3 Pos, Vector3 Vel, const float Mass, const float Radius, Color Body_Color) {
        this->Name = n;
        this->Pos = Pos;
        this->Vel = Vel;
        this->Mass = Mass;
        this->Radius = Radius;
        this->StableRadius = Radius;
        this->Body_Color = Body_Color;
        this->Acc = { 0.0f, 0.0f, 0.0f };
    }

    CelestialBody& operator=(const CelestialBody& other) {
        if (this != &other) {
            Name = other.Name;
            Pos = other.Pos;
            Vel = other.Vel;
            Acc = other.Acc;
            Mass = other.Mass;
            Radius = other.Radius;
            StableRadius = other.StableRadius;
            Body_Color = other.Body_Color;
            Trail = other.Trail;
            DeformationAmount = other.DeformationAmount;
        }
        return *this;
    }

    void Reset_Acceleration() {
        Acc = { 0.0f, 0.0f, 0.0f };
    }

    void Compute_Gravity_From(const CelestialBody& Other) {
        Vector3 Direction = Vector3Subtract(Other.Pos, Pos);
        float Distance_Squared = Vector3LengthSqr(Direction) + 0.000001f;
        float Force = (G * Mass * Other.Mass) / Distance_Squared;
        Direction = Vector3Normalize(Direction);
        Vector3 Acceleration_Contribution = Vector3Scale(Direction, Force / Mass);
        Acc = Vector3Add(Acc, Acceleration_Contribution);
    }

    void Update_Position(float Delta_Time) {
        Vel = Vector3Add(Vel, Vector3Scale(Acc, Delta_Time));
        Pos = Vector3Add(Pos, Vector3Scale(Vel, Delta_Time));

        Radius = Lerp(Radius, StableRadius, 0.02f);
        DeformationAmount *= 0.98f;

        float rippleEffect = sin(GetTime() * 5.0f) * DeformationAmount * 0.5f;
        Radius += rippleEffect;

        Trail.push(Pos);
        if (Trail.size() > Max_Trail_Length) {
            Trail.delete_at(0);
        }
    }

    void Apply_Collision_Deformation(float impactEnergy, const Vector3& impactDirection) {
        float deformationFactor = impactEnergy * 0.002f;
        DeformationAmount = deformationFactor;
        Pos = Vector3Add(Pos, Vector3Scale(Vector3Normalize(impactDirection), deformationFactor * Radius));

    }


    void Draw_Body() const {
        DrawSphere(Pos, Radius, Body_Color);
    }

    void Draw_Trail() const {
        for (int i = 1; i < Trail.size(); i++) {
            DrawLine3D(Trail[i - 1], Trail[i], Body_Color);
        }
    }
    void Draw_Info_Box() const {
            Vector2 mousePos = GetMousePosition();
            float boxWidth = 200;
            float boxHeight = 100;

            DrawRectangle(mousePos.x + 10, mousePos.y + 10, boxWidth, boxHeight, Fade(BLACK, 0.7f)); // Background
            DrawText(Name.C_Str(), mousePos.x + 20, mousePos.y + 20, 20, WHITE);
            DrawText(TextFormat("Mass: %.2f", Mass), mousePos.x + 20, mousePos.y + 40, 20, WHITE);
            DrawText(TextFormat("Radius: %.2f", Radius), mousePos.x + 20, mousePos.y + 60, 20, WHITE);
    }

    const Vector3& Get_Velocity() const { return Vel; }
    const float Get_Radius() const { return Radius; }
    const String Get_Name() const { return Name; }
    const Vector3& Get_Position() const { return Pos; }
    const float Get_Mass() const { return Mass; }
};

#endif
