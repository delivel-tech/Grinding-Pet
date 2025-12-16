#pragma once
#include <geode/Geode.hpp>
using namespace geode::prelude;

class PetLayer : public CCLayer {
public:
	static PetLayer* create();
	bool init() override;

	void onBack(CCObject*);
	void keyBackClicked() override;
	void update(float dt) override;
	void onSettingsBtn(CCObject* sender);
	void onUpgradeBtn(CCObject* sender);
	void onRenameBtn(CCObject* sender);
private:
	CCSprite* m_bg1 = nullptr;
	CCSprite* m_bg2 = nullptr;
	float m_bgWidth = 0.f;
	float m_speed = 30.f;
};