#pragma once
#include "cocos2d.h"
using namespace cocos2d;

class Player;

class Item : public Sprite {
public:
    static Item* create();
    void applyEffect(Player* player);
};