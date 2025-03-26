#include "Boss.h"

Boss* Boss::create() {
    Boss* boss = new (std::nothrow) Boss();
    if (boss && boss->initWithFile("enemy.png")) {
        boss->autorelease();
        boss->setContentSize(Size(80, 80));
        boss->scheduleUpdate();
        return boss;
    }
    delete boss;
    return nullptr;
}

void Boss::setTarget(Sprite* target) {
    targetPlayer = target;
    patrolCenter = getPosition();
}

void Boss::update(float dt) {
    if (!targetPlayer) return;

    Vec2 playerPos = targetPlayer->getPosition();
    Vec2 bossPos = getPosition();

    float distance = playerPos.distance(bossPos);

    if (distance <= 500) {
        Vec2 direction = (playerPos - bossPos).getNormalized();
        setPosition(bossPos + direction * speed * dt);
    }
    else {
        setPosition(bossPos);
    }

    if (playerPos.x > this->getPosition().x) {
        this->setFlippedX(true);
    }
    else {
        this->setFlippedX(false);
    }
}

