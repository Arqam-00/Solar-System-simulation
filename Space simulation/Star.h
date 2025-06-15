#ifndef STAR_H
#define STAR_H

#include "Planet.h"
#include "Meteor.h"
#include "Dust.h"
class Star : public CelestialBody
{
private:
    Dynamic_array<Planet*> Planets;
    float Intensity_Of_Light;
    float temperature;
    Shader Shine_Shader;
    int centerLoc, resolutionLoc, colorLoc, radiusLoc, glowLoc;


public:
    Star(String name, Vector3 p, Vector3 v, float m, float r, Color color);
    Star(String name, Vector3 p, Vector3 v, float m, float r, Color color, float light, float heat);
    void Add_Planet(Planet* planet);

    void Place_Planet_In_Orbit(Planet* planet, float distance_multiplier);
    void Check_Escape(Dynamic_array<CelestialBody*>& free_bodies);

    int Get_Number_Of_Planets();
    Planet* Get_Planet_At(int index);
    
    void CheckDelete(CelestialBody* B);
    void Shine_Draw(Camera3D camera, int screenWidth, int screenHeight);
    void Add_Asteroid_Belt(int num_asteroid, float min_distance, float max_distance, Dynamic_array<CelestialBody*>& bodies);

    };



#endif
