#pragma once
#include "cocos2d.h"
using namespace cocos2d;

class Boss : public Sprite {
public:
    static Boss* create();
    void update(float dt);
    void setTarget(Sprite* target);

private:
    Sprite* targetPlayer = nullptr;
    Vec2 patrolCenter;
    float patrolSize = 50.0f;
    float speed = 70.0f;
};
