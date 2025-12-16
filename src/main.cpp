#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>

using namespace geode::prelude;

class $modify(PetMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;
        
        auto rightMenu = getChildByIDRecursive("right-side-menu");

        auto spr = CCSprite::createWithSpriteFrameName("GJ_plainBtn_001.png");

        auto btn = CCMenuItemSpriteExtra::create(spr, this, menu_selector(PetMenuLayer::onPetButton));

        rightMenu->addChild(btn);
        rightMenu->updateLayout();

        return true;
    };

    void onPetButton(CCObject* sender) {
        Notification::create("Callback :)", NotificationIcon::Success)->show();
    }
};