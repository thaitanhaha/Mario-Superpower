#pragma once
#include "cocos2d.h"
using namespace cocos2d;

class Enemy : public Sprite {
public:
    static Enemy* create();
    void update(float dt);
    void setTarget(Sprite* target);

private:
    Sprite* targetPlayer = nullptr;
    Vec2 patrolCenter;
    float patrolSize = 50.0f;
    float speed = 100.0f;
};
