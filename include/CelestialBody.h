#ifndef CELESTIALBODY_H
#define CELESTIALBODY_H


#include "raylib.h"
#include "raymath.h"
#include "math.h"
#include "Dynamic_array.h"
#include "MyString.h"


class CelestialBody {
protected:
    const float G = 0.1f;
    String Name;
    Vector3 Pos;
    Vector3 Vel;
    Vector3 Acc;
    float Mass;
    float Radius;
    Color Body_Color;
    Dynamic_array<Vector3> Trail;
    int Max_Trail_Length = 50000;
    float DeformationAmount = 0.0f;
    bool Textured;
    Model SphereModel;
public:
    friend class Collision;
    virtual bool CheckDelete();
    CelestialBody();

    CelestialBody(String n, Vector3 Pos, Vector3 Vel, const float Mass, const float Radius, Color Body_Color);

    CelestialBody& operator=(const CelestialBody& other);
    virtual ~CelestialBody();

    void Reset_Acceleration();

    virtual void Compute_Gravity_From(const CelestialBody& Other);
    virtual void Apply_Texture(Texture2D& texture);
    
    virtual void Update_Position(float Delta_Time);

    void Apply_Collision_Deformation(float impactEnergy);



    virtual void Draw_Body() const;

    virtual void Draw_Trail() const;
    virtual void Draw_Info_Box() const;
    const Vector3& Get_Velocity() const;
    const float Get_Radius() const;
    const String Get_Name() const;
    const Vector3& Get_Position() const;
    const float Get_Mass() const;
};

#endif
