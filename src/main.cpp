#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include "layers/PetLayer.hpp"
#include "utils/PetUtils.hpp"
#include <argon/argon.hpp>
#include <Geode/utils/coro.hpp>

using namespace geode::prelude;

$on_mod(Loaded) {
    if (!Mod::get()->getSavedValue<int>("current-stars") && !Mod::get()->getSavedValue<int>("current-moons")) {
        PetUtils::getTotalStats();
    }

    auto res = argon::startAuth([](Result<std::string> res) {
        if (!res) {
            log::warn("argon auth failed");
            return;
        }

        std::string token = std::move(res).unwrap();
        Mod::get()->setSavedValue("argon-token", token);
        log::info("argon token OK");
        coro::spawn << PetLayer::runSyncFlow();
    }, [](argon::AuthProgress progress) {
        log::info("Auth progress: {}", argon::authProgressToString(progress));
    });
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