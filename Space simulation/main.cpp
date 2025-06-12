#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include "String.h"
#include "Dynamic_Array.h"
#include "CelestialBody.h"

using namespace std;

int main()
{
    const int screenWidth = 1200;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "Gravity Simulation - Stable Solar System");
    SetTargetFPS(60);

    Camera3D camera = { 0 };
    camera.position = { 0.0f, 50.0f, 200.0f };
    camera.target = { 0.0f, 0.0f, 0.0f };
    camera.up = { 0.0f, 1.0f, 0.0f };
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    Dynamic_array<CelestialBody> bodies;

    bodies.push(CelestialBody("Sun", { 0, 0, 0 }, { 0, 0, 0 }, 100000, 20, YELLOW));
    bodies.push(CelestialBody("Planet1", { 50, 0, 0 }, { 0, 10, 0 }, 1000, 3, RED));
    bodies.push(CelestialBody("Planet2", { -70, 0, 0 }, { 0, -8, 0 }, 150, 4, BLUE));
    bodies.push(CelestialBody("Planet3", { 0, 90, 0 }, { -9, 0, 0 }, 200, 5, ORANGE));
    bodies.push(CelestialBody("Planet4", { 30, -120, 0 }, { 7, 0, 0 }, 250, 5, PURPLE));

    while (!WindowShouldClose())
    {
        for (int i = 0; i < bodies.size(); i++)
        {
            bodies[i].Reset_Acceleration();
        }

        for (int i = 0; i < bodies.size(); i++)
        {
            for (int j = 0; j < bodies.size(); j++)
            {
                if (i != j)
                {
                    bodies[i].Compute_Gravity_From(bodies[j]);
                }
            }
        }

        for (int i = 0; i < bodies.size(); i++)
        {
            bodies[i].Update_Position(GetFrameTime());
        }

        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode3D(camera);

        for (int i = 0; i < bodies.size(); i++)
        {
            bodies[i].Draw_Body();
            bodies[i].Draw_Trail();
        }

        EndMode3D();

        DrawFPS(10, 10);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
