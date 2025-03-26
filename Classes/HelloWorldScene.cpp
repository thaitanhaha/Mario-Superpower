/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "HelloWorldScene.h"
#include "../proj.win32/GameScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto closeItem = MenuItemImage::create(
        "CloseNormal.png",
        "CloseSelected.png",
        CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
        float y = origin.y + closeItem->getContentSize().height / 2;
        closeItem->setPosition(Vec2(x, y));
    }

    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    if (label == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        label->setPosition(Vec2(origin.x + visibleSize.width / 2,
            origin.y + visibleSize.height - label->getContentSize().height));
        this->addChild(label, 1);
    }

    auto play = MenuItemLabel::create(Label::createWithTTF("PLAY", "fonts/Marker Felt.ttf", 40), CC_CALLBACK_1(HelloWorld::play, this));
    CCASSERT(play != nullptr, "problem loading fonts/Marker Felt.ttf");

    play->setPosition((Vec2(visibleSize) - origin) / 2);
    menu->addChild(play);

    muteLabel = Label::createWithTTF("MUTE", "fonts/Marker Felt.ttf", 30);
    muteButton = MenuItemLabel::create(muteLabel, CC_CALLBACK_1(HelloWorld::toggleMute, this));
    muteButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 3));
    menu->addChild(muteButton);

    auto about = MenuItemLabel::create(Label::createWithTTF("ABOUT", "fonts/Marker Felt.ttf", 30), CC_CALLBACK_1(HelloWorld::showAbout, this));
    about->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 4));
    menu->addChild(about);

    return true;
}

void HelloWorld::toggleMute(Ref* pSender) {
    isMuted = !isMuted;
    if (isMuted) {
        muteLabel->setString("UNMUTE");
    }
    else {
        muteLabel->setString("MUTE");
    }
}

void HelloWorld::showAbout(Ref* pSender) {
    auto aboutLayer = LayerColor::create(Color4B(0, 0, 0, 255));
    this->addChild(aboutLayer, 2);

    auto infoLabel = Label::createWithTTF("Group Name: Chetcom\n\nNguyen Thai Tan - 2112256\n\nPham Duc Hao - 2111128\n\n\n", "fonts/Marker Felt.ttf", 24);
    infoLabel->setPosition(Director::getInstance()->getVisibleSize() / 2);
    aboutLayer->addChild(infoLabel);

    auto closeButton = MenuItemLabel::create(Label::createWithTTF("CLOSE", "fonts/Marker Felt.ttf", 24), [aboutLayer](Ref* sender) {
        aboutLayer->removeFromParent();
        });
    closeButton->setPosition(Vec2(infoLabel->getPosition().x, infoLabel->getPosition().y - 50));

    auto aboutMenu = Menu::create(closeButton, NULL);
    aboutMenu->setPosition(Vec2::ZERO);
    aboutLayer->addChild(aboutMenu);
}

void HelloWorld::play(Ref* pSender) {
    Director::getInstance()->replaceScene(TransitionFade::create(0.5, GameScene::createScene(isMuted), 
        Color3B(0, 255, 255)));
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();
}
