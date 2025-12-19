#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include "PetLayer.hpp"
#include "PetUtils.hpp"

using namespace geode::prelude;

$on_mod(Loaded) {
    PetUtils::getTotalStars();
}

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
        auto scene = CCScene::create();
        auto layer = PetLayer::create();
        scene->addChild(layer);

        auto transition = CCTransitionFade::create(0.5f, scene);

        CCDirector::sharedDirector()->pushScene(transition);
    }
};