#include "Moon.h"

Moon::Moon() :CelestialBody(), Environment("unknown"), trail(1) {
    Max_Trail_Length = 550;
}
Moon::Moon(String n, Vector3 Pos, Vector3 Vel, float Mass, float Radius, Color clr)
    : CelestialBody(n, Pos, Vel, Mass, Radius, clr), Environment("unknown"), trail(1) {
    Max_Trail_Length = 550;
}
Moon::Moon(String n, Vector3 Pos, Vector3 Vel, float Mass, float Radius, Color clr, String env)
    : CelestialBody(n, Pos, Vel, Mass, Radius, clr), Environment(env), trail(1) {
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
void Moon::Draw_Body() const  {
    DrawSphere(Pos, Radius, Body_Color);
}
Moon:: ~Moon() {}