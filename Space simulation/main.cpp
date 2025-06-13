#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include "String.h"
#include "Dynamic_Array.h"
#include "CelestialBody.h"
#include "Meteor.h"
#include "Dust.h"
#include "Camera_Control.h"
#include "Collision.h"

using namespace std;

int main()
{
    const int screenWidth = 1200;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "Solar System");
    SetTargetFPS(60);

    Camera_Control My_Camera;
    Collision C;

    Dynamic_array<CelestialBody*> bodies;
    bodies.push(new CelestialBody("Sun", { 90, 0, 0 }, { 0, 0, 3 }, 100000, 30, YELLOW));
    //bodies.push(new CelestialBody("Sun", { 0, 90, 0 }, { 3, 0, 3 }, 1000, 30, GREEN));


    //bodies.push(new CelestialBody("Sun", { 0, 0, 0 }, { 0, 0, 3 }, 1000000, 20, YELLOW));

    bodies.push(new CelestialBody("Mercury", { 60, 0, 0 }, { 0, 12, 0 }, 15, 3, GRAY));
    bodies.push(new CelestialBody("Venus", { 90, 0, 0 }, { 0, 10.5f, 0 }, 80, 5, ORANGE));
    bodies.push(new CelestialBody("Earth", { 120, 0, 0 }, { 0, 9.3f, 0 }, 190, 5.5f, BLUE));
    bodies.push(new CelestialBody("Mars", { 150, 0, 0 }, { 0, 7.9f, 0 }, 60, 4, RED));

    bodies.push(new CelestialBody("Jupiter", { 250, 0, 0 }, { 0, 6.0f, 0 }, 800, 8, BROWN));
    bodies.push(new CelestialBody("Saturn", { 320, 0, 0 }, { 0, 5.3f, 0 }, 600, 7, BEIGE));

    bodies.push(new CelestialBody("Uranus", { 400, 0, 0 }, { 0, 4.7f, 0 }, 400, 4, SKYBLUE));
    bodies.push(new CelestialBody("Neptune", { 500, 0, 0 }, { 0, 4.0f, 0 }, 350, 6, DARKBLUE));

    bodies.push(new CelestialBody("Moon", { 110, 0, 0 }, { -0.9, 9.4f, -3.9 }, 05, 1.2f, LIGHTGRAY));
    bodies.push(new CelestialBody("Phobos", { 152, 3, 0 }, { 0, 8.1f, 0 }, 1, 0.8f, DARKGRAY));
    bodies.push(new CelestialBody("Europa", { 252, 6, 0 }, { 0, 6.2f, 0 }, 3, 1.2f, LIGHTGRAY));

    bodies.push(new CelestialBody("Asteroid1", { 180, -50, 0 }, { 3, 7.5f, 0 }, 10, 1, DARKGRAY));
    bodies.push(new CelestialBody("Asteroid2", { 220, 60, 0 }, { -3, 7.2f, 0 }, 5, 0.8f, LIGHTGRAY));

    bodies.push(new CelestialBody("Comet1", { 300, -150, 0 }, { 2.5f, 5.8f, 0 }, 20, 1.5f, WHITE));
    bodies.push(new CelestialBody("Comet2", { -400, 100, 0 }, { 2.0f, 4.5f, 0 }, 25, 2, WHITE));

    bodies.push(new Meteor("Meteor1", { 450, 200, 0 }, { 0, 4.2f, 0 }, 2, 0.8f, GRAY));
    bodies.push(new Meteor("Meteor2", { 350, -300, 0 }, { 0, 4.8f, 0 }, 3, 0.9f, DARKGRAY));
    bodies.push(new Meteor("Meteor3", { 500, 400, 0 }, { 0, 3.9f, 0 }, 1.5f, 0.6f, LIGHTGRAY));
    bodies.push(new Meteor("Meteor4", { -600, 250, 0 }, { 0, 4.1f, 0 }, 2.5f, 1.0f, DARKBROWN));






    while (!WindowShouldClose())
    {
        float Delta_Time = GetFrameTime();
        CelestialBody* HoveredBody = nullptr;

        My_Camera.Update(Delta_Time);
        My_Camera.Update_Info(Delta_Time, bodies, HoveredBody);

        for (int i = 0; i < bodies.size(); i++)
        {
            bodies[i]->Reset_Acceleration();
        }

        for (int i = 0; i < bodies.size(); i++)
        {
            for (int j = 0; j < bodies.size(); j++)
            {
                if (i != j)
                {
                    bodies[i]->Compute_Gravity_From(*bodies[j]);
                }
            }
        }

        for (int i = 0; i < bodies.size(); i++)
        {
            for (int j = i + 1; j < bodies.size() ; j++)
            {

                C.Handle_Collision(*bodies[i], *bodies[j], bodies);
            }
        }

        for (int i = 0; i < bodies.size(); )
        {
            if (bodies[i]->Get_Mass() == 0.0f)
            {
                delete bodies[i];
                bodies.delete_at(i);
            }
            else
            {
                i++;
            }
        }

        for (int i = 0; i < bodies.size(); i++)
        {
            bodies[i]->Update_Position(Delta_Time);
        }

        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode3D(My_Camera.Get_Camera());

        for (int i = 0; i < bodies.size(); i++)
        {
            bodies[i]->Draw_Body();
            bodies[i]->Draw_Trail();
        }

        


        EndMode3D();
        DrawFPS(10, 10);
        if (HoveredBody != nullptr)
        {
            HoveredBody->Draw_Info_Box();
        }
        if (My_Camera.Is_Showing_Names())
        {
            for (int i = 0; i < bodies.size(); i++)
            {
                Vector2 screenPos = GetWorldToScreenEx(bodies[i]->Get_Position(), My_Camera.Get_Camera(), GetScreenWidth(), GetScreenHeight());
                DrawText(bodies[i]->Get_Name().C_Str(), (int)screenPos.x, (int)screenPos.y, 20, RED);


            }
        }
        EndDrawing();
    }

    for (int i = 0; i < bodies.size(); i++)
    {
        delete bodies[i];
    }
    bodies.clear();

    CloseWindow();
    return 0;
}

