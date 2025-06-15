#ifndef METEOR_H
#define METEOR_H

#include "CelestialBody.h"
#include "raylib.h"
#include "raymath.h"

class Meteor : public CelestialBody {
private:
    Model meteor_model;
    static Model model[8]; 
    static bool Loaded; //just need to em once 
    bool trail;
public:
    Meteor(String n, Vector3 pos, Vector3 vel, float mass, float radius, Color body_color);

    ~Meteor();
    void Draw_Trail() const override;
    void toggle();
    static void LoadModels(); 
    friend class Star;
    void Update_Position(float delta_time) override;
    void Draw_Body() const override;
};

#endif
