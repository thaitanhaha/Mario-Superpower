#pragma once
#include "cocos2d.h"
using namespace cocos2d;

class Player : public Sprite {
public:
    static Player* create();
    virtual bool init();
    void move(float dx, float dy);
    void setSpeedMultiplier(float multiplier, float duration);
    float getSpeedMultiplier() const { return speedMultiplier; }
    void updateEnergyBallSkill(float duration);

    std::vector<Sprite*>& getEnergyBalls() { return energyBalls; }

    float speedMultiplier = 1.0f;
    std::vector<Sprite*> energyBalls;
    void createEnergyBalls(float radius, int numBalls, float rotate_duration);
    void updateEnergyBallMovement(Vec2 pos);

    Node* energyBallContainer = nullptr;

    int hp = 3;

    Sprite* energyAura = nullptr;
    bool hasEnergyAura = false;
    bool doubleCheck = false;

    void createEnergyAura();
    void activateEnergy();
};
