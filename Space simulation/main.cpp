#include "raylib.h"
#include "raymath.h"
#include <iostream>
using namespace std;

#include "CelestialBody.h"
#include "Camera_Control.h"

// Include your String and Dynamic_array here

int main()
{
    InitWindow(1280, 720, "Gravity Simulation 3D");
    SetTargetFPS(60);

    Camera_Control Camera;

    Dynamic_array<CelestialBody> Bodies;

    // Sun
    Bodies.push(CelestialBody({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, 10000.0f, 20.0f, YELLOW));

    // Planet
    Bodies.push(CelestialBody({ 150.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 35.0f }, 10.0f, 8.0f, BLUE));

    float Delta_Time = 0.01f;

    while (!WindowShouldClose())
    {
        Camera.Update();
        Update_Bodies(Bodies, Delta_Time);

        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode3D(Camera.Cam);

        for (int i = 0; i < Bodies.size(); i++)
        {
            DrawSphere(Bodies[i].Pos, Bodies[i].Radius, Bodies[i].Body_Color);
        }

        EndMode3D();
        DrawFPS(10, 10);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
