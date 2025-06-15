#include "Moon.h"

Moon::Moon() :CelestialBody(), Rotation_Axis({ 0.0f,1.0f,0.0f })
, Rotation_Vel(20),trail(1) {
    Max_Trail_Length = 550;
}
Moon::Moon(String n, Vector3 Pos, Vector3 Vel, float Mass, float Radius, Color clr)
    : CelestialBody(n, Pos, Vel, Mass, Radius, clr), Rotation_Axis({0.0f,1.0f,0.0f})
    , Rotation_Vel(20), trail(1) {
    Max_Trail_Length = 550;
}
Moon::Moon(String n, Vector3 Pos, Vector3 Vel, float Mass, float Radius, Color clr, Vector3 rot_axis, float rot_Vel)
    : CelestialBody(n, Pos, Vel, Mass, Radius, clr), Rotation_Axis(rot_axis)
    , Rotation_Vel(rot_Vel), trail(1) {
    Max_Trail_Length = 550;
}
void Moon::Toggle_Trail() {
    trail = !trail;
}
void Moon::Draw_Trail() const  {
    if (trail) {
        for (int i = 1; i < Trail.size(); i++) {
            DrawLine3D(Trail[i - 1], Trail[i], Body_Color);
        }
    }
}
void Moon::Draw_Body() const {
    if (Textured) {
        float Rotation = GetTime() * Rotation_Vel;
        Matrix transform = MatrixMultiply(
            MatrixRotate(Rotation_Axis, Rotation * DEG2RAD),
            MatrixTranslate(Pos.x, Pos.y, Pos.z)
        );
        DrawModelEx(SphereModel, Pos, Rotation_Axis, Rotation, { 1.0f, 1.0f, 1.0f }, WHITE);
    }
    else {
        DrawSphere(Pos, Radius, Body_Color);
    }

}
Moon:: ~Moon() {}