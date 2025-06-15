#ifndef METEOR_H
#define METEOR_H

#include "CelestialBody.h"
#include "raylib.h"
#include "raymath.h"



class Meteor : public CelestialBody
{
private:
    Model meteor_model;
    int i = 0;
    inline Mesh Generate_Deformed_Meteor_Mesh(float radius, int rings, int slices, float deform_strength)
    {
        Model models[8] = { 0 };
        i++;
        models[0] = LoadModelFromMesh(GenMeshPlane(2, 2, 4, 3));
        models[1] = LoadModelFromMesh(GenMeshCube(2.0f, 1.0f, 2.0f));
        models[2] = LoadModelFromMesh(GenMeshSphere(2, 32, 32));
        models[3] = LoadModelFromMesh(GenMeshHemiSphere(2, 16, 16));
        models[4] = LoadModelFromMesh(GenMeshCylinder(1, 2, 16));
        models[5] = LoadModelFromMesh(GenMeshTorus(0.25f, 4.0f, 16, 32));
        models[6] = LoadModelFromMesh(GenMeshKnot(1.0f, 2.0f, 16, 128));
        models[7] = LoadModelFromMesh(GenMeshPoly(5, 2.0f));
        if (i == 8) {
            i = 0;
        }
        return models[i].meshes[2];
    }
public:
    Meteor(String n, Vector3 pos, Vector3 vel, float mass, float radius, Color body_color)
        : CelestialBody(n, pos, vel, mass, radius, body_color)
    {
        Max_Trail_Length = 500;

        Mesh mesh = Generate_Deformed_Meteor_Mesh(radius, 16, 16, 9.5f);
        meteor_model = LoadModelFromMesh(mesh);
        meteor_model.materials[0].maps[MATERIAL_MAP_DIFFUSE].color = body_color;
    }

    void Update_Position(float delta_time) override
    {
        Vel = Vector3Add(Vel, Vector3Scale(Acc, delta_time));
        Pos = Vector3Add(Pos, Vector3Scale(Vel, delta_time));

        Trail.push(Pos);
        if (Trail.size() > Max_Trail_Length)
            Trail.delete_at(0);
    }

    void Draw_Body() const override
    {
        DrawModel(meteor_model, Pos, 1.0f, WHITE);
    }
};

#endif
