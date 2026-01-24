#include <Geode/Geode.hpp>
#include <Geode/modify/CreatorLayer.hpp>
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

class $modify(PetCreatorLayer, CreatorLayer) {
    bool init() {
        if (!CreatorLayer::init()) return false;

        auto menu = CCMenu::create();
        menu->setID("pet-menu");

        auto layout = ColumnLayout::create();
        layout->setAutoGrowAxis(true);
        layout->setAxisAlignment(AxisAlignment::Center);

        menu->setLayout(layout);
        this->addChild(menu);

        auto btnSprTop = CCSprite::create("player.png"_spr);
        btnSprTop->setID("pet-btn-spr-top");

        auto btnSpr = CircleButtonSprite::create(btnSprTop);
        btnSpr->setID("pet-btn-spr");
        btnSpr->setScale(0.9f);

        auto btn = CCMenuItemSpriteExtra::create(
            btnSpr,
            this,
            menu_selector(PetCreatorLayer::onPetBtn)
        );
        btn->setID("pet-btn");
        menu->setPositionX(23.f);
        btnSprTop->setScale(0.85f);
        menu->addChild(btn);
        menu->updateLayout();

        return true;
    }

    void onPetBtn(CCObject* sender) {
        if (Mod::get()->getSavedValue<int>("is-banned") == 1) {
            FLAlertLayer::create("Banned", fmt::format("You have been <cj>banned from Grinding Pet</c>, if you believe it is a mistake, <cy>appeal</c> in our <cp>Discord Server.</c>\n<cy>Ban reason:</c> <cp>{}</c>.", Mod::get()->getSavedValue<std::string>("ban-reason")), "OK")->show();
            return;
        }
        auto scene = CCScene::create();
        auto layer = PetLayer::create();
        scene->addChild(layer);

        auto transition = CCTransitionFade::create(0.5f, scene);
        CCDirector::sharedDirector()->pushScene(transition);
    }
};