#include "Dust.h"


Dust::Dust(String n, Vector3 Pos, Vector3 Vel, float Mass, float Radius, Color Body_Color)
    : CelestialBody(n, Pos, Vel, Mass, Radius, Body_Color)
{
    Max_Trail_Length = 100;
    Lifetime = 3.0f;
    FadeSpeed = 1.0f;
}

void Dust::Update_Position(float Delta_Time)  {
    Lifetime -= Delta_Time;
    if (Lifetime < 0) Lifetime = 0;

    Vel = Vector3Add(Vel, Vector3Scale(Acc, Delta_Time));
    Pos = Vector3Add(Pos, Vector3Scale(Vel, Delta_Time));
    

    Trail.push(Pos);
    if (Trail.size() > Max_Trail_Length) {
        Trail.delete_at(0);
    }
}

void Dust::Draw_Body() const  {
    if (Lifetime <= 0) return;

    float alpha = Lifetime / 3.0f;
    Color faded_color = { Body_Color.r, Body_Color.g, Body_Color.b, (unsigned char)(alpha * 255) };
    DrawSphere(Get_Position(), Get_Radius(), faded_color);
}

bool Dust::CheckDelete()  {
    return (Mass <= 0.0f || Lifetime <= 0.0f);
}