#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

class PetShop : public Popup<std::string const&> {
    public:
    bool setup(std::string const& value) override;
    static PetShop* create();
    void onDecoBtn(cocos2d::CCObject* sender);
    void onEffectsBtn(cocos2d::CCObject* sender);
    void onRepliesBtn(cocos2d::CCObject* sender);
    void selectTab(int id);
    TabButton* m_decoBtn;
    TabButton* m_effectsBtn;
    TabButton* m_repliesBtn;
    CCLayer* mainLayer;
};