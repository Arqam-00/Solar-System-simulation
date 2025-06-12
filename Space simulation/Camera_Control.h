#ifndef CAMERA_CONTROL_H
#define CAMERA_CONTROL_H

#include "raylib.h"
#include <iostream>
using namespace std;

struct Camera_Control
{
    Camera3D Cam;

    Camera_Control()
    {
        Cam.position = { 200.0f, 100.0f, 300.0f };
        Cam.target = { 0.0f, 0.0f, 0.0f };
        Cam.up = { 1.0f, 1.0f, 1.0f };
        Cam.fovy = 90.0f;
        Cam.projection = CAMERA_PERSPECTIVE;
    }

    void Update()
    {
        UpdateCamera(&Cam, CAMERA_FREE);
    }
};

#endif
