#ifndef HUD_H
#define HUD_H

#include "cocos2d.h"

class HUD : public cocos2d::Node {
public:
    static HUD* create();
    void setScore(int increase);
    void setHP(int hp);

    virtual bool init();
    cocos2d::Label* scoreLabel;
    int score;
    std::vector<cocos2d::Sprite*> hearts;
};

#endif // HUD_H