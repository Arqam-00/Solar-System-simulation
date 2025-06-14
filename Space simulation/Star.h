#ifndef STAR_H
#define STAR_H

#include "Planet.h"

class Star : public CelestialBody
{
private:
    Dynamic_array<Planet*> Planets;
    float Intensity_Of_Light;
    float temperature;
    Shader Shine_Shader;
    int centerLoc, resolutionLoc, colorLoc, radiusLoc, glowLoc;


public:
    Star(String name, Vector3 p, Vector3 v, float m, float r, Color color)
        : CelestialBody(name, p, v, m, r, color)
        , Shine_Shader(LoadShader(0, "Textures&Shaders/star.fs"))
        , centerLoc(GetShaderLocation(Shine_Shader, "center"))
        , resolutionLoc(GetShaderLocation(Shine_Shader, "resolution"))
        , colorLoc(GetShaderLocation(Shine_Shader, "starColor"))
        , radiusLoc(GetShaderLocation(Shine_Shader, "radius"))
        , glowLoc(GetShaderLocation(Shine_Shader, "glowStrength")) {
    }
    Star(String name, Vector3 p, Vector3 v, float m, float r, Color color ,float light,float heat)
        : CelestialBody(name, p, v, m, r, color),Intensity_Of_Light(light),temperature(heat)
        ,Shine_Shader ( LoadShader(0, "Textures&Shaders/star.fs"))
        ,centerLoc ( GetShaderLocation(Shine_Shader, "center"))
        ,resolutionLoc ( GetShaderLocation(Shine_Shader, "resolution"))
        ,colorLoc ( GetShaderLocation(Shine_Shader, "starColor"))
        ,radiusLoc ( GetShaderLocation(Shine_Shader, "radius"))
        ,glowLoc ( GetShaderLocation(Shine_Shader, "glowStrength")){}

    void Add_Planet(Planet* planet)
    {
        Planets.push(planet);
    }

    void Place_Planet_In_Orbit(Planet* planet, float distance_multiplier)
    {
        float desired_distance = this->Get_Radius() * distance_multiplier + planet->Get_Radius();
        Vector3 orbit_axis = { 0, 0, 1 };
        const float min_separation = 20.0f;

        bool position_ok = false;

        while (!position_ok)
        {
            float inclination = DEG2RAD * GetRandomValue(-100, 100);
            float phase = DEG2RAD * GetRandomValue(0, 360);

            Vector3 reference = (fabs(orbit_axis.y) < 0.99f) ? Vector3{ 0, 1, 0 } : Vector3{ 1, 0, 0 };
            Vector3 base_direction = Vector3Normalize(Vector3CrossProduct(orbit_axis, reference));

            Matrix phase_rot = MatrixRotate(orbit_axis, phase);
            Vector3 orbit_direction = Vector3Transform(base_direction, phase_rot);

            Vector3 incl_axis = Vector3Normalize(Vector3CrossProduct(orbit_axis, orbit_direction));
            Matrix incl_rot = MatrixRotate(incl_axis, inclination);
            orbit_direction = Vector3Transform(orbit_direction, incl_rot);

            Vector3 test_pos = Vector3Add(this->Pos, Vector3Scale(orbit_direction, desired_distance));

            position_ok = true;
            for (int i = 0; i < Planets.size(); i++)
            {
                float dist = Vector3Distance(test_pos, Planets[i]->Pos);
                if (dist < min_separation + planet->Get_Radius() + Planets[i]->Get_Radius())
                {
                    position_ok = false;
                    break;
                }
            }

            if (position_ok)
            {
                planet->Pos = test_pos;

                float speed = sqrt(G * (this->Mass + planet->Mass) / desired_distance);
                Vector3 tangent = Vector3Normalize(Vector3CrossProduct(orbit_axis, orbit_direction));
                planet->Vel = Vector3Add(this->Vel, Vector3Scale(tangent, speed));

                this->Add_Planet(planet);
            }
        }
    }




    void Check_Escape(Dynamic_array<CelestialBody*>& free_bodies)
    {
        for (int i = 0; i < Planets.size(); )
        {
            float dist = Vector3Distance(Pos, Planets[i]->Get_Position());

            float escape_velocity = sqrt((2.0f * G * Mass) / dist);
            Vector3 rel_velocity = Vector3Subtract(Planets[i]->Get_Velocity(), Vel);
            float current_speed = Vector3Length(rel_velocity);

            if (current_speed > escape_velocity*2)
            {
                Planets[i]->Name += "(No longer orbiting)";
                free_bodies.push(Planets[i]);
                Planets.delete_at(i);
            }
            else
            {
                i++;
            }
        }
    }


    int Get_Number_Of_Planets() { return Planets.size(); }
    Planet* Get_Planet_At(int index) { return Planets[index]; }
    virtual ~Star() {
        UnloadShader(Shine_Shader);
        Planets.clear();

    }

    
    void CheckDelete(CelestialBody* B) {
        for (int i = Planets.size() - 1; i >=0 ;i--) {
            if (Planets[i] == B) {
                Planets.delete_at(i);

            }
            else {
                (Planets[i]->CheckDelete(B));
            }
            
        }
        
    }
    void Shine_Draw(Camera3D camera, int screenWidth, int screenHeight)
    {

        Vector2 center = GetWorldToScreenEx(Pos, camera, screenWidth, screenHeight);

        center.y = (float)screenHeight - center.y;

        Vector2 resolution = { (float)screenWidth, (float)screenHeight };

        float starRadius = Radius * 0.5f;
        float glowStrength = Radius * 5.0f;

        Vector4 normalized = ColorNormalize(Body_Color);
        Vector3 color = { normalized.x, normalized.y, normalized.z };

        SetShaderValue(Shine_Shader, centerLoc, &center, SHADER_UNIFORM_VEC2);
        SetShaderValue(Shine_Shader, resolutionLoc, &resolution, SHADER_UNIFORM_VEC2);
        SetShaderValue(Shine_Shader, colorLoc, &color, SHADER_UNIFORM_VEC3);
        SetShaderValue(Shine_Shader, radiusLoc, &starRadius, SHADER_UNIFORM_FLOAT);
        SetShaderValue(Shine_Shader, glowLoc, &glowStrength, SHADER_UNIFORM_FLOAT);

        BeginShaderMode(Shine_Shader);
        DrawRectangle(0, 0, screenWidth, screenHeight, BLACK);
        EndShaderMode();
    }



};

#endif
