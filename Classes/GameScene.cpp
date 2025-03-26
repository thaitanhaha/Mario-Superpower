#include "GameScene.h"
#include <HelloWorldScene.h>
#include "audio/include/AudioEngine.h"

Scene* GameScene::createScene(bool mute)
{
    auto scene = GameScene::create();
    scene->setMuted(mute);
    if (!scene->getMuted()) AudioEngine::play2d("background_music.mp3", true, 0.5f);
    return scene;
}

bool GameScene::init()
{
	if (!Scene::init()) return false;

	tileMap = TileMap::create();
	addChild(tileMap);

	player = Player::create();
	player->setPosition(Vec2(359, 263));
	addChild(player);

    player->createEnergyBalls(80.0f, 3, 2.0f);

    hud = HUD::create();
    hud->setPosition(Vec2(0, 0));
    addChild(hud);

	scheduleUpdate();
    schedule(CC_SCHEDULE_SELECTOR(GameScene::spawnEnemies), 1.5f);
    schedule(CC_SCHEDULE_SELECTOR(GameScene::spawnItems), 10.0f);
    schedule(CC_SCHEDULE_SELECTOR(GameScene::spawnCoins), 4.0f);
    schedule(CC_SCHEDULE_SELECTOR(GameScene::spawnBosses), 8.0f);

    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);
    listener->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto background = LayerColor::create(Color4B(255, 255, 255, 255));
	addChild(background, -1);

	//auto back = MenuItemImage::create("back1.png", "back2.png", CC_CALLBACK_1(GameScene::back, this));
	//CCASSERT(back != nullptr, "Fail to load BACK images");

	//float scaleX = 50.0f / back->getContentSize().width;
	//float scaleY = 50.0f / back->getContentSize().height;
	//back->setScale(scaleX, scaleY);

	//auto menu = Menu::create(back, nullptr);
	//this->addChild(menu);
	//menu->setPosition(origin);
	//back->setPosition(50, 526);

    auto follow = Follow::create(player, Rect(0, 0, 2304, 1728));
    this->runAction(follow);

	return true;
}

void GameScene::back(Ref* pSender) {
	auto homeScene = HelloWorld::createScene();
	Director::getInstance()->replaceScene(
		TransitionFade::create(0.5, homeScene, Color3B(0, 255, 255)));
}

void GameScene::update(float dt) {
    Vec2 movement(0, 0);
    float speed = 150.0f * dt * player->getSpeedMultiplier();

    if (keys[EventKeyboard::KeyCode::KEY_W] && player->hp > 0) movement.y += speed;
    if (keys[EventKeyboard::KeyCode::KEY_S] && player->hp > 0) movement.y -= speed;
    if (keys[EventKeyboard::KeyCode::KEY_A] && player->hp > 0) {
        movement.x -= speed;
        player->setFlippedX(true);
    }
    if (keys[EventKeyboard::KeyCode::KEY_D] && player->hp > 0) {
        movement.x += speed;
        player->setFlippedX(false);
    }

    Vec2 newPosition = player->getPosition() + movement;

    float halfWidth = player->getContentSize().width / 2;
    float halfHeight = player->getContentSize().height / 2;

    float minX = halfWidth;
    float minY = halfHeight;
    float maxX = 2304 - halfWidth;
    float maxY = 1728 - halfHeight;

    newPosition.x = clampf(newPosition.x, minX, maxX);
    newPosition.y = clampf(newPosition.y, minY, maxY);

    player->setPosition(newPosition);
    player->updateEnergyBallMovement(newPosition);

    for (auto enemy : enemies) {
        enemy->update(dt);
    }
    checkCollisions();

    Vec2 scorePosition = player->getPosition() + Vec2(0, 50);
    hud->scoreLabel->setPosition(scorePosition);

    Vec2 heartPosition = player->getPosition() - Vec2(0, 50);
    for (size_t i = 0; i < hud->hearts.size(); i++) {
        hud->hearts[i]->setPosition(heartPosition.x - 40 + i * 40, heartPosition.y);
    }

    if (player->energyAura != nullptr) {
        player->energyAura->setPosition(newPosition);
    }
}


void GameScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) {
    keys[keyCode] = true;
    if (keyCode == EventKeyboard::KeyCode::KEY_Q) {
        player->activateEnergy();
    }
}

void GameScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event) {
    keys[keyCode] = false;
}

void GameScene::spawnEnemies(float dt) {
    Enemy* enemy = Enemy::create();
    Vec2 playerPos = player->getPosition();
    Vec2 enemyPos;

    do {
        float x = RandomHelper::random_real(20.0f, 2284.0f);
        float y = RandomHelper::random_real(20.0f, 1708.0f);
        enemyPos = Vec2(x, y);
    } while (enemyPos.distance(playerPos) < 200.0f);

    enemy->setPosition(enemyPos);
    enemy->setTarget(player);

    addChild(enemy);
    enemies.pushBack(enemy);
}

void GameScene::spawnBosses(float dt) {
    Boss* boss = Boss::create();
    Vec2 playerPos = player->getPosition();
    Vec2 bossPos;

    do {
        float x = RandomHelper::random_real(20.0f, 2284.0f);
        float y = RandomHelper::random_real(20.0f, 1708.0f);
        bossPos = Vec2(x, y);
    } while (bossPos.distance(playerPos) < 200.0f);

    boss->setPosition(bossPos);
    boss->setTarget(player);

    addChild(boss);
    bosses.pushBack(boss);
}

void GameScene::spawnItems(float dt) {
    Item* item = Item::create();
    float x = RandomHelper::random_real(20.0f, 2284.0f);
    float y = RandomHelper::random_real(20.0f, 1708.0f);
    item->setPosition(Vec2(x, y));
    addChild(item);
    items.pushBack(item);
}

void GameScene::spawnCoins(float dt) {
    for (int i = 0; i < 2; i++) {
        Coin* coin = Coin::create();
        float x = RandomHelper::random_real(20.0f, 2284.0f);
        float y = RandomHelper::random_real(20.0f, 1708.0f);
        coin->setPosition(Vec2(x, y));
        addChild(coin);
        coins.pushBack(coin);
    }
}

void GameScene::checkCollisions() {
    for (auto coin : coins) {
        if (player->getBoundingBox().intersectsRect(coin->getBoundingBox())) {
            if (!getMuted()) AudioEngine::play2d("coin_sound.mp3", false, 0.5f);
            hud->setScore(1);
            coin->applyEffect(player);
            coin->removeFromParent();
            coins.eraseObject(coin);
            break;
        }
    }

    for (auto boss : bosses) {
        if (player->getBoundingBox().intersectsRect(boss->getBoundingBox())) {
            if (!getMuted()) AudioEngine::play2d("hit_sound.mp3");
            boss->removeFromParent();
            bosses.eraseObject(boss);
            player->hp = 0;
            hud->setHP(player->hp);
            if (player->hp <= 0) {
                if (!getMuted()) AudioEngine::play2d("game_over.mp3");
                this->runAction(cocos2d::Sequence::create(
                    cocos2d::DelayTime::create(1.0f),
                    cocos2d::CallFunc::create([this]() {
                        AudioEngine::stopAll();
                        auto scene = HelloWorld::createScene();
                        cocos2d::Director::getInstance()->replaceScene(scene);
                        }),
                    nullptr
                            ));
            }
            break;
        }

        if (player->hasEnergyAura && player->energyAura &&
            player->energyAura->getBoundingBox().intersectsRect(boss->getBoundingBox())) {

            if (!getMuted()) AudioEngine::play2d("hit_sound.mp3");
            boss->removeFromParent();
            bosses.eraseObject(boss);
            hud->setScore(5);
            break;
        }
    }

    for (auto enemy : enemies) {
        if (player->getBoundingBox().intersectsRect(enemy->getBoundingBox())) {
            if (!getMuted()) AudioEngine::play2d("hit_sound.mp3");
            enemy->removeFromParent();
            enemies.eraseObject(enemy);
            player->hp--;
            hud->setHP(player->hp);
            if (player->hp == 0) {
                if (!getMuted()) AudioEngine::play2d("game_over.mp3");
                this->runAction(cocos2d::Sequence::create(
                    cocos2d::DelayTime::create(1.0f),
                    cocos2d::CallFunc::create([this]() {
                        AudioEngine::stopAll();
                        auto scene = HelloWorld::createScene();
                        cocos2d::Director::getInstance()->replaceScene(scene);
                        }),
                    nullptr
                            ));
            }
            break;
        }
        if (player->hasEnergyAura && player->energyAura &&
            player->energyAura->getBoundingBox().intersectsRect(enemy->getBoundingBox())) {

            if (!getMuted()) AudioEngine::play2d("hit_sound.mp3");
            enemy->removeFromParent();
            enemies.eraseObject(enemy);
            hud->setScore(1);
            break;
        }
    }

    std::vector<Sprite*>& energyBalls = player->getEnergyBalls();
    for (auto ball : energyBalls) {
        Rect ballBox = ball->getBoundingBox();
        ballBox.origin = ball->getParent()->convertToWorldSpace(ballBox.origin);

        for (auto item : items) {
            Rect itemBox = item->getBoundingBox();
            itemBox.origin = item->getParent()->convertToWorldSpace(itemBox.origin);

            if (ballBox.intersectsRect(itemBox)) {
                if (!getMuted()) AudioEngine::play2d("hit_sound.mp3");
                int scoreIncrease = RandomHelper::random_int(1, 2);
                hud->setScore(scoreIncrease);

                int temp = RandomHelper::random_int(1, 10);
                if (temp <= 5) {
                    player->setSpeedMultiplier(2.0f, 5.0f);
                }
                else if (temp > 5 && temp <= 7) {
                    if (player->hp < 3) {
                        player->hp++;
                        hud->setHP(player->hp);
                    }
                }
                else {
                    player->updateEnergyBallSkill(5.0f);
                }

                item->applyEffect(player);
                item->removeFromParent();
                items.eraseObject(item);
                return;
            }
        }

        for (auto enemy : enemies) {
            Rect enemyBox = enemy->getBoundingBox();
            enemyBox.origin = enemy->getParent()->convertToWorldSpace(enemyBox.origin);

            if (ballBox.intersectsRect(enemyBox)) {
                if (!getMuted()) AudioEngine::play2d("hit_sound.mp3");
                enemy->removeFromParent();
                enemies.eraseObject(enemy);
                hud->setScore(1);
                return;
            }
        }
    }
}

bool GameScene::onTouchBegan(Touch* touch, Event* event) {
    Vec2 location = touch->getLocation();
    if (location.x > player->getPositionX()) {
        player->move(10, 0);
    }
    else {
        player->move(-10, 0);
    }
    return true;
}

