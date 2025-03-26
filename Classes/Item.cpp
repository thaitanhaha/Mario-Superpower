#include "Item.h"
#include "Player.h"

Item* Item::create() {
    Item* item = new Item();
    if (item && item->initWithFile("item.png")) {
        item->autorelease();
        item->setContentSize(Size(30, 30));
        return item;
    }
    delete item;
    return nullptr;
}

void Item::applyEffect(Player* player) {
    // Define effect logic here
}