#include "Player.h"

Player* Player::create() {
    Player* player = new (std::nothrow) Player();
    if (player && player->init()) {
        player->autorelease();
        return player;
    }
    delete player;
    return nullptr;
}

bool Player::init() {
    if (!Sprite::initWithFile("player.png")) return false;

    this->setContentSize(Size(50, 50));

    auto delay = DelayTime::create(10.0f);
    auto createAura = CallFunc::create([this]() { createEnergyAura(); });
    this->runAction(Sequence::create(delay, createAura, nullptr));

    return true;
}


void Player::move(float dx, float dy) {
    setPosition(getPosition() + Vec2(dx, dy));
    updateEnergyBallMovement(getPosition());
}

void Player::setSpeedMultiplier(float multiplier, float duration) {
    speedMultiplier = multiplier;

    auto delay = DelayTime::create(duration);
    auto resetSpeed = CallFunc::create([this]() { speedMultiplier = 1.0f; });
    runAction(Sequence::create(delay, resetSpeed, nullptr));
}

void Player::createEnergyBalls(float radius=80.0f, int numBalls=3, float rotate_duration=2.0f) {
    if (energyBallContainer) {
        energyBallContainer->removeFromParent();
        energyBallContainer = nullptr;
    }

    energyBallContainer = Node::create();
    getParent()->addChild(energyBallContainer);
    energyBallContainer->setPosition(getPosition());

    energyBalls.clear();

    for (int i = 0; i < numBalls; ++i) {
        auto ball = Sprite::create("energy_ball.png");
        ball->setContentSize(Size(20, 20));

        float angle = i * (2 * M_PI / numBalls);
        Vec2 offset(radius * cos(angle), radius * sin(angle));
        ball->setPosition(offset);

        energyBallContainer->addChild(ball);
        energyBalls.push_back(ball);
    }

    auto rotateAction = RepeatForever::create(RotateBy::create(rotate_duration, 360));
    energyBallContainer->runAction(rotateAction);
}

void Player::updateEnergyBallMovement(Vec2 pos) {
    energyBallContainer->setPosition(pos);
}

void Player::updateEnergyBallSkill(float duration) {

    createEnergyBalls(80.0f, 8, 1.5f);

    auto delay = DelayTime::create(duration);
    auto resetBalls = CallFunc::create([this]() { createEnergyBalls(); });
    runAction(Sequence::create(delay, resetBalls, nullptr));
}

void Player::createEnergyAura() {
    if (energyAura) return;
    energyAura = Sprite::create("energy_ball.png");
    energyAura->setContentSize(Size(30, 30));
    energyAura->setOpacity(100);
    energyAura->setPosition(this->getPosition());
    this->getParent()->addChild(energyAura);
    hasEnergyAura = true;
}

void Player::activateEnergy() {
    if (!hasEnergyAura) return;
    if (doubleCheck) return;
    doubleCheck = true;

    auto grow = ScaleTo::create(0.5f, 20.0f);
    auto fadeOut = FadeOut::create(0.5f);
    auto remove = CallFunc::create([this]() {
        energyAura->removeFromParent();
        energyAura = nullptr;
        hasEnergyAura = false;
        doubleCheck = false;
        auto delay = DelayTime::create(3.0f);
        auto createAura = CallFunc::create([this]() { createEnergyAura(); });
        this->runAction(Sequence::create(delay, createAura, nullptr));
     });

    energyAura->runAction(Sequence::create(grow, fadeOut, remove, nullptr));
}









