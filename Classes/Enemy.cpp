#include "Enemy.h"

Enemy* Enemy::create() {
    Enemy* enemy = new (std::nothrow) Enemy();
    if (enemy && enemy->initWithFile("enemy.png")) {
        enemy->autorelease();
        enemy->setContentSize(Size(50, 50));
        enemy->scheduleUpdate();
        return enemy;
    }
    delete enemy;
    return nullptr;
}

void Enemy::setTarget(Sprite* target) {
    targetPlayer = target;
    patrolCenter = getPosition();
}

void Enemy::update(float dt) {
    if (!targetPlayer) return;

    Vec2 playerPos = targetPlayer->getPosition();
    Vec2 enemyPos = getPosition();

    float distance = playerPos.distance(enemyPos);

    if (distance <= 500) {
        Vec2 direction = (playerPos - enemyPos).getNormalized();
        setPosition(enemyPos + direction * speed * dt);
    }
    else {
        setPosition(enemyPos);
    }

    if (playerPos.x > this->getPosition().x) {
        this->setFlippedX(true);
    }
    else {
        this->setFlippedX(false);
    }
}

