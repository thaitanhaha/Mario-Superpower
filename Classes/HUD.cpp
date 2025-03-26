#include "HUD.h"

HUD* HUD::create() {
    HUD* hud = new (std::nothrow) HUD();
    if (hud && hud->init()) {
        hud->autorelease();
        return hud;
    }
    delete hud;
    return nullptr;
}

bool HUD::init() {
    if (!Node::init()) {
        return false;
    }
    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    score = 0;
    scoreLabel = cocos2d::Label::createWithTTF("0", "fonts/arial.ttf", 24);
    scoreLabel->setPosition(cocos2d::Vec2(359, 213));
    this->addChild(scoreLabel);

    for (int i = 0; i < 3; i++) {
        auto heart = cocos2d::Sprite::create("heart.png");
        heart->setContentSize(cocos2d::Size(32, 32));
        heart->setPosition(cocos2d::Vec2(319 + i * 40, 313));
        this->addChild(heart);
        hearts.push_back(heart);
    }

    return true;
}

void HUD::setScore(int increase) {
    score += increase;
    scoreLabel->setString(std::to_string(score));
}

void HUD::setHP(int hp) {
    for (size_t i = 0; i < hearts.size(); i++) {
        if (i < hp) {
            hearts[i]->setVisible(true);
        }
        else {
            hearts[i]->setVisible(false);
        }
    }
}

