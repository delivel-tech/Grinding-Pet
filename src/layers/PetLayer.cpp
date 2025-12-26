#include <Geode/Geode.hpp>
#include "layers/PetLayer.hpp"
#include <Geode/fmod/fmod.h>
#include "popups/RenamePopup.hpp"
#include "popups/PetShop.hpp"
#include "utils/PetUtils.hpp"
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

	auto upgradeRareSpr = CCSprite::createWithSpriteFrameName("GJ_orderUpBtn_001.png");
	upgradeRareSpr->setScale(0.8f);
	auto upgradeRareBtn = CCMenuItemSpriteExtra::create(upgradeRareSpr, this, menu_selector(PetLayer::onUpgradeRareBtn));
	upgradeRareBtn->setPosition(panelCS.width / 2 - 30.f, panelCS.height / 2 - 116.f);
	btnMenu->addChild(upgradeRareBtn);

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
	playerPet->setColors({111, 255, 0}, {0, 251, 255});
	if (Mod::get()->getSavedValue<int>("pet-level") >= 10) {
		playerPet->setColors(gm->colorForIdx(gm->m_playerColor), gm->colorForIdx(gm->m_playerColor2));
	    if (gm->m_playerGlow != 0) {
		    playerPet->setGlowOutline(gm->colorForIdx(gm->m_playerGlowColor));
	}
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

	auto rareSpr = CCSprite::createWithSpriteFrameName("shadowShardSmall_001.png");
	if (!rareSpr) {
		log::error("sprite not found");
		return true;
	}
	rareSpr->setPosition({320.f, 145.f});
	rareSpr->setScale(1.2f);
	panel->addChild(rareSpr);

	auto rare = CCLabelBMFont::create(Mod::get()->getSavedValue<std::string>("pet-rareness").c_str(), "bigFont.fnt");
	rare->setAnchorPoint({0.f, 0.5f});
	rare->setPosition({332.f, 145.f});
	limitNodeSize(rare, {40.f, 32.f}, 0.5f, 0.1f);
	panel->addChild(rare);

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


	int level = Mod::get()->getSavedValue<int>("pet-level");
int stars = Mod::get()->getSavedValue<int>("pet-stars");

int cost = getUpgradeCost(level);
if (cost > 99 && cost < 16540) {
	auto rightStarBar = CCLabelBMFont::create(std::to_string(cost).c_str(), "bigFont.fnt");
	limitNodeSize(rightStarBar, {40.f, 32.f}, 0.4f, 0.1f);
	rightStarBar->setPosition(343.f, 65.f);
	rightStarBar->setAnchorPoint({0.f, 0.5f});
	panel->addChild(rightStarBar);

	float ratio = ((float)stars / (float)cost) * 100.f;
	if (ratio > 100.f) ratio = 100.f;
	if (ratio < 0.f) ratio = 0.f;

	progressBar->setPercentage(ratio);

	auto estimatedInBar = CCLabelBMFont::create(fmt::format("{}%", (int)ratio).c_str(), "bigFont.fnt");
	limitNodeSize(estimatedInBar, {40.f, 32.f}, 0.4f, 0.1f);
	estimatedInBar->setPosition(232.f, 65.f);
	panel->addChild(estimatedInBar);
} else {
	progressBar->setPercentage(100.f);

		auto estimatedInBar = CCLabelBMFont::create("Fully upgraded!", "bigFont.fnt");
		estimatedInBar->setScale(0.4f);
		estimatedInBar->setPosition(232.f, 65.f);
		panel->addChild(estimatedInBar);
		panel->removeChild(leftStarBar);
}

	auto infoBtnSpr = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
	auto infoBtn = CCMenuItemSpriteExtra::create(infoBtnSpr, this, menu_selector(PetLayer::onInfoBtn));
	infoBtn->setPosition(200.f, -12.f);
	btnMenu->addChild(infoBtn);

	if (Mod::get()->getSavedValue<int>("pet-level") >= 20) {
		auto shopSpr = CCSprite::createWithSpriteFrameName("shopRope4_001.png");
	if (!shopSpr) {
		log::error("spr not found");
	}
	shopSpr->setScale(0.9f);
	auto shopBtn = CCMenuItemSpriteExtra::create(shopSpr, this, menu_selector(PetLayer::onShopBtn));
	shopBtn->setPosition(-180.f, 130.f);
	btnMenu->addChild(shopBtn);
	}

	return true;
}

int PetLayer::getUpgradeCost(int level) {
	switch (level) {
		case 1: return 100;
		case 2: return 120;
		case 3: return 144;
		case 4: return 173;
		case 5: return 208;
		case 6: return 250;
		case 7: return 300;
		case 8: return 360;
		case 9: return 432;
		case 10: return 518;
		case 11: return 622;
		case 12: return 746;
		case 13: return 895;
		case 14: return 1074;
		case 15: return 1289;
		case 16: return 1547;
		case 17: return 1856;
		case 18: return 2227;
		case 19: return 2672;
		case 20: return 3206;
		case 21: return 3847;
		case 22: return 4616;
		case 23: return 5539;
		case 24: return 6647;
		case 25: return 7976;
		case 26: return 9571;
		case 27: return 11485;
		case 28: return 13782;
		case 29: return 16539;
		default: return 0;
	}
}

void PetLayer::onShopBtn(CCObject* sender) {
	PetShop::create()->show();
}

void PetLayer::onUpgradeRareBtn(CCObject* sender) {
	if (Mod::get()->getSavedValue<std::string>("pet-rareness") == "Common") {
		if (Mod::get()->getSavedValue<int>("pet-moons") >= 500) {
			coro::spawn << PetUtils::upgradeRarenessToRare();
		} else {
			Notification::create("Reach 500 pet moons!", NotificationIcon::Error)->show();
		}
	}
	if (Mod::get()->getSavedValue<std::string>("pet-rareness") == "Rare") {
		if (Mod::get()->getSavedValue<int>("pet-moons") >= 3000) {
			coro::spawn << PetUtils::upgradeRarenessToEpic();
		} else {
			Notification::create("Reach 3000 pet moons!", NotificationIcon::Error)->show();
		}
	}
	if (Mod::get()->getSavedValue<std::string>("pet-rareness") == "Epic") {
		if (Mod::get()->getSavedValue<int>("pet-moons") >= 10000) {
			coro::spawn << PetUtils::upgradeRarenessToMythic();
		} else {
			Notification::create("Reach 10000 pet moons!", NotificationIcon::Error)->show();
		}
	}
	if (Mod::get()->getSavedValue<std::string>("pet-rareness") == "Mythic") {
		Notification::create("Your rareness is fully upgraded!", NotificationIcon::Success)->show();
	}
}

void PetLayer::onInfoBtn(CCObject* sender) {
	MDPopup::create(
		"Grinding Pet",
		"### <cj>How to use</c> the mod?\n"
		"Well, once this mod is installed, it tracks your <cg>in-game stats</c>. Once you sync your data, newly grinded stats will turn to <cy>Pet Stats</c>.\n"
		"You can use <cy>Pet Stats</c> for evolving your pet, for example, you need to reach N amount of stars to upgrade <cb>Pet's Level.</c>\n"
		"<cy>Pet Stars</c> are used for long-time pet evolving, while <cp>Pet Moons</c> are used for small purchases like <cg>Pet Decorations</c>, or rareness.\n"
		"<cp>Pet Level</c> allows you to open new features, like shop. <cg>Pet Rareness</c> is just a status of your pet, evolved by moons, doesn't give any perks."
		"\r\n\r\n---\r\n\r\n"
		"### <cj>Additional info</c> regarding the mod:\n"
		"<cy>First:</c> Your statistics are not automatically updated except when mod is loaded, therefore you'll need to use Sync button in Pet Menu.\n"
		"<co>Secondly:</c> Pet statistics DOESN'T change real stars or moons, it can only load. So that you don't need to worry, it doesn't go against Leaderboard Guidelines.\n"
		"<cg>Thirdly:</c> Once your data is synced, it's stored in User Database, so if you reinstall the mod, you won't lose your data! Mod doesn't store any private information.\n"
		"<cr>Fourthly:</c> This mod uses Argon for authentification purposes. By using Grinding Pet you allow it to send a one-time message to a bot account.\n"
		"\r\n\r\n---\r\n\r\n"
		"### <cj>Rules:</c>\n"
		"<cy>Firstly:</c> As this mod is online, keep <cb>Pet Name</c> appropriate. Naming it bad may result in <cy>Ban</c> from the mod.\n"
		"<cp>Secondly:</c> Don't modify any statistics in mod, you will be <cy>banned</c>.\n",
		"OK"
	)->show();
}

void PetLayer::onInfoBtnOutside() {
	MDPopup::create(
		"Grinding Pet",
		"### <cj>How to use</c> the mod?\n"
		"Well, once this mod is installed, it tracks your <cg>in-game stats</c>. Once you sync your data, newly grinded stats will turn to <cy>Pet Stats</c>.\n"
		"You can use <cy>Pet Stats</c> for evolving your pet, for example, you need to reach N amount of stars to upgrade <cb>Pet's Level.</c>\n"
		"<cy>Pet Stars</c> are used for long-time pet evolving, while <cp>Pet Moons</c> are used for small purchases like <cg>Pet Decorations</c>, or rareness.\n"
		"<cp>Pet Level</c> allows you to open new features, like shop. <cg>Pet Rareness</c> is just a status of your pet, evolved by moons, doesn't give any perks.\n"
		"\r\n\r\n---\r\n\r\n"
		"### <cj>Additional info</c> regarding the mod:\n"
		"<cy>First:</c> Your statistics are not automatically updated except when mod is loaded, therefore you'll need to use Sync button in Pet Menu.\n"
		"<co>Secondly:</c> Pet statistics DOESN'T change real stars or moons, it can only load. So that you don't need to worry, it doesn't go against Leaderboard Guidelines.\n"
		"<cg>Thirdly:</c> Once your data is synced, it's stored in User Database, so if you reinstall the mod, you won't lose your data! Mod doesn't store any private information.\n"
		"<cr>Fourthly:</c> This mod uses Argon for authentification purposes. By using Grinding Pet you allow it to send a one-time message to a bot account.\n"
		"\r\n\r\n---\r\n\r\n"
		"### <cj>Rules:</c>\n"
		"<cy>Firstly:</c> As this mod is online, keep <cb>Pet Name</c> appropriate. Naming it bad may result in <cy>Ban</c> from the mod.\n"
		"<cp>Secondly:</c> Don't modify any statistics in mod, you will be <cy>banned</c>.\n",
		"OK"
	)->show();
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
	int level = Mod::get()->getSavedValue<int>("pet-level");
	int stars = Mod::get()->getSavedValue<int>("pet-stars");

	if (level == 1) {
		if (stars >= 100) coro::spawn << PetUtils::upgradeLevelBy(1, -100);
		else Notification::create("Reach 100 pet stars!", NotificationIcon::Error)->show();
	}
	else if (level == 2) {
		if (stars >= 120) coro::spawn << PetUtils::upgradeLevelBy(1, -120);
		else Notification::create("Reach 120 pet stars!", NotificationIcon::Error)->show();
	}
	else if (level == 3) {
		if (stars >= 144) coro::spawn << PetUtils::upgradeLevelBy(1, -144);
		else Notification::create("Reach 144 pet stars!", NotificationIcon::Error)->show();
	}
	else if (level == 4) {
		if (stars >= 173) coro::spawn << PetUtils::upgradeLevelBy(1, -173);
		else Notification::create("Reach 173 pet stars!", NotificationIcon::Error)->show();
	}
	else if (level == 5) {
		if (stars >= 208) coro::spawn << PetUtils::upgradeLevelBy(1, -208);
		else Notification::create("Reach 208 pet stars!", NotificationIcon::Error)->show();
	}
	else if (level == 6) {
		if (stars >= 250) coro::spawn << PetUtils::upgradeLevelBy(1, -250);
		else Notification::create("Reach 250 pet stars!", NotificationIcon::Error)->show();
	}
	else if (level == 7) {
		if (stars >= 300) coro::spawn << PetUtils::upgradeLevelBy(1, -300);
		else Notification::create("Reach 300 pet stars!", NotificationIcon::Error)->show();
	}
	else if (level == 8) {
		if (stars >= 360) coro::spawn << PetUtils::upgradeLevelBy(1, -360);
		else Notification::create("Reach 360 pet stars!", NotificationIcon::Error)->show();
	}
	else if (level == 9) {
		if (stars >= 432) coro::spawn << PetUtils::upgradeLevelBy(1, -432);
		else Notification::create("Reach 432 pet stars!", NotificationIcon::Error)->show();
	}
	else if (level == 10) {
		if (stars >= 518) coro::spawn << PetUtils::upgradeLevelBy(1, -518);
		else Notification::create("Reach 518 pet stars!", NotificationIcon::Error)->show();
	}
	else if (level == 11) {
		if (stars >= 622) coro::spawn << PetUtils::upgradeLevelBy(1, -622);
		else Notification::create("Reach 622 pet stars!", NotificationIcon::Error)->show();
	}
	else if (level == 12) {
		if (stars >= 746) coro::spawn << PetUtils::upgradeLevelBy(1, -746);
		else Notification::create("Reach 746 pet stars!", NotificationIcon::Error)->show();
	}
	else if (level == 13) {
		if (stars >= 895) coro::spawn << PetUtils::upgradeLevelBy(1, -895);
		else Notification::create("Reach 895 pet stars!", NotificationIcon::Error)->show();
	}
	else if (level == 14) {
		if (stars >= 1074) coro::spawn << PetUtils::upgradeLevelBy(1, -1074);
		else Notification::create("Reach 1074 pet stars!", NotificationIcon::Error)->show();
	}
	else if (level == 15) {
		if (stars >= 1289) coro::spawn << PetUtils::upgradeLevelBy(1, -1289);
		else Notification::create("Reach 1289 pet stars!", NotificationIcon::Error)->show();
	}
	else if (level == 16) {
		if (stars >= 1547) coro::spawn << PetUtils::upgradeLevelBy(1, -1547);
		else Notification::create("Reach 1547 pet stars!", NotificationIcon::Error)->show();
	}
	else if (level == 17) {
		if (stars >= 1856) coro::spawn << PetUtils::upgradeLevelBy(1, -1856);
		else Notification::create("Reach 1856 pet stars!", NotificationIcon::Error)->show();
	}
	else if (level == 18) {
		if (stars >= 2227) coro::spawn << PetUtils::upgradeLevelBy(1, -2227);
		else Notification::create("Reach 2227 pet stars!", NotificationIcon::Error)->show();
	}
	else if (level == 19) {
		if (stars >= 2672) coro::spawn << PetUtils::upgradeLevelBy(1, -2672);
		else Notification::create("Reach 2672 pet stars!", NotificationIcon::Error)->show();
	}
	else if (level == 20) {
		if (stars >= 3206) coro::spawn << PetUtils::upgradeLevelBy(1, -3206);
		else Notification::create("Reach 3206 pet stars!", NotificationIcon::Error)->show();
	}
	else if (level == 21) {
		if (stars >= 3847) coro::spawn << PetUtils::upgradeLevelBy(1, -3847);
		else Notification::create("Reach 3847 pet stars!", NotificationIcon::Error)->show();
	}
	else if (level == 22) {
		if (stars >= 4616) coro::spawn << PetUtils::upgradeLevelBy(1, -4616);
		else Notification::create("Reach 4616 pet stars!", NotificationIcon::Error)->show();
	}
	else if (level == 23) {
		if (stars >= 5539) coro::spawn << PetUtils::upgradeLevelBy(1, -5539);
		else Notification::create("Reach 5539 pet stars!", NotificationIcon::Error)->show();
	}
	else if (level == 24) {
		if (stars >= 6647) coro::spawn << PetUtils::upgradeLevelBy(1, -6647);
		else Notification::create("Reach 6647 pet stars!", NotificationIcon::Error)->show();
	}
	else if (level == 25) {
		if (stars >= 7976) coro::spawn << PetUtils::upgradeLevelBy(1, -7976);
		else Notification::create("Reach 7976 pet stars!", NotificationIcon::Error)->show();
	}
	else if (level == 26) {
		if (stars >= 9571) coro::spawn << PetUtils::upgradeLevelBy(1, -9571);
		else Notification::create("Reach 9571 pet stars!", NotificationIcon::Error)->show();
	}
	else if (level == 27) {
		if (stars >= 11485) coro::spawn << PetUtils::upgradeLevelBy(1, -11485);
		else Notification::create("Reach 11485 pet stars!", NotificationIcon::Error)->show();
	}
	else if (level == 28) {
		if (stars >= 13782) coro::spawn << PetUtils::upgradeLevelBy(1, -13782);
		else Notification::create("Reach 13782 pet stars!", NotificationIcon::Error)->show();
	}
	else if (level == 29) {
		if (stars >= 16539) coro::spawn << PetUtils::upgradeLevelBy(1, -16539);
		else Notification::create("Reach 16539 pet stars!", NotificationIcon::Error)->show();
	} else if (level == 30) {
		Notification::create("Your level is fully upgraded!", NotificationIcon::Success)->show();
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