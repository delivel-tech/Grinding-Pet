#include <Geode/Geode.hpp>
#include "PetLayer.hpp"
#include <Geode/fmod/fmod.h>
#include "RenamePopup.hpp"
#include "PetUtils.hpp"
#include <Geode/utils/coro.hpp>

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

	auto audio = GameManager::sharedState();
	auto audio1 = FMODAudioEngine::sharedEngine();

	audio1->pauseMusic(0);
	audio1->playMusic("secretShop.mp3", true, 1.f, 0);

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

	m_titleLabel = CCLabelBMFont::create(Mod::get()->getSavedValue<std::string>("pet-name").c_str(), "goldFont.fnt");
	m_titleLabel->setPosition(panelCS.width / 2, panelCS.height - 28.f);
	panel->addChild(m_titleLabel);

	auto settingsSpr = CCSprite::createWithSpriteFrameName("GJ_optionsBtn02_001.png");

	auto btnMenu = CCMenu::create();
	btnMenu->setPosition(panelCS.width / 2, panelCS.height / 2);
	panel->addChild(btnMenu);

	auto settingsBtn = CCMenuItemSpriteExtra::create(settingsSpr, this, menu_selector(PetLayer::onSettingsBtn));
	settingsBtn->setPosition(panelCS.width / 2 - 30.f, panelCS.height / 2 - 30.f);
	btnMenu->addChild(settingsBtn);

	auto reloadSpr = CCSprite::createWithSpriteFrameName("GJ_replayBtn_001.png");
	reloadSpr->setScale(0.65f);

	auto reloadBtn = CCMenuItemSpriteExtra::create(reloadSpr, this, menu_selector(PetLayer::onReloadBtn));
	reloadBtn->setPosition(panelCS.width / 2 - 30.f, panelCS.height / 2 - 73.f);
	btnMenu->addChild(reloadBtn);

	auto upgradeSpr = ButtonSprite::create("Upgrade Pet");
	auto upgradeBtn = CCMenuItemSpriteExtra::create(upgradeSpr, this, menu_selector(PetLayer::onUpgradeBtn));
	upgradeBtn->setPosition(100.f, -110.f);
	btnMenu->addChild(upgradeBtn);

	auto renameSpr = ButtonSprite::create("Rename Pet");
	auto renameBtn = CCMenuItemSpriteExtra::create(renameSpr, this, menu_selector(PetLayer::onRenameBtn));
	renameBtn->setPosition(-100.f, -110.f);
	btnMenu->addChild(renameBtn);

	auto petPanel = CCScale9Sprite::create("GJ_square05.png");
	petPanel->setContentSize({120.f, 140.f});
	petPanel->setPosition(0, 12);
	btnMenu->addChild(petPanel);

	auto petShadow = CCSprite::createWithSpriteFrameName("chest_shadow_001.png");
	petShadow->setPosition({60.f, 40.f});
	petShadow->setScale(0.8f);
	petShadow->setOpacity(50.f);
	petPanel->addChild(petShadow);

	auto statsPanel = CCScale9Sprite::create("GJ_square05.png");
	statsPanel->setContentSize({80.f, 80.f});
	statsPanel->setPosition(-115.f, 15.f);
	btnMenu->addChild(statsPanel);

	auto infoPanel = CCScale9Sprite::create("GJ_square05.png");
	infoPanel->setContentSize({80.f, 80.f});
	infoPanel->setPosition(115.f, 15.f);
	btnMenu->addChild(infoPanel);

	auto gm = GameManager::sharedState();
	auto playerPet = SimplePlayer::create(gm->m_playerFrame);
	playerPet->updatePlayerFrame(gm->m_playerFrame, IconType::Cube);
	playerPet->setColors(gm->colorForIdx(gm->m_playerColor), gm->colorForIdx(gm->m_playerColor2));
	if (gm->m_playerGlow != 0) {
		playerPet->setGlowOutline(gm->colorForIdx(gm->m_playerGlowColor));
	}
	playerPet->setScale(1.5f);
	playerPet->setPositionY(5.f);
	btnMenu->addChild(playerPet);

	auto lvlSpr = CCSprite::createWithSpriteFrameName("shard0202ShardSmall_001.png");
	if (!lvlSpr) {
		log::error("lvlSpr not found");
		return true;
	}
	lvlSpr->setPosition({320.f, 170.f});
	lvlSpr->setScale(1.2f);
	panel->addChild(lvlSpr);

	auto lvlAmount = CCLabelBMFont::create(fmt::format("Lvl: {}", Mod::get()->getSavedValue<int>("pet-level")).c_str(), "bigFont.fnt");
	lvlAmount->setAnchorPoint({0.f, 0.5f});
	lvlAmount->setPosition({332.f, 170.f});
	limitNodeSize(lvlAmount, {40.f, 32.f}, 0.5f, 0.1f);
	panel->addChild(lvlAmount);

	auto starSpr = CCSprite::create("starSpr.png"_spr);
	if (!starSpr) return true;
	starSpr->setPosition({95.f, 170.f});
	starSpr->setScale(1.5f);
	panel->addChild(starSpr);

	auto starAmountVa = Mod::get()->getSavedValue<int>("pet-stars");
	auto starAmount = CCLabelBMFont::create(std::to_string(starAmountVa).c_str(), "bigFont.fnt");
	starAmount->setAnchorPoint({0.f, 0.5f});
	starAmount->setPosition({110.f, 170.f});
	limitNodeSize(starAmount, {40.f, 32.f}, 0.5f, 0.1f);
	panel->addChild(starAmount);

	auto moonSpr = CCSprite::create("moonSpr.png"_spr);
	if (!moonSpr) return true;
	moonSpr->setPosition({95.f, 145.f});
	moonSpr->setScale(1.5f);
	panel->addChild(moonSpr);

	auto moonAmountVa = Mod::get()->getSavedValue<int>("pet-moons");
	auto moonAmount = CCLabelBMFont::create(std::to_string(moonAmountVa).c_str(), "bigFont.fnt");
	moonAmount->setAnchorPoint({0.f, 0.5f});
	moonAmount->setPosition({110.f, 145.f});
	limitNodeSize(moonAmount, {40.f, 32.f}, 0.5f, 0.1f);
	panel->addChild(moonAmount);

	auto backProgressBar = CCSprite::create("GJ_progressBar_001.png");
	backProgressBar->setPosition({235.f, 65.f});
	backProgressBar->setScale(0.875f);
	backProgressBar->setColor({0, 0, 0});
	backProgressBar->setOpacity(80.f);
	panel->addChild(backProgressBar);

	auto topProgressBar = CCSprite::create("GJ_progressBar_001.png");
	topProgressBar->setColor({ 235, 255, 93 });
	auto progressBar = CCProgressTimer::create(topProgressBar);
	progressBar->setPosition({235.f, 65.f});
	progressBar->setScaleX(0.867f);
	progressBar->setScaleY(0.7f);
	progressBar->setType(kCCProgressTimerTypeBar);
	progressBar->setMidpoint({0.f, 0.5f});
	progressBar->setBarChangeRate({1.f, 0.f});
	panel->addChild(progressBar);

	auto leftStarBar = CCLabelBMFont::create(std::to_string(starAmountVa).c_str(), "bigFont.fnt");
	limitNodeSize(leftStarBar, {40.f, 32.f}, 0.4f, 0.1f);
	leftStarBar->setPosition(110.f, 59.f);
	leftStarBar->setAnchorPoint({0.5f, 0.f});
	panel->addChild(leftStarBar);

	if (Mod::get()->getSavedValue<int>("pet-level") == 1) {
		auto rightStarBar = CCLabelBMFont::create(std::to_string(2000).c_str(), "bigFont.fnt");
	    limitNodeSize(rightStarBar, {40.f, 32.f}, 0.4f, 0.1f);
	    rightStarBar->setPosition(343.f, 65.f);
	    rightStarBar->setAnchorPoint({0.f, 0.5f});
	    panel->addChild(rightStarBar);

		float ratio = ((float)Mod::get()->getSavedValue<int>("pet-stars") / 2000.f) * 100.f;
		if (ratio > 100) {
			ratio = 100.f;
		}
		progressBar->setPercentage(ratio);

		auto estimatedInBar = CCLabelBMFont::create(fmt::format("{}%", (int)ratio).c_str(), "bigFont.fnt");
		limitNodeSize(estimatedInBar, {40.f, 32.f}, 0.4f, 0.1f);
		estimatedInBar->setPosition(232.f, 65.f);
		panel->addChild(estimatedInBar);
	}

	if (Mod::get()->getSavedValue<int>("pet-level") == 2) {
		auto rightStarBar = CCLabelBMFont::create(std::to_string(5000).c_str(), "bigFont.fnt");
	    limitNodeSize(rightStarBar, {40.f, 32.f}, 0.4f, 0.1f);
	    rightStarBar->setPosition(343.f, 65.f);
	    rightStarBar->setAnchorPoint({0.f, 0.5f});
	    panel->addChild(rightStarBar);

		float ratio = ((float)Mod::get()->getSavedValue<int>("pet-stars") / 5000.f) * 100.f;
		if (ratio > 100) {
			ratio = 100.f;
		}
		progressBar->setPercentage(ratio);

		auto estimatedInBar = CCLabelBMFont::create(fmt::format("{}%", (int)ratio).c_str(), "bigFont.fnt");
		limitNodeSize(estimatedInBar, {40.f, 32.f}, 0.4f, 0.1f);
		estimatedInBar->setPosition(232.f, 65.f);
		panel->addChild(estimatedInBar);
	}

	if (Mod::get()->getSavedValue<int>("pet-level") == 3) {
		auto rightStarBar = CCLabelBMFont::create(std::to_string(20000).c_str(), "bigFont.fnt");
	    limitNodeSize(rightStarBar, {40.f, 32.f}, 0.4f, 0.1f);
	    rightStarBar->setPosition(343.f, 65.f);
	    rightStarBar->setAnchorPoint({0.f, 0.5f});
	    panel->addChild(rightStarBar);

		float ratio = ((float)Mod::get()->getSavedValue<int>("pet-stars") / 20000.f) * 100.f;
		if (ratio > 100) {
			ratio = 100.f;
		}
		progressBar->setPercentage(ratio);

		auto estimatedInBar = CCLabelBMFont::create(fmt::format("{}%", (int)ratio).c_str(), "bigFont.fnt");
		limitNodeSize(estimatedInBar, {40.f, 32.f}, 0.4f, 0.1f);
		estimatedInBar->setPosition(232.f, 65.f);
		panel->addChild(estimatedInBar);
	}

	if (Mod::get()->getSavedValue<int>("pet-level") == 4) {
		auto rightStarBar = CCLabelBMFont::create(std::to_string(50000).c_str(), "bigFont.fnt");
	    limitNodeSize(rightStarBar, {40.f, 32.f}, 0.4f, 0.1f);
	    rightStarBar->setPosition(343.f, 65.f);
	    rightStarBar->setAnchorPoint({0.f, 0.5f});
	    panel->addChild(rightStarBar);

		float ratio = ((float)Mod::get()->getSavedValue<int>("pet-stars") / 50000.f) * 100.f;
		if (ratio > 100) {
			ratio = 100.f;
		}
		progressBar->setPercentage(ratio);

		auto estimatedInBar = CCLabelBMFont::create(fmt::format("{}%", (int)ratio).c_str(), "bigFont.fnt");
		limitNodeSize(estimatedInBar, {40.f, 32.f}, 0.4f, 0.1f);
		estimatedInBar->setPosition(232.f, 65.f);
		panel->addChild(estimatedInBar);
	}

	if (Mod::get()->getSavedValue<int>("pet-level") == 5) {
		progressBar->setPercentage(100.f);

		auto estimatedInBar = CCLabelBMFont::create("Fully upgraded!", "bigFont.fnt");
		estimatedInBar->setScale(0.4f);
		estimatedInBar->setPosition(232.f, 65.f);
		panel->addChild(estimatedInBar);
		panel->removeChild(leftStarBar);
	}

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

Task<void> PetLayer::runSyncFlow() {
	Notification::create("[Grinding Pet] Syncing...", NotificationIcon::Loading)->show();
	
	co_await PetUtils::newCreateUser();
	co_await PetUtils::checkStats();

	Notification::create("[Grinding Pet] Synced data.", NotificationIcon::Success)->show();

	co_return;
}

void PetLayer::onReloadBtn(CCObject* sender) {
	coro::spawn << PetLayer::runSyncFlow();
}

void PetLayer::onUpgradeBtn(CCObject* sender) {
	if (Mod::get()->getSavedValue<int>("pet-level") == 1) {
		if (Mod::get()->getSavedValue<int>("pet-stars") >= 2000) {
			coro::spawn << PetUtils::upgradeLevelTo2();
		} else {
			Notification::create("Reach 2000 pet stars!", NotificationIcon::Error)->show();
		}
	}
	if (Mod::get()->getSavedValue<int>("pet-level") == 2) {
		if (Mod::get()->getSavedValue<int>("pet-stars") >= 5000) {
			coro::spawn << PetUtils::upgradeLevelTo3();
		} else {
			Notification::create("Reach 5000 pet stars!", NotificationIcon::Error)->show();
		}
	}
	if (Mod::get()->getSavedValue<int>("pet-level") == 3) {
		if (Mod::get()->getSavedValue<int>("pet-stars") >= 20000) {
			coro::spawn << PetUtils::upgradeLevelTo4();
		} else {
			Notification::create("Reach 20000 pet stars!", NotificationIcon::Error)->show();
		}
	}
	if (Mod::get()->getSavedValue<int>("pet-level") == 4) {
		if (Mod::get()->getSavedValue<int>("pet-stars") >= 50000) {
			coro::spawn << PetUtils::upgradeLevelTo5();
		} else {
			Notification::create("Reach 50000 pet stars!", NotificationIcon::Error)->show();
		}
	}
	if (Mod::get()->getSavedValue<int>("pet-level") == 5) {
		Notification::create("Your pet is fully upgraded!", NotificationIcon::Success)->show();
	}
}

void PetLayer::onRenameBtn(CCObject*) {
    RenamePopup::create("Grinding Pet")->show();
}

void PetLayer::onBack(CCObject*) {
	auto audio = GameManager::sharedState();
	audio->fadeInMenuMusic();
	CCDirector::sharedDirector()->popSceneWithTransition(0.5f, PopTransition::kPopTransitionFade);
}

void PetLayer::keyBackClicked() {
	this->onBack(nullptr);
}