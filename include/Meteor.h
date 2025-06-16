#ifndef METEOR_H
#define METEOR_H

#include "CelestialBody.h"
#include "raylib.h"
#include "raymath.h"

class Meteor : public CelestialBody {
private:
    static Model model[8];
    static bool Loaded;
    bool trail;
    int model_index;

public:
    Meteor(String n, Vector3 pos, Vector3 vel, float mass, float radius, Color body_color);
    ~Meteor();

    void Draw_Trail() const override;
    void toggle();
    void Update_Position(float delta_time) override;
    void Draw_Body() const override;

    static void LoadModels();
    static void UnloadModels();
};

#endif
