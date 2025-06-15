#include "Meteor.h"

Model Meteor::model[8];
bool Meteor::Loaded = false;

void Meteor::LoadModels() {
    if (!Loaded) {
        model[0] = LoadModelFromMesh(GenMeshPlane(2, 2, 4, 3));
        model[1] = LoadModelFromMesh(GenMeshCube(2.0f, 1.0f, 2.0f));
        model[2] = LoadModelFromMesh(GenMeshSphere(2, 32, 32));
        model[3] = LoadModelFromMesh(GenMeshHemiSphere(2, 16, 16));
        model[4] = LoadModelFromMesh(GenMeshCylinder(1, 2, 16));
        model[5] = LoadModelFromMesh(GenMeshTorus(0.25f, 4.0f, 16, 32));
        model[6] = LoadModelFromMesh(GenMeshKnot(1.0f, 2.0f, 16, 128));
        model[7] = LoadModelFromMesh(GenMeshPoly(5, 2.0f));
        Loaded = true;
    }
}
void Meteor:: Draw_Trail() const {
    if (trail) {
        for (int i = 1; i < Trail.size(); i++) {
            DrawLine3D(Trail[i - 1], Trail[i], Body_Color);
        }
    }
}

Meteor::Meteor(String n, Vector3 pos, Vector3 vel, float mass, float radius, Color body_color)
    : CelestialBody(n, pos, vel, mass, radius, body_color)  {

    Max_Trail_Length = 500;
    trail = true;
    if (!Loaded) LoadModels(); 
    meteor_model = model[rand() % 8];
    meteor_model.materials[0].maps[MATERIAL_MAP_DIFFUSE].color = body_color;
}
void Meteor::toggle() {
    trail = !trail;
}
Meteor::~Meteor() {}

void Meteor::Update_Position(float delta_time) {
    Vel = Vector3Add(Vel, Vector3Scale(Acc, delta_time));
    Pos = Vector3Add(Pos, Vector3Scale(Vel, delta_time));

    Trail.push(Pos);
    if (Trail.size() > Max_Trail_Length) Trail.delete_at(0);
}

void Meteor::Draw_Body() const {
    DrawModel(meteor_model, Pos, 1.0f, WHITE);
}
