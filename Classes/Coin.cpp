#include "Coin.h"
#include "Player.h"

Coin* Coin::create() {
    Coin* coin = new Coin();
    if (coin && coin->initWithFile("coin.png")) {
        coin->autorelease();
        coin->setContentSize(Size(30, 30));
        return coin;
    }
    delete coin;
    return nullptr;
}

void Coin::applyEffect(Player* player) {
    // Define effect logic here
}