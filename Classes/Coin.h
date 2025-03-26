#pragma once
#include "cocos2d.h"
using namespace cocos2d;

class Player;

class Coin : public Sprite {
public:
    static Coin* create();
    void applyEffect(Player* player);
};