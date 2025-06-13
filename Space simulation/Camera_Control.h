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

public:

    Camera_Control()
    {
        Cam.position = { 200.0f, 100.0f, 300.0f };
        Cam.target = { 0.0f, 0.0f, 0.0f };
        Cam.up = { 0.0f, 1.0f, 0.0f };
        Cam.fovy = 90.0f;
        Cam.projection = CAMERA_PERSPECTIVE;
    }
    void Update_Info(float Delta_Time, Dynamic_array<CelestialBody*>& bodies,CelestialBody*& HoveredBody) {
        Vector2 mousePosition = GetMousePosition();
        Ray ray = GetMouseRay(mousePosition, Cam);

        HoveredBody = nullptr;

        for (int i = 0; i < bodies.size(); i++) {
            CelestialBody* body = bodies[i];
            float hitDistance = 0.0f;
            bool hit = GetRayCollisionSphere(ray, body->Get_Position(), body->Get_Radius()).hit;

            if (hit) {
                HoveredBody = body;
                break;
            }
        }
    }


    void Update(float Delta_Time)
    {
        Vector3 Forward = Vector3Normalize(Vector3Subtract(Cam.target, Cam.position));
        Vector3 Right = Vector3Normalize(Vector3CrossProduct(Forward, Cam.up));
        Vector3 Upward = Vector3Normalize(Vector3CrossProduct(Right, Forward));

        Vector3 Move = { 0 };

        if (IsKeyDown(KEY_W)) Move = Vector3Add(Move, Vector3Scale(Forward, Move_Speed * Delta_Time*10));
        if (IsKeyDown(KEY_S)) Move = Vector3Add(Move, Vector3Scale(Forward, -Move_Speed * Delta_Time*10));
        if (IsKeyDown(KEY_D)) Move = Vector3Add(Move, Vector3Scale(Right, Move_Speed * Delta_Time*10));
        if (IsKeyDown(KEY_A)) Move = Vector3Add(Move, Vector3Scale(Right, -Move_Speed * Delta_Time*10));
        if (IsKeyDown(KEY_SPACE)) Move = Vector3Add(Move, Vector3Scale(Upward, Move_Speed * Delta_Time * 10));
        if (IsKeyDown(KEY_LEFT_SHIFT)) Move = Vector3Add(Move, Vector3Scale(Upward, -Move_Speed * Delta_Time * 10));



        Cam.position = Vector3Add(Cam.position, Move);
        Cam.target = Vector3Add(Cam.target, Move);

        if (Mouse_Mode_Active)
        {
            float Mouse_X = GetMouseDelta().x * Rotation_Speed;
            float Mouse_Y = GetMouseDelta().y * Rotation_Speed;

            Vector3 Forward_Dir = Vector3Subtract(Cam.target, Cam.position);

            Matrix Rot_Y = MatrixRotateY(-Mouse_X);
            Forward_Dir = Vector3Transform(Forward_Dir, Rot_Y);

            Vector3 Right_Axis = Vector3Normalize(Vector3CrossProduct(Forward_Dir, Cam.up));
            Matrix Rot_X = MatrixRotate(Right_Axis, -Mouse_Y);
            Forward_Dir = Vector3Transform(Forward_Dir, Rot_X);

            Cam.target = Vector3Add(Cam.position, Forward_Dir);
        }

        float Wheel = GetMouseWheelMove();
        if (Wheel != 0.0f)
        {
            Vector3 Forward_Dir = Vector3Normalize(Vector3Subtract(Cam.target, Cam.position));
            Vector3 Zoom = Vector3Scale(Forward_Dir, Wheel * Zoom_Speed);
            Cam.position = Vector3Add(Cam.position, Zoom);
        }

        if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
        {
            Mouse_Mode_Active = !Mouse_Mode_Active;
            if (Mouse_Mode_Active)
                DisableCursor();
            else
                EnableCursor();
        }
    }

    Camera3D Get_Camera() const
    {
        return Cam;
    }
};

#endif
