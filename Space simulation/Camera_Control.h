#ifndef CAMERA_CONTROL_H
#define CAMERA_CONTROL_H

#include "raylib.h"
#include "raymath.h"
#include "CelestialBody.h"
#include <iostream>
using namespace std;

class Camera_Control
{
private:
    Camera3D Cam;
    bool Mouse_Mode_Active = false;
    float Move_Speed = 10.0f;
    float Rotation_Speed = 0.005f;
    float Zoom_Speed = 20.0f;
    bool Show_Names = false;

public:

    Camera_Control();
    void Update_Info(float Delta_Time, Dynamic_array<CelestialBody*>& bodies, CelestialBody*& HoveredBody);
    void Update(float Delta_Time);
    bool Is_Showing_Names() const;
    Camera3D Get_Camera() const;
};

#endif
