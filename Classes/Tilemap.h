#pragma once
#include "cocos2d.h"
using namespace cocos2d;

class TileMap : public cocos2d::TMXTiledMap {
public:
    static TileMap* create();
    virtual bool init();
};