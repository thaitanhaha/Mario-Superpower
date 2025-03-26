#pragma once
#include "cocos2d.h"
#include "Player.h"
#include "Enemy.h"
#include "Item.h"
#include "Coin.h"
#include "TileMap.h"
#include "Boss.h"
#include "HUD.h"

using namespace cocos2d;

class GameScene : public Scene {
public:
    static Scene* createScene(bool mute);
    virtual bool init();
    CREATE_FUNC(GameScene);

    void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
    void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
    void setMuted(bool mute) { isMuted = mute; }
    bool getMuted() { return isMuted; }

private:
    Player* player;
    TileMap* tileMap;
    HUD* hud;
    Vector<Enemy*> enemies;
    Vector<Boss*> bosses;
    Vector<Item*> items;
    Vector<Coin*> coins;
    bool isMuted;

    void update(float dt);
    void spawnEnemies(float dt);
    void spawnBosses(float dt);
    void spawnItems(float dt);
    void spawnCoins(float dt);
    void checkCollisions();
    bool onTouchBegan(Touch* touch, Event* event);

    void back(Ref* pSender);

    std::map<EventKeyboard::KeyCode, bool> keys;
};