#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include "String.h"
#include "Dynamic_Array.h"
#include "CelestialBody.h"
#include "Camera_Control.h"
#include "Collision.h"


using namespace std;

int main()
{
    const int screenWidth = 1200;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "Gravity Simulation for Stable Solar System");
    SetTargetFPS(60);

    Camera_Control My_Camera;

    Dynamic_array<CelestialBody> bodies;

    bodies.push(CelestialBody("Sun", { 0, 0, 0 }, { 0, 0, 0 }, 100000, 20, YELLOW));
    bodies.push(CelestialBody("Planet1", { 50, 0, 0 }, { 0, 10, 0 }, 1000, 3, RED));
    bodies.push(CelestialBody("Planet2", { -70, 0, 0 }, { 0, -8, 0 }, 150, 4, BLUE));
    bodies.push(CelestialBody("Planet3", { 0, 90, 0 }, { -9, 0, 0 }, 2000, 5, ORANGE));
    bodies.push(CelestialBody("Planet4", { 30, -120, 0 }, { 7, 0, 0 }, 2500, 5, PURPLE));

    while (!WindowShouldClose())
    {
        float Delta_Time = GetFrameTime();

        My_Camera.Update(Delta_Time);

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
        Collision C;

        for (int i = 0; i < bodies.size(); i++)
        {
            bodies[i].Update_Position(Delta_Time);
        }
        for (int i = 0; i < bodies.size(); i++)
        {
            for (int j = i + 1; j < bodies.size(); j++)
            {
                C.Handle_Collision(bodies[i], bodies[j]);
            }
        }
        for (int i = 0; i < bodies.size(); )
        {
            if (bodies[i].Get_Mass() == 0.0f)
            {
                bodies.delete_at(i);
            }
            else
            {
                i++;
            }
        }

        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode3D(My_Camera.Get_Camera());

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