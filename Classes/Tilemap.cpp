#include "TileMap.h"
#include "2d/CCTMXTiledMap.h"

TileMap* TileMap::create() {
    TileMap* map = new (std::nothrow) TileMap();
    if (map && map->init()) {
        map->autorelease();
        return map;
    }
    delete map;
    return nullptr;
}

bool TileMap::init() {
    if (!TMXTiledMap::initWithTMXFile("tilemap.tmx")) {
        return false;
    }
    return true;
}