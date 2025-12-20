#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include "PetLayer.hpp"
#include "PetUtils.hpp"

using namespace geode::prelude;

$on_mod(Loaded) {
    if (!Mod::get()->getSavedValue<int>("current-stars") && !Mod::get()->getSavedValue<int>("current-moons")) {
        PetUtils::getTotalStats();
    }
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

    void waitReady(float) {
    if (!PetUtils::is_ready1 || !PetUtils::is_ready2) return;

    this->unschedule(schedule_selector(PetMenuLayer::waitReady));

    auto scene = CCScene::create();
    auto layer = PetLayer::create();
    scene->addChild(layer);

    auto transition = CCTransitionFade::create(0.5f, scene);

    CCDirector::sharedDirector()->pushScene(transition);
}

    void onPetButton(CCObject* sender) {
        PetUtils::getUser();
        PetUtils::newCreateUser();
        PetUtils::checkStats();

        this->schedule(schedule_selector(PetMenuLayer::waitReady), 0.2f);
    }
};