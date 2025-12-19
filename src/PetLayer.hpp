#pragma once
#include <Geode/Geode.hpp>

class PetLayer : public cocos2d::CCLayer {
public:
    static PetLayer* create();
    bool init() override;

    void onBack(cocos2d::CCObject*);
    void keyBackClicked() override;
    void update(float dt) override;
    void onSettingsBtn(cocos2d::CCObject* sender);
    void onUpgradeBtn(cocos2d::CCObject* sender);
    void onRenameBtn(cocos2d::CCObject* sender);

private:
    cocos2d::CCSprite* m_bg1 = nullptr;
    cocos2d::CCSprite* m_bg2 = nullptr;
    float m_bgWidth = 0.f;
    float m_speed = 30.f;

    std::string m_petName = "Grinding Pet";
    cocos2d::CCLabelBMFont* m_titleLabel = nullptr;
};
