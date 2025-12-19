#include <Geode/Geode.hpp>
#include "PetLayer.hpp"
#include <Geode/fmod/fmod.h>
#include "RenamePopup.hpp"
#include "PetUtils.hpp"

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

static constexpr char const* kPetNameKey = "pet-name";


bool PetLayer::init() {
	if (!CCLayer::init()) return false;

	auto audio = GameManager::sharedState();
	auto audio1 = FMODAudioEngine::sharedEngine();

	audio1->pauseMusic(0);
	audio1->playMusic("secretShop.mp3", true, 1.f, 0);

	this->setKeypadEnabled(true);

	auto winSize = CCDirector::sharedDirector()->getWinSize();

	PetUtils::checkStars();
	Mod::get()->setSavedValue<int>("moonAmountV", 0);

	auto saved = geode::Mod::get()->getSavedValue<std::string>(kPetNameKey, "Grinding Pet");
    m_petName = saved;


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

	m_titleLabel = CCLabelBMFont::create(m_petName.c_str(), "goldFont.fnt");
	m_titleLabel->setPosition(panelCS.width / 2, panelCS.height - 28.f);
	panel->addChild(m_titleLabel);

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
	petPanel->setContentSize({120.f, 170.f});
	petPanel->setPosition(0, 4);
	btnMenu->addChild(petPanel);

	auto petShadow = CCSprite::createWithSpriteFrameName("chest_shadow_001.png");
	petShadow->setPosition({60.f, 40.f});
	petShadow->setScale(0.8f);
	petShadow->setOpacity(50.f);
	petPanel->addChild(petShadow);

	auto petIcon = CCSprite::createWithSpriteFrameName("player_01_001.png");
	petIcon->setPosition({60.f, 65.f});
	petIcon->setScale(1.7f);
	petPanel->addChild(petIcon);

	auto starSpr = CCSprite::create("starSpr.png"_spr);
	if (!starSpr) return true;
	starSpr->setPosition({20.f, 200.f});
	starSpr->setScale(1.5f);
	panel->addChild(starSpr);

	auto starAmountVa = Mod::get()->getSavedValue<int>("starAmountV");
	auto starAmount = CCLabelBMFont::create(std::to_string(starAmountVa).c_str(), "bigFont.fnt");
	starAmount->setAnchorPoint({0.f, 0.5f});
	starAmount->setPosition({35.f, 200.f});
	starAmount->setScale(0.5f);
	panel->addChild(starAmount);

	auto moonSpr = CCSprite::create("moonSpr.png"_spr);
	if (!moonSpr) return true;
	moonSpr->setPosition({20.f, 175.f});
	moonSpr->setScale(1.5f);
	panel->addChild(moonSpr);

	auto moonAmountVa = Mod::get()->getSavedValue<int>("moonAmountV");
	auto moonAmount = CCLabelBMFont::create(std::to_string(moonAmountVa).c_str(), "bigFont.fnt");
	moonAmount->setAnchorPoint({0.f, 0.5f});
	moonAmount->setPosition({35.f, 175.f});
	moonAmount->setScale(0.5f);
	panel->addChild(moonAmount);

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
	Notification::create(fmt::format("Stats: {}", Mod::get()->getSavedValue<int>("current-stars")), NotificationIcon::Info)->show();
}

void PetLayer::onRenameBtn(CCObject* sender) {
	RenamePopup::create(m_petName, [this](std::string newName) {
    m_petName = std::move(newName);

    geode::Mod::get()->setSavedValue<std::string>(kPetNameKey, m_petName);

    if (m_titleLabel) m_titleLabel->setString(m_petName.c_str());
})->show();

}

void PetLayer::onBack(CCObject*) {
	auto audio = GameManager::sharedState();
	audio->fadeInMenuMusic();
	CCDirector::sharedDirector()->popSceneWithTransition(0.5f, PopTransition::kPopTransitionFade);
}

void PetLayer::keyBackClicked() {
	this->onBack(nullptr);
}