#include "CelestialBody.h"

bool CelestialBody::CheckDelete() {
    return Mass <= 0.0f;
}
CelestialBody::CelestialBody():
    Name ("none")
    ,Pos ({ 0.0f, 0.0f, 0.0f })
    ,Vel ({ 0.0f, 0.0f, 0.0f })
    ,Acc ({ 0.0f, 0.0f, 0.0f })
    ,Mass (1.0f)
    ,Radius (1.0f)
    ,Body_Color ( WHITE)
    ,Textured ( false)
    ,SphereModel(), DeformationAmount(0.0f){
}

CelestialBody::CelestialBody(String n, Vector3 Pos, Vector3 Vel, const float Mass, const float Radius, Color Body_Color):
     Name( n)
    ,Pos ( Pos)
    ,Vel ( Vel)
    ,Mass ( Mass)
    ,Radius ( Radius)
    ,Body_Color ( Body_Color)
    , Acc({ 0.0f, 0.0f, 0.0f })
    ,Textured ( false)
    ,SphereModel(){
}

CelestialBody& CelestialBody::operator=(const CelestialBody& other) {
    if (this != &other) {
        Name = other.Name;
        Pos = other.Pos;
        Vel = other.Vel;
        Acc = other.Acc;
        Mass = other.Mass;
        Radius = other.Radius;
        Body_Color = other.Body_Color;
        Trail = other.Trail;
        DeformationAmount = other.DeformationAmount;
    }
    return *this;
}
CelestialBody::~CelestialBody() {
    
    if (Textured) {
        UnloadModel(SphereModel);
    }
}

void CelestialBody::Reset_Acceleration() {
    Acc = { 0.0f, 0.0f, 0.0f };
}

void CelestialBody::Compute_Gravity_From(const CelestialBody& Other) {
    Vector3 Direction = Vector3Subtract(Other.Pos, Pos);
    float Distance_Squared = Vector3LengthSqr(Direction) + 0.000001f;
    float Force = (G * Mass * Other.Mass) / Distance_Squared;
    Direction = Vector3Normalize(Direction);
    Vector3 Acceleration_Contribution = Vector3Scale(Direction, Force / Mass);
    Acc = Vector3Add(Acc, Acceleration_Contribution);
}
void CelestialBody::Apply_Texture(Texture2D& texture) {
    if (!Textured) {
        Mesh sphereMesh = GenMeshSphere(1.0f, 64, 64);
        SphereModel = LoadModelFromMesh(sphereMesh);
        SphereModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
        UploadMesh(&sphereMesh, true);
        Textured = true;
    }
}

void CelestialBody::Update_Position(float Delta_Time) {
    Vel = Vector3Add(Vel, Vector3Scale(Acc, Delta_Time));
    Pos = Vector3Add(Pos, Vector3Scale(Vel, Delta_Time));
    float r = Radius;
    Radius = Lerp(Radius, r, 0.02f);
    DeformationAmount *= 0.98f;

    float rippleEffect = sin(GetTime() * 5.0f) * DeformationAmount * 0.5f;
    Radius += rippleEffect;

    Trail.push(Pos);
    if (Trail.size() > Max_Trail_Length) {
        Trail.delete_at(0);
    }
}

void CelestialBody::Apply_Collision_Deformation(float impactEnergy)
{
    float DeformationAmount = impactEnergy * 0.0000002f;
    if (DeformationAmount > 1.0f)
        DeformationAmount = 1.0f;

}



 void CelestialBody::Draw_Body() const {
    if (Textured) {
        DrawModel(SphereModel, Pos, 1.0f, WHITE);
    }
    else {
        DrawSphere(Pos, Radius, Body_Color);
    }
}

void CelestialBody::Draw_Trail() const {
    for (int i = 1; i < Trail.size(); i++) {
        DrawLine3D(Trail[i - 1], Trail[i], Body_Color);
    }
}
void CelestialBody::Draw_Info_Box() const {
    Vector2 mousePos = GetMousePosition();
    float boxWidth = 200;
    float boxHeight = 100;

    DrawRectangle(mousePos.x + 10, mousePos.y + 10, boxWidth, boxHeight, Fade(GRAY, 0.7f));
    DrawText(Name.C_Str(), mousePos.x + 20, mousePos.y + 20, 20, WHITE);
    DrawText(TextFormat("Mass: %.2f", Mass), mousePos.x + 20, mousePos.y + 40, 20, WHITE);
    DrawText(TextFormat("Radius: %.2f", Radius), mousePos.x + 20, mousePos.y + 60, 20, WHITE);
    DrawText(TextFormat(String("Coords: X: " + String(Pos.x) + " Y: " + String(Pos.y) + " Z: " + String(Pos.z)).C_Str()), mousePos.x + 20, mousePos.y + 80, 20, WHITE);
}

const Vector3& CelestialBody::Get_Velocity() const { return Vel; }
const float CelestialBody::Get_Radius() const { return Radius; }
const String CelestialBody::Get_Name() const { return Name; }
const Vector3& CelestialBody::Get_Position() const { return Pos; }
const float CelestialBody::Get_Mass() const { return Mass; }