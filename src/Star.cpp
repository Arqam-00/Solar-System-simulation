#include "Star.h"

Star::Star(String name, Vector3 p, Vector3 v, float m, float r, Color color)
    : CelestialBody(name, p, v, m, r, color), Intensity_Of_Light(10), temperature(1000)
    , Shine_Shader(LoadShader(0, "assets/Star.fs"))
    , centerLoc(GetShaderLocation(Shine_Shader, "center"))
    , resolutionLoc(GetShaderLocation(Shine_Shader, "resolution"))
    , colorLoc(GetShaderLocation(Shine_Shader, "starColor"))
    , radiusLoc(GetShaderLocation(Shine_Shader, "radius"))
    , glowLoc(GetShaderLocation(Shine_Shader, "glowStrength"))
    , Rotation_Axis({ 0.0f,1.0f,0.0f })
    ,Rotation_Vel(20.0f){}
Star::Star(String name, Vector3 p, Vector3 v, float m, float r, Color color, float light, float heat)
    : CelestialBody(name, p, v, m, r, color), Intensity_Of_Light(light), temperature(heat)
    , Shine_Shader(LoadShader(0, "assets/Star.fs"))
    , centerLoc(GetShaderLocation(Shine_Shader, "center"))
    , resolutionLoc(GetShaderLocation(Shine_Shader, "resolution"))
    , colorLoc(GetShaderLocation(Shine_Shader, "starColor"))
    , radiusLoc(GetShaderLocation(Shine_Shader, "radius"))
    , glowLoc(GetShaderLocation(Shine_Shader, "glowStrength"))
    , Rotation_Axis({ 0.0f,1.0f,0.0f })
    , Rotation_Vel(20.0f){}
Star::Star(String name, Vector3 p, Vector3 v, float m, float r, Color color,Vector3 rot_axis,float rot_vel)
    : CelestialBody(name, p, v, m, r, color), Intensity_Of_Light(10), temperature(1000)
    , Shine_Shader(LoadShader(0, "assets/Star.fs"))
    , centerLoc(GetShaderLocation(Shine_Shader, "center"))
    , resolutionLoc(GetShaderLocation(Shine_Shader, "resolution"))
    , colorLoc(GetShaderLocation(Shine_Shader, "starColor"))
    , radiusLoc(GetShaderLocation(Shine_Shader, "radius"))
    , glowLoc(GetShaderLocation(Shine_Shader, "glowStrength"))
    , Rotation_Axis(rot_axis)
    , Rotation_Vel(rot_vel) {
}
Star::Star(String name, Vector3 p, Vector3 v, float m, float r, Color color, float light, float heat, Vector3 rot_axis, float rot_vel)
    : CelestialBody(name, p, v, m, r, color), Intensity_Of_Light(light), temperature(heat)
    , Shine_Shader(LoadShader(0, "assets/Star.fs"))
    , centerLoc(GetShaderLocation(Shine_Shader, "center"))
    , resolutionLoc(GetShaderLocation(Shine_Shader, "resolution"))
    , colorLoc(GetShaderLocation(Shine_Shader, "starColor"))
    , radiusLoc(GetShaderLocation(Shine_Shader, "radius"))
    , glowLoc(GetShaderLocation(Shine_Shader, "glowStrength"))
    , Rotation_Axis(rot_axis)
    , Rotation_Vel(rot_vel) {
}

void Star::Add_Planet(Planet* planet)
{
    Planets.push(planet);
}

void Star::Place_Planet_In_Orbit(Planet* planet, float distance_multiplier)
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


void Star::Check_Escape(Dynamic_array<CelestialBody*>& free_bodies)
{
    for (int i = 0; i < Planets.size(); )
    {
        float dist = Vector3Distance(Pos, Planets[i]->Get_Position());

        float escape_velocity = sqrt((2.0f * G * Mass) / dist);
        Vector3 rel_velocity = Vector3Subtract(Planets[i]->Get_Velocity(), Vel);
        float current_speed = Vector3Length(rel_velocity);

        if (current_speed > escape_velocity * 2)
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


int Star::Get_Number_Of_Planets() { return Planets.size(); }
Planet* Star::Get_Planet_At(int index) { return Planets[index]; }
void Star::Draw_Body() const {
    if (Textured) {
        float Rotation = GetTime() * Rotation_Vel;
        Matrix transform = MatrixMultiply(
            MatrixRotate(Rotation_Axis, Rotation * DEG2RAD),
            MatrixTranslate(Pos.x, Pos.y, Pos.z)
        );
        DrawModelEx(SphereModel, Pos, Rotation_Axis, Rotation, { Radius, Radius, Radius }, WHITE);
    }
    else {
        DrawSphere(Pos, Radius, Body_Color);
    }

}
void Star::CheckDelete(CelestialBody* B) {
    for (int i = Planets.size() - 1; i >= 0; i--) {
        if (Planets[i] == B) {
            Planets.delete_at(i);

        }
        else {
            (Planets[i]->CheckDelete(B));
        }

    }

}
void Star::Shine_Draw(Camera3D camera, int screenWidth, int screenHeight)
{

    Vector2 center = GetWorldToScreenEx(Pos, camera, screenWidth, screenHeight);

    center.y = (float)screenHeight - center.y;

    Vector2 resolution = { (float)screenWidth, (float)screenHeight };

    float starRadius = Radius * 0.8f;
    float glowStrength = Radius * 2.0f;

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
void Star::Add_Asteroid_Belt(int num_asteroid, float min_distance, float max_distance, Dynamic_array<CelestialBody*>& bodies)
{
    Vector3 orbit_axis = { 0, 1, 0 };

    for (int i = 0; i < num_asteroid; i++)
    {
        float distance = min_distance + ((float)GetRandomValue(0, 10000) / 10000.0f) * (max_distance - min_distance);

        float inclination = DEG2RAD * GetRandomValue(-5, 5);
        float phase = DEG2RAD * GetRandomValue(0, 360);

        Vector3 reference = (fabs(orbit_axis.y) < 0.99f) ? Vector3{ 1, 0, 0 } : Vector3{ 0, 0, 1 };
        Vector3 base_direction = Vector3Normalize(Vector3CrossProduct(orbit_axis, reference));

        Matrix phase_rot = MatrixRotate(orbit_axis, phase);
        Vector3 orbit_direction = Vector3Transform(base_direction, phase_rot);

        Vector3 incl_axis = Vector3Normalize(Vector3CrossProduct(orbit_axis, orbit_direction));
        Matrix incl_rot = MatrixRotate(incl_axis, inclination);
        orbit_direction = Vector3Transform(orbit_direction, incl_rot);

        Vector3 asteroid_pos = Vector3Add(this->Pos, Vector3Scale(orbit_direction, distance));

        float speed = sqrt(G * (this->Mass) / distance);
        Vector3 tangent = Vector3Normalize(Vector3CrossProduct(orbit_axis, orbit_direction));
        Vector3 vel = Vector3Add(this->Vel, Vector3Scale(tangent, speed));

        bodies.push(new Meteor("Asteroid" + String(i), asteroid_pos, vel, GetRandomValue(0.1f, 5.0f), GetRandomValue(1.0f, 3.0f), DARKGRAY));
    }

}


void Star::Draw_Info_Box() const {
    Vector2 mousePos = GetMousePosition();
    float boxWidth = 250;
    float boxHeight = 200;

    DrawRectangle(mousePos.x + 10, mousePos.y + 10, boxWidth, boxHeight, Fade(GRAY, 0.7f));
    DrawText(Name.C_Str(), mousePos.x + 20, mousePos.y + 20, 20, WHITE);
    DrawText(TextFormat("Mass: %.2f", Mass), mousePos.x + 20, mousePos.y + 40, 20, WHITE);
    DrawText(TextFormat("Radius: %.2f", Radius), mousePos.x + 20, mousePos.y + 60, 20, WHITE);
    DrawText(TextFormat("Coords: X: %.2f Y: %.2f Z: %.2f", Pos.x, Pos.y, Pos.z), mousePos.x + 20, mousePos.y + 80, 20, WHITE);
    DrawText(TextFormat("Planets: %d", Planets.size()), mousePos.x + 20, mousePos.y + 100, 20, WHITE);
    DrawText(TextFormat("Light Intensity: %.2f", Intensity_Of_Light), mousePos.x + 20, mousePos.y + 120, 20, WHITE);
    DrawText(TextFormat("Temperature: %.2f K", temperature), mousePos.x + 20, mousePos.y + 140, 20, WHITE);
}
