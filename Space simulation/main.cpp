#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include "String.h"
#include "Dynamic_Array.h"
#include "CelestialBody.h"
#include "Star.h"
#include "Planet.h"
#include "Moon.h"
#include "Meteor.h"
#include "Dust.h"
#include "Camera_Control.h"
#include "Collision.h"

using namespace std;

void Flatten_Star(Star* star, Dynamic_array<CelestialBody*>& flat)
{
    flat.push(star);
    for (int i = 0; i < star->Get_Number_Of_Planets(); i++)
    {
        Planet* planet = star->Get_Planet_At(i);
        flat.push(planet);
        for (int j = 0; j < planet->Get_Number_Of_Moons(); j++)
        {
            flat.push(planet->Get_Moon_At(j));
        }
    }
}

int main()
{
    const int screenWidth = 1200;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "Hierarchical Solar System");
    SetTargetFPS(60);

    Camera_Control My_Camera;
    Collision C;

    Dynamic_array<Star*> Stars;
    Dynamic_array<CelestialBody*> Free_Bodies;
    Dynamic_array<CelestialBody*> All_Bodies;

    Star* sun = new Star("Sun", { 0, 0, 0 }, { 0, 0, 0 }, 100000, 40, YELLOW);
    Stars.push(sun);

    Planet* mercury = new Planet("Mercury", { 60, 0, 0 }, { 0, 12, 0 }, 15, 3, GRAY);
    Planet* venus = new Planet("Venus", { 90, 0, 0 }, { 0, 10.5f, 0 }, 80, 5, ORANGE);
    Planet* earth = new Planet("Earth", { 120, 0, 0 }, { 0, 9.3f, 0 }, 190, 5.5f, BLUE);
    Planet* mars = new Planet("Mars", { 150, 0, 0 }, { 0, 7.9f, 0 }, 60, 4, RED);

    Planet* jupiter = new Planet("Jupiter", { 250, 0, 0 }, { 0, 6.0f, 0 }, 800, 8, BROWN);
    Planet* saturn = new Planet("Saturn", { 320, 0, 0 }, { 0, 5.3f, 0 }, 600, 7, BEIGE);
    Planet* uranus = new Planet("Uranus", { 400, 0, 0 }, { 0, 4.7f, 0 }, 400, 4, SKYBLUE);
    Planet* neptune = new Planet("Neptune", { 500, 0, 0 }, { 0, 4.0f, 0 }, 350, 6, DARKBLUE);
    sun->Add_Planet(mercury);
    sun->Add_Planet(venus);
    sun->Add_Planet(earth);
    sun->Add_Planet(mars);
    sun->Add_Planet(jupiter);
    sun->Add_Planet(saturn);
    sun->Add_Planet(uranus);
    sun->Add_Planet(neptune);

    Moon* moon = new Moon("Moon", { 130, 0, 10 }, { 0, 9.0f, .0 }, 10, 0.9f, LIGHTGRAY);
    Moon* phobos = new Moon("Phobos", { 155, 3, 0 }, { 0, 8.1f, 0 }, 1, 0.8f, DARKGRAY);
    Moon* europa = new Moon("Europa", { 260, 6, 20 }, { 0, 6.2f, 0 }, 3, 1.2f, LIGHTGRAY);
    earth->Place_Moon_In_Orbit(moon,1.15);
    mars->Place_Moon_In_Orbit(phobos, 1.2);
    jupiter->Place_Moon_In_Orbit(europa, 1.5);



    Moon* deimos = new Moon("Deimos", { 160, 5, 10 }, { 0, 7.9f, 0 }, 0.5f, 0.5f, GRAY);
    mars->Place_Moon_In_Orbit(deimos, 1.5);

    Moon* io = new Moon("Io", { 270, 0, 0 }, { 0, 6.5f, 0 }, 4, 1.2f, ORANGE);
    jupiter->Place_Moon_In_Orbit(io, 1.5);

    Moon* ganymede = new Moon("Ganymede", { 290, 20, -30 }, { 0, 6.0f, 0 }, 5, 1.5f, BROWN);
    jupiter->Place_Moon_In_Orbit(ganymede, 1.5);

    Moon* callisto = new Moon("Callisto", { 240, 25, 20 }, { 0, 5.7f, 0 }, 4, 1.3f, DARKBROWN);
    jupiter->Place_Moon_In_Orbit(callisto, 1.5);

    Moon* titan = new Moon("Titan", { 340, -15, 0 }, { 0, 5.0f, 0 }, 5, 1.3f, BEIGE);
    saturn->Place_Moon_In_Orbit(titan, 1.5);

    Moon* enceladus = new Moon("Enceladus", { 335, 10, 20 }, { 0, 5.3f, 0 }, 2, 0.7f, LIGHTGRAY);
    saturn->Place_Moon_In_Orbit(enceladus, 1.5);

    Moon* titania = new Moon("Titania", { 420, 15, 0 }, { 0, 4.4f, 0 }, 3, 1.1f, SKYBLUE);
    uranus->Place_Moon_In_Orbit(titania, 1.5);

    Moon* oberon = new Moon("Oberon", { 430, -20, 10 }, { 0, 4.2f, 0 }, 3, 1.0f, DARKBLUE);
    uranus->Place_Moon_In_Orbit(oberon, 1.5);

    Moon* triton = new Moon("Triton", { 510, 18, 0 }, { 0, 4.0f, 0 }, 4, 1.2f, WHITE);
    neptune->Place_Moon_In_Orbit(triton, 1.5);



    for (int i = 0; i < sun->Get_Number_Of_Planets(); i++)
    {
        sun->Stabilize_Orbit(i);

        for (int j = 0; j < sun->Get_Planet_At(i)->Get_Number_Of_Moons(); j++)
        {
            sun->Get_Planet_At(i)->Stabilize_Orbit(j);
        }
    }


    

   

    for (int i = 0; i < Stars.size(); i++)
    {
        Flatten_Star(Stars[i], All_Bodies);
    }

    Free_Bodies.push(new CelestialBody("Asteroid1", { 180, -50, 0 }, { 3, 7.5f, 0 }, 10, 1, DARKGRAY));
    Free_Bodies.push(new CelestialBody("Asteroid2", { 220, 60, 0 }, { -3, 7.2f, 0 }, 5, 0.8f, LIGHTGRAY));
    Free_Bodies.push(new CelestialBody("Comet1", { 300, -150, 0 }, { 2.5f, 5.8f, 0 }, 20, 1.5f, WHITE));
    Free_Bodies.push(new CelestialBody("Comet2", { -400, 100, 0 }, { 2.0f, 4.5f, 0 }, 25, 2, WHITE));

    Free_Bodies.push(new Meteor("Meteor1", { 450, 200, 0 }, { 0, 4.2f, 0 }, 2, 0.8f, GRAY));
    Free_Bodies.push(new Meteor("Meteor2", { 350, -300, 0 }, { 0, 4.8f, 0 }, 3, 0.9f, DARKGRAY));
    Free_Bodies.push(new Meteor("Meteor3", { 500, 400, 0 }, { 0, 3.9f, 0 }, 1.5f, 0.6f, LIGHTGRAY));
    Free_Bodies.push(new Meteor("Meteor4", { -600, 250, 0 }, { 0, 4.1f, 0 }, 2.5f, 1.0f, DARKBROWN));

    All_Bodies += Free_Bodies;

    while (!WindowShouldClose())
    {
        float Delta_Time = GetFrameTime();
        CelestialBody* HoveredBody = nullptr;

        My_Camera.Update(Delta_Time);
        My_Camera.Update_Info(Delta_Time, All_Bodies, HoveredBody);

        for (int i = 0; i < All_Bodies.size(); i++)
        {
            All_Bodies[i]->Reset_Acceleration();
        }

        for (int i = 0; i < All_Bodies.size(); i++)
        {
            for (int j = 0; j < All_Bodies.size(); j++)
            {
                if (i != j)
                {
                    All_Bodies[i]->Compute_Gravity_From(*All_Bodies[j]);
                }
            }
        }

        for (int i = 0; i < All_Bodies.size(); i++)
        {
            for (int j = i + 1; j < All_Bodies.size(); j++)
            {
                C.Handle_Collision(*All_Bodies[i], *All_Bodies[j], All_Bodies);
            }
        }

        for (int i = 0; i < All_Bodies.size(); )
        {
            if (All_Bodies[i]->Get_Mass() == 0.0f)
            {
                delete All_Bodies[i];
                All_Bodies.delete_at(i);
            }
            else
            {
                i++;
            }
        }

        for (int i = 0; i < All_Bodies.size(); i++)
        {
            All_Bodies[i]->Update_Position(Delta_Time);
        }

        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode3D(My_Camera.Get_Camera());

        for (int i = 0; i < All_Bodies.size(); i++)
        {
            All_Bodies[i]->Draw_Body();
            All_Bodies[i]->Draw_Trail();
        }

        EndMode3D();
        DrawFPS(10, 10);

        if (HoveredBody != nullptr)
        {
            HoveredBody->Draw_Info_Box();
        }

        if (My_Camera.Is_Showing_Names())
        {
            for (int i = 0; i < All_Bodies.size(); i++)
            {
                Vector2 screenPos = GetWorldToScreenEx(All_Bodies[i]->Get_Position(), My_Camera.Get_Camera(), GetScreenWidth(), GetScreenHeight());
                DrawText(All_Bodies[i]->Get_Name().C_Str(), (int)screenPos.x, (int)screenPos.y, 20, RED);
            }
        }

        EndDrawing();
    }

    for (int i = 0; i < All_Bodies.size(); i++) delete All_Bodies[i];
    All_Bodies.clear();



    CloseWindow();
    return 0;
}
