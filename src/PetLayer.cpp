#include <Geode/Geode.hpp>
#include "PetLayer.hpp"

using namespace geode::prelude;

PetLayer* PetLayer::create() {
	auto ret = new PetLayer();
	if (ret && ret->init()) {
		ret->autorelease();
		return ret;
	}
	delete ret;
	return nullptr;
}

bool PetLayer::init() {
	if (!CCLayer::init()) return false;

	this->setKeypadEnabled(true);

	auto winSize = CCDirector::sharedDirector()->getWinSize();

	const char* bgFile = "game_bg_01_001.png";

	m_bg1 = CCSprite::create(bgFile);
	m_bg2 = CCSprite::create(bgFile);

	m_bg1->setColor({ 34, 60, 110 });
	m_bg2->setColor({34, 60, 110});

	if (!m_bg1 || !m_bg2) {
		return true;
	}

	m_bg1->setAnchorPoint({ 0.f, 0.f });
	m_bg2->setAnchorPoint({ 0.f, 0.f });

	float scaleY = winSize.height / m_bg1->getContentSize().height;
	float scaleX = winSize.width / m_bg1->getContentSize().width;
	float scale = std::max(scaleY, scaleX);
	m_bg1->setScale(scale);
	m_bg2->setScale(scale);

	m_bgWidth = m_bg1->boundingBox().size.width;

	m_bg1->setPosition({ 0.f, 0.f });
	m_bg2->setPosition({ m_bgWidth - 1.f, 0.f });

	this->addChild(m_bg1, -100);
	this->addChild(m_bg2, -100);

	this->scheduleUpdate();

	auto backBtn = CCMenuItemSpriteExtra::create(
		CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png"), this, menu_selector(PetLayer::onBack)
	);
	backBtn->setPosition(25, winSize.height - 25);

	auto menu = CCMenu::create();
	menu->addChild(backBtn);
	menu->setPosition(0, 0);
	this->addChild(menu);

	const char* spr9 = "GJ_square02.png";

	auto panel = CCScale9Sprite::create(spr9);
	panel->setInsetLeft(12);
	panel->setInsetRight(12);
	panel->setInsetTop(12);
	panel->setInsetBottom(12);

	panel->setContentSize({460.f, 280.f});
	panel->setPosition({winSize.width / 2, winSize.height / 2});

	this->addChild(panel);

	auto panelCS = panel->getContentSize();

	auto title = CCLabelBMFont::create("Grinding Pet", "goldFont.fnt");
	title->setPosition(panelCS.width / 2, panelCS.height - 28.f);
	panel->addChild(title);

	auto settingsSpr = CCSprite::createWithSpriteFrameName("accountBtn_settings_001.png");

	auto btnMenu = CCMenu::create();
	btnMenu->setPosition(panelCS.width / 2, panelCS.height / 2);
	panel->addChild(btnMenu);

	auto settingsBtn = CCMenuItemSpriteExtra::create(settingsSpr, this, menu_selector(PetLayer::onSettingsBtn));
	settingsBtn->setPosition(panelCS.width / 2 - 30.f, panelCS.height / 2 - 30.f);
	btnMenu->addChild(settingsBtn);

	auto upgradeSpr = ButtonSprite::create("Upgrade Pet");
	auto upgradeBtn = CCMenuItemSpriteExtra::create(upgradeSpr, this, menu_selector(PetLayer::onUpgradeBtn));
	upgradeBtn->setPosition(100.f, -110.f);
	btnMenu->addChild(upgradeBtn);

	auto renameSpr = ButtonSprite::create("Rename Pet");
	auto renameBtn = CCMenuItemSpriteExtra::create(renameSpr, this, menu_selector(PetLayer::onRenameBtn));
	renameBtn->setPosition(-100.f, -110.f);
	btnMenu->addChild(renameBtn);

	auto petPanel = CCScale9Sprite::create("GJ_square05.png");
	petPanel->setContentSize({200.f, 170.f});
	petPanel->setPosition(0, 4);
	btnMenu->addChild(petPanel);

	return true;
}

void PetLayer::update(float dt) {
	float dx = m_speed * dt;

	m_bg1->setPositionX(m_bg1->getPositionX() - dx);
	m_bg2->setPositionX(m_bg2->getPositionX() - dx);
	if (m_bg1->getPositionX() <= -m_bgWidth) {
		m_bg1->setPositionX(m_bg2->getPositionX() + m_bgWidth - 1.f);
	}
	if (m_bg2->getPositionX() <= -m_bgWidth) {
		m_bg2->setPositionX(m_bg1->getPositionX() + m_bgWidth - 1.f);
	}
}

void PetLayer::onSettingsBtn(CCObject* sender) {
	Notification::create("Settings callback placeholder", NotificationIcon::Info)->show();
}

void PetLayer::onUpgradeBtn(CCObject* sender) {
	Notification::create("Upgrade callback placeholder", NotificationIcon::Info)->show();
}

void PetLayer::onRenameBtn(CCObject* sender) {
	Notification::create("Rename callback placeholder", NotificationIcon::Info)->show();
}

void PetLayer::onBack(CCObject*) {
	CCDirector::sharedDirector()->popSceneWithTransition(0.5f, PopTransition::kPopTransitionFade);
}

void PetLayer::keyBackClicked() {
	this->onBack(nullptr);
}