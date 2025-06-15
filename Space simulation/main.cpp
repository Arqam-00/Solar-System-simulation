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
    
    Star* sun = new Star("Sun", { 0, 0, 0 }, { 0, 0, 0 }, 10000, 45, YELLOW);
    //Star* sun2 = new Star("Sun2", { 300, 100, -220 }, { -5, -3, 0 }, 100000, 50, YELLOW);
    //Stars.push(sun2);

    Stars.push(sun);

    Planet* mercury = new Planet("Mercury", { 60, 0, 0 }, { 0, 12, 0 }, 15, 3, GRAY);
    Planet* venus = new Planet("Venus", { 90, 0, 0 }, { 0, 10.5f, 0 }, 80, 5, ORANGE);
    Planet* earth = new Planet("Earth", { 160, 0, 0 }, { 0, 7.3f, 0 }, 100, 5.5f, BLUE);
    Planet* mars = new Planet("Mars", { 205, 0, 0 }, { 0, 6.9f, 0 }, 70, 4, RED);

    Planet* jupiter = new Planet("Jupiter", { 250, 0, 0 }, { 0, 6.0f, 0 }, 800, 8, BROWN);
    Planet* saturn = new Planet("Saturn", { 320, 0, 0 }, { 0, 5.3f, 0 }, 600, 7, BEIGE);
    Planet* uranus = new Planet("Uranus", { 400, 0, 0 }, { 0, 4.7f, 0 }, 400, 4, SKYBLUE);
    Planet* neptune = new Planet("Neptune", { 500, 0, 0 }, { 0, 4.0f, 0 }, 350, 6, DARKBLUE);
    sun->Place_Planet_In_Orbit(mercury,1.7);
    sun->Place_Planet_In_Orbit(venus,2.1);
    sun->Place_Planet_In_Orbit(earth,3.0);
    sun->Place_Planet_In_Orbit(mars,3.8);


    sun->Place_Planet_In_Orbit(jupiter,7.3);
    sun->Place_Planet_In_Orbit(saturn,7.9);
    sun->Place_Planet_In_Orbit(uranus,8.4);
    sun->Place_Planet_In_Orbit(neptune,9.0);

    Moon* moon = new Moon("Moon", { 0, 0, 0 }, { 0, 7.0f, .0 }, 5, 1.1f, LIGHTGRAY);
    Moon* phobos = new Moon("Phobos", { 155, 3, 0 }, { 0, 7.0f, 0 }, 1.2, 0.8f, DARKGRAY);
    Moon* europa = new Moon("Europa", { 260, 6, 20 }, { 0, 6.2f, 0 }, 3, 1.2f, LIGHTGRAY);
    earth->Place_Moon_In_Orbit(moon,1.7);
    mars->Place_Moon_In_Orbit(phobos, 1.8);
    jupiter->Place_Moon_In_Orbit(europa, 2.8);



    Moon* deimos = new Moon("Deimos", { 160, 5, 10 }, { 0, 7.9f, 0 }, 0.5f, 0.5f, GRAY);
    mars->Place_Moon_In_Orbit(deimos, 1.6);

    Moon* io = new Moon("Io", { 270, -20, 0 }, { 0, 6.5f, -3.0f }, 4, 1.2f, ORANGE);
    jupiter->Place_Moon_In_Orbit(io, 2.1f);

    Moon* ganymede = new Moon("Ganymede", { 290, 20, -30 }, { 0, 6.0f, 3.0f }, 5, 1.5f, BROWN);
    jupiter->Place_Moon_In_Orbit(ganymede, 2.9f);

    Moon* callisto = new Moon("Callisto", { 220, 20, 20 }, { 0, 6.7f, 0 }, 4, 1.3f, DARKBROWN);
    jupiter->Place_Moon_In_Orbit(callisto, 2.7f);

    Moon* titan = new Moon("Titan", { 340, -15, 0 }, { 0, 5.0f, 0 }, 5, 1.3f, BEIGE);
    saturn->Place_Moon_In_Orbit(titan, 1.5);

    Moon* enceladus = new Moon("Enceladus", { 335, 10, 20 }, { 0, 5.3f, 0 }, 2, 0.7f, LIGHTGRAY);
    saturn->Place_Moon_In_Orbit(enceladus, 1.5);

    Moon* titania = new Moon("Titania", { 420, 15, 0 }, { 0, 4.4f, 0 }, 3, 1.1f, SKYBLUE);
    uranus->Place_Moon_In_Orbit(titania, 2.5);

    Moon* oberon = new Moon("Oberon", { 430, -20, 10 }, { 0, 4.2f, 0 }, 3, 1.0f, DARKBLUE);
    uranus->Place_Moon_In_Orbit(oberon, 2.5);

    Moon* triton = new Moon("Triton", { 510, 18, 0 }, { 0, 4.0f, 0 }, 4, 1.2f, WHITE);
    neptune->Place_Moon_In_Orbit(triton, 2.5);

    saturn->Add_Ring(30);
    sun->Add_Asteroid_Belt(80, 230, 290, Free_Bodies);

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
    int count = 0;
    bool Paused = false;
    bool Show_Trail = true;

    Texture2D texture[11] = { LoadTexture("Textures&shaders/Sun2.png")
    ,LoadTexture("Textures&shaders/Mercury.png")
    ,LoadTexture("Textures&shaders/Venus.jpg")
    ,LoadTexture("Textures&shaders/Earth.png")
    ,LoadTexture("Textures&shaders/Mars.png")
    ,LoadTexture("Textures&shaders/Jupiter.png")
    ,LoadTexture("Textures&shaders/Saturn.png")
    ,LoadTexture("Textures&shaders/Uranus.png")
    ,LoadTexture("Textures&shaders/Neptune.png")
    ,LoadTexture("Textures&shaders/Moon.png"),
     LoadTexture("Textures&shaders/Sun.png")
    };
    Stars[0]->Apply_Texture(texture[0]);
    mercury->Apply_Texture(texture[1]);
    venus->Apply_Texture(texture[2]);
    earth->Apply_Texture(texture[3]);
    mars->Apply_Texture(texture[4]);
    jupiter->Apply_Texture(texture[5]);
    saturn->Apply_Texture(texture[6]);
    uranus->Apply_Texture(texture[7]);
    neptune->Apply_Texture(texture[8]);
    moon->Apply_Texture(texture[9]);
    //Stars[1]->Apply_Texture(texture[10]);

    while (!WindowShouldClose())
    {

        float Delta_Time = GetFrameTime();
        CelestialBody* HoveredBody = nullptr;
        My_Camera.Update(Delta_Time);
        My_Camera.Update_Info(Delta_Time, All_Bodies, HoveredBody);

        if (IsKeyPressed(KEY_P))
        {

            Paused = !Paused;
        }
        if (!Paused) {
            

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
            
            for (int i = All_Bodies.size() - 1; i >= 0;i-- )
            {
                if (All_Bodies[i]->Get_Mass() <=0)
                {
                    for (int j = Stars.size() - 1; j >= 0; j--) {
                        if (Stars[j]->Get_Mass() <= 0) {
                            Stars.delete_at(j);
                        }
                        else {
                            Stars[j]->CheckDelete(All_Bodies[i]);
                        }
                    }
                    delete All_Bodies[i];
                    All_Bodies[i] = nullptr;
                    All_Bodies.delete_at(i);

                }

            }
            //cout << Stars[0]->Get_Name() << "  ," << Stars[0]->Get_Mass() << endl;
            //cout << Stars[1]->Get_Name() << "  ," << Stars[1]->Get_Mass() << endl;

            if (IsKeyDown(KEY_F)) 
            {
                Delta_Time *= 10;
            }
            else
            {
                Delta_Time = GetFrameTime();
            }
            for (int i = 0; i < All_Bodies.size(); i++)
            {
                All_Bodies[i]->Update_Position(Delta_Time);
            }
        }

        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode3D(My_Camera.Get_Camera());
        if (IsKeyDown(KEY_T))
        {
            Show_Trail = !Show_Trail;
        }


        for (int i = 0; i < All_Bodies.size(); i++)
        {
            All_Bodies[i]->Draw_Body();
            if (Show_Trail) {
                All_Bodies[i]->Draw_Trail();
            }
        }
        //Vector3 position = { 1.0f, 1.2f, 3.0f };
        //DrawModel(sphereModel, position, 1.0f, WHITE);

        EndMode3D();
        DrawFPS(10, 10);
        for (int i = 0; i < Stars.size(); i++) {
            Stars[i]->Shine_Draw(My_Camera.Get_Camera(), screenWidth, screenHeight);
        }
        if (HoveredBody != nullptr)
        {
            HoveredBody->Draw_Info_Box();
        }
        if (Paused) {
            DrawText("ZA WORLD", 400, 300, 100, RED);

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
