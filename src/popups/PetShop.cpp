#include <Geode/Geode.hpp>
#include "popups/PetShop.hpp"

using namespace geode::prelude;

bool PetShop::setup(std::string const& value) {
    this->setTitle("Pet Shop", "goldFont.fnt", 0.8f);

    auto size = m_mainLayer->getContentSize();

    auto decoBtn = TabButton::create(
        TabBaseColor::Unselected,
        TabBaseColor::UnselectedDark,
        "Decorations",
        this,
        menu_selector(PetShop::onDecoBtn)
    );
    auto effectsBtn = TabButton::create(
        TabBaseColor::Unselected,
        TabBaseColor::UnselectedDark,
        "Effects",
        this,
        menu_selector(PetShop::onEffectsBtn)
    );
    auto repliesBtn = TabButton::create(
        TabBaseColor::Unselected,
        TabBaseColor::UnselectedDark,
        "Replies",
        this,
        menu_selector(PetShop::onRepliesBtn)
    );
    effectsBtn->setPositionX(120.f);
    repliesBtn->setPositionX(240.f);
    auto menu = CCMenu::create();
    menu->setPosition(66.5f, 242.f);
    m_mainLayer->addChild(menu);
    menu->addChild(decoBtn);
    menu->addChild(effectsBtn);
    menu->addChild(repliesBtn);
    m_decoBtn = decoBtn;
    m_effectsBtn = effectsBtn;
    m_repliesBtn = repliesBtn;
    mainLayer = m_mainLayer;

    selectTab(0);
    auto decoMenu = ScrollLayer::create({320.f, 200.f}, true, true);
    decoMenu->setID("deco-menu");
    decoMenu->setPositionX(25.f);
    decoMenu->setScale(0.925f);

    auto item1Menu = CCMenu::create();
    item1Menu->setPosition(40.f, 140.f);
    auto bgItem = CCScale9Sprite::create("GJ_square03.png");
    bgItem->setContentHeight(100.f);
    auto petShadow = CCSprite::createWithSpriteFrameName("chest_shadow_001.png");
    petShadow->setOpacity(50.f);
    petShadow->setScale(0.5f);
    petShadow->setPositionY(-10.f);
    auto pet = CCSprite::createWithSpriteFrameName("player_01_001.png");
    pet->setPositionY(5.f);
    auto itemBuyBtn = ButtonSprite::create("Buy");
    itemBuyBtn->setScale(0.75f);
    itemBuyBtn->setPositionY(-30.f);
    auto itemCostSpr = CCSprite::create("moonSpr.png"_spr);
    auto itemCost = CCLabelBMFont::create("250", "bigFont.fnt");
    itemCostSpr->setPosition({-20.f, 35.f});
    itemCost->setPosition({5.f, 35.f});
    itemCost->setScale(0.5f);
    item1Menu->addChild(bgItem);
    item1Menu->addChild(petShadow);
    item1Menu->addChild(pet);
    item1Menu->addChild(itemBuyBtn);
    item1Menu->addChild(itemCostSpr);
    item1Menu->addChild(itemCost);
    decoMenu->m_contentLayer->addChild(item1Menu);

    auto item2Menu = CCMenu::create();
    item2Menu->setPosition(160.f, 140.f);
    auto bgItem2 = CCScale9Sprite::create("GJ_square03.png");
    bgItem2->setContentHeight(100.f);
    auto petShadow2 = CCSprite::createWithSpriteFrameName("chest_shadow_001.png");
    petShadow2->setOpacity(50.f);
    petShadow2->setScale(0.5f);
    petShadow2->setPositionY(-10.f);
    auto pet2 = CCSprite::createWithSpriteFrameName("player_01_001.png");
    pet2->setPositionY(5.f);
    auto itemBuyBtn2 = ButtonSprite::create("Buy");
    itemBuyBtn2->setScale(0.75f);
    itemBuyBtn2->setPositionY(-30.f);
    auto itemCostSpr2 = CCSprite::create("moonSpr.png"_spr);
    auto itemCost2 = CCLabelBMFont::create("300", "bigFont.fnt");
    itemCostSpr2->setPosition({-20.f, 35.f});
    itemCost2->setPosition({5.f, 35.f});
    itemCost2->setScale(0.5f);
    item2Menu->addChild(bgItem2);
    item2Menu->addChild(petShadow2);
    item2Menu->addChild(pet2);
    item2Menu->addChild(itemBuyBtn2);
    item2Menu->addChild(itemCostSpr2);
    item2Menu->addChild(itemCost2);
    decoMenu->m_contentLayer->addChild(item2Menu);

    auto item3Menu = CCMenu::create();
    item3Menu->setPosition(280.f, 140.f);
    auto bgItem3 = CCScale9Sprite::create("GJ_square03.png");
    bgItem3->setContentHeight(100.f);
    auto petShadow3 = CCSprite::createWithSpriteFrameName("chest_shadow_001.png");
    petShadow3->setOpacity(50.f);
    petShadow3->setScale(0.5f);
    petShadow3->setPositionY(-10.f);
    auto pet3 = CCSprite::createWithSpriteFrameName("player_01_001.png");
    pet3->setPositionY(5.f);
    auto itemBuyBtn3 = ButtonSprite::create("Buy");
    itemBuyBtn3->setScale(0.75f);
    itemBuyBtn3->setPositionY(-30.f);
    auto itemCostSpr3 = CCSprite::create("moonSpr.png"_spr);
    auto itemCost3 = CCLabelBMFont::create("250", "bigFont.fnt");
    itemCostSpr3->setPosition({-20.f, 35.f});
    itemCost3->setPosition({5.f, 35.f});
    itemCost3->setScale(0.5f);
    item3Menu->addChild(bgItem3);
    item3Menu->addChild(petShadow3);
    item3Menu->addChild(pet3);
    item3Menu->addChild(itemBuyBtn3);
    item3Menu->addChild(itemCostSpr3);
    item3Menu->addChild(itemCost3);
    decoMenu->m_contentLayer->addChild(item3Menu);

    auto item4Menu = CCMenu::create();
    item4Menu->setPosition(40.f, 20.f);
    auto bgItem4 = CCScale9Sprite::create("GJ_square03.png");
    bgItem4->setContentHeight(100.f);
    auto petShadow4 = CCSprite::createWithSpriteFrameName("chest_shadow_001.png");
    petShadow4->setOpacity(50.f);
    petShadow4->setScale(0.5f);
    petShadow4->setPositionY(-10.f);
    auto pet4 = CCSprite::createWithSpriteFrameName("player_01_001.png");
    pet4->setPositionY(5.f);
    auto itemBuyBtn4 = ButtonSprite::create("Buy");
    itemBuyBtn4->setScale(0.75f);
    itemBuyBtn4->setPositionY(-30.f);
    auto itemCostSpr4 = CCSprite::create("moonSpr.png"_spr);
    auto itemCost4 = CCLabelBMFont::create("275", "bigFont.fnt");
    itemCostSpr4->setPosition({-20.f, 35.f});
    itemCost4->setPosition({5.f, 35.f});
    itemCost4->setScale(0.5f);
    item4Menu->addChild(bgItem4);
    item4Menu->addChild(petShadow4);
    item4Menu->addChild(pet4);
    item4Menu->addChild(itemBuyBtn4);
    item4Menu->addChild(itemCostSpr4);
    item4Menu->addChild(itemCost4);
    decoMenu->m_contentLayer->addChild(item4Menu);

    auto item5Menu = CCMenu::create();
    item5Menu->setPosition(160.f, 20.f);
    auto bgItem5 = CCScale9Sprite::create("GJ_square03.png");
    bgItem5->setContentHeight(100.f);
    auto petShadow5 = CCSprite::createWithSpriteFrameName("chest_shadow_001.png");
    petShadow5->setOpacity(50.f);
    petShadow5->setScale(0.5f);
    petShadow5->setPositionY(-10.f);
    auto pet5 = CCSprite::createWithSpriteFrameName("player_01_001.png");
    pet5->setPositionY(5.f);
    auto itemBuyBtn5 = ButtonSprite::create("Buy");
    itemBuyBtn5->setScale(0.75f);
    itemBuyBtn5->setPositionY(-30.f);
    auto itemCostSpr5 = CCSprite::create("moonSpr.png"_spr);
    auto itemCost5 = CCLabelBMFont::create("250", "bigFont.fnt");
    itemCostSpr5->setPosition({-20.f, 35.f});
    itemCost5->setPosition({5.f, 35.f});
    itemCost5->setScale(0.5f);
    item5Menu->addChild(bgItem5);
    item5Menu->addChild(petShadow5);
    item5Menu->addChild(pet5);
    item5Menu->addChild(itemBuyBtn5);
    item5Menu->addChild(itemCostSpr5);
    item5Menu->addChild(itemCost5);
    decoMenu->m_contentLayer->addChild(item5Menu);

    auto item6Menu = CCMenu::create();
    item6Menu->setPosition(280.f, 20.f);
    auto bgItem6 = CCScale9Sprite::create("GJ_square03.png");
    bgItem6->setContentHeight(100.f);
    auto petShadow6 = CCSprite::createWithSpriteFrameName("chest_shadow_001.png");
    petShadow6->setOpacity(50.f);
    petShadow6->setScale(0.5f);
    petShadow6->setPositionY(-10.f);
    auto pet6 = CCSprite::createWithSpriteFrameName("player_01_001.png");
    pet6->setPositionY(5.f);
    auto itemBuyBtn6 = ButtonSprite::create("Buy");
    itemBuyBtn6->setScale(0.75f);
    itemBuyBtn6->setPositionY(-30.f);
    auto itemCostSpr6 = CCSprite::create("moonSpr.png"_spr);
    auto itemCost6 = CCLabelBMFont::create("300", "bigFont.fnt");
    itemCostSpr6->setPosition({-20.f, 35.f});
    itemCost6->setPosition({5.f, 35.f});
    itemCost6->setScale(0.5f);
    item6Menu->addChild(bgItem6);
    item6Menu->addChild(petShadow6);
    item6Menu->addChild(pet6);
    item6Menu->addChild(itemBuyBtn6);
    item6Menu->addChild(itemCostSpr6);
    item6Menu->addChild(itemCost6);
    decoMenu->m_contentLayer->addChild(item6Menu);

    mainLayer->addChild(decoMenu);
}

void PetShop::selectTab(int id) {
    this->m_decoBtn->toggle(id == 0);
    this->m_effectsBtn->toggle(id == 1);
    this->m_repliesBtn->toggle(id == 2);
}

void PetShop::onDecoBtn(CCObject* sender) {
    selectTab(0);
    auto foundDecoMenu = mainLayer->getChildByIDRecursive("deco-menu");
    if (foundDecoMenu) foundDecoMenu->removeFromParent();
    auto decoMenu = ScrollLayer::create({320.f, 200.f}, true, true);
    decoMenu->setID("deco-menu");
    decoMenu->setPositionX(25.f);
    decoMenu->setScale(0.925f);

    auto item1Menu = CCMenu::create();
    item1Menu->setPosition(40.f, 140.f);
    auto bgItem = CCScale9Sprite::create("GJ_square03.png");
    bgItem->setContentHeight(100.f);
    auto petShadow = CCSprite::createWithSpriteFrameName("chest_shadow_001.png");
    petShadow->setOpacity(50.f);
    petShadow->setScale(0.5f);
    petShadow->setPositionY(-10.f);
    auto pet = CCSprite::createWithSpriteFrameName("player_01_001.png");
    pet->setPositionY(5.f);
    auto itemBuyBtn = ButtonSprite::create("Buy");
    itemBuyBtn->setScale(0.75f);
    itemBuyBtn->setPositionY(-30.f);
    auto itemCostSpr = CCSprite::create("moonSpr.png"_spr);
    auto itemCost = CCLabelBMFont::create("250", "bigFont.fnt");
    itemCostSpr->setPosition({-20.f, 35.f});
    itemCost->setPosition({5.f, 35.f});
    itemCost->setScale(0.5f);
    item1Menu->addChild(bgItem);
    item1Menu->addChild(petShadow);
    item1Menu->addChild(pet);
    item1Menu->addChild(itemBuyBtn);
    item1Menu->addChild(itemCostSpr);
    item1Menu->addChild(itemCost);
    decoMenu->m_contentLayer->addChild(item1Menu);

    auto item2Menu = CCMenu::create();
    item2Menu->setPosition(160.f, 140.f);
    auto bgItem2 = CCScale9Sprite::create("GJ_square03.png");
    bgItem2->setContentHeight(100.f);
    auto petShadow2 = CCSprite::createWithSpriteFrameName("chest_shadow_001.png");
    petShadow2->setOpacity(50.f);
    petShadow2->setScale(0.5f);
    petShadow2->setPositionY(-10.f);
    auto pet2 = CCSprite::createWithSpriteFrameName("player_01_001.png");
    pet2->setPositionY(5.f);
    auto itemBuyBtn2 = ButtonSprite::create("Buy");
    itemBuyBtn2->setScale(0.75f);
    itemBuyBtn2->setPositionY(-30.f);
    auto itemCostSpr2 = CCSprite::create("moonSpr.png"_spr);
    auto itemCost2 = CCLabelBMFont::create("300", "bigFont.fnt");
    itemCostSpr2->setPosition({-20.f, 35.f});
    itemCost2->setPosition({5.f, 35.f});
    itemCost2->setScale(0.5f);
    item2Menu->addChild(bgItem2);
    item2Menu->addChild(petShadow2);
    item2Menu->addChild(pet2);
    item2Menu->addChild(itemBuyBtn2);
    item2Menu->addChild(itemCostSpr2);
    item2Menu->addChild(itemCost2);
    decoMenu->m_contentLayer->addChild(item2Menu);

    auto item3Menu = CCMenu::create();
    item3Menu->setPosition(280.f, 140.f);
    auto bgItem3 = CCScale9Sprite::create("GJ_square03.png");
    bgItem3->setContentHeight(100.f);
    auto petShadow3 = CCSprite::createWithSpriteFrameName("chest_shadow_001.png");
    petShadow3->setOpacity(50.f);
    petShadow3->setScale(0.5f);
    petShadow3->setPositionY(-10.f);
    auto pet3 = CCSprite::createWithSpriteFrameName("player_01_001.png");
    pet3->setPositionY(5.f);
    auto itemBuyBtn3 = ButtonSprite::create("Buy");
    itemBuyBtn3->setScale(0.75f);
    itemBuyBtn3->setPositionY(-30.f);
    auto itemCostSpr3 = CCSprite::create("moonSpr.png"_spr);
    auto itemCost3 = CCLabelBMFont::create("250", "bigFont.fnt");
    itemCostSpr3->setPosition({-20.f, 35.f});
    itemCost3->setPosition({5.f, 35.f});
    itemCost3->setScale(0.5f);
    item3Menu->addChild(bgItem3);
    item3Menu->addChild(petShadow3);
    item3Menu->addChild(pet3);
    item3Menu->addChild(itemBuyBtn3);
    item3Menu->addChild(itemCostSpr3);
    item3Menu->addChild(itemCost3);
    decoMenu->m_contentLayer->addChild(item3Menu);

    auto item4Menu = CCMenu::create();
    item4Menu->setPosition(40.f, 20.f);
    auto bgItem4 = CCScale9Sprite::create("GJ_square03.png");
    bgItem4->setContentHeight(100.f);
    auto petShadow4 = CCSprite::createWithSpriteFrameName("chest_shadow_001.png");
    petShadow4->setOpacity(50.f);
    petShadow4->setScale(0.5f);
    petShadow4->setPositionY(-10.f);
    auto pet4 = CCSprite::createWithSpriteFrameName("player_01_001.png");
    pet4->setPositionY(5.f);
    auto itemBuyBtn4 = ButtonSprite::create("Buy");
    itemBuyBtn4->setScale(0.75f);
    itemBuyBtn4->setPositionY(-30.f);
    auto itemCostSpr4 = CCSprite::create("moonSpr.png"_spr);
    auto itemCost4 = CCLabelBMFont::create("275", "bigFont.fnt");
    itemCostSpr4->setPosition({-20.f, 35.f});
    itemCost4->setPosition({5.f, 35.f});
    itemCost4->setScale(0.5f);
    item4Menu->addChild(bgItem4);
    item4Menu->addChild(petShadow4);
    item4Menu->addChild(pet4);
    item4Menu->addChild(itemBuyBtn4);
    item4Menu->addChild(itemCostSpr4);
    item4Menu->addChild(itemCost4);
    decoMenu->m_contentLayer->addChild(item4Menu);

    auto item5Menu = CCMenu::create();
    item5Menu->setPosition(160.f, 20.f);
    auto bgItem5 = CCScale9Sprite::create("GJ_square03.png");
    bgItem5->setContentHeight(100.f);
    auto petShadow5 = CCSprite::createWithSpriteFrameName("chest_shadow_001.png");
    petShadow5->setOpacity(50.f);
    petShadow5->setScale(0.5f);
    petShadow5->setPositionY(-10.f);
    auto pet5 = CCSprite::createWithSpriteFrameName("player_01_001.png");
    pet5->setPositionY(5.f);
    auto itemBuyBtn5 = ButtonSprite::create("Buy");
    itemBuyBtn5->setScale(0.75f);
    itemBuyBtn5->setPositionY(-30.f);
    auto itemCostSpr5 = CCSprite::create("moonSpr.png"_spr);
    auto itemCost5 = CCLabelBMFont::create("250", "bigFont.fnt");
    itemCostSpr5->setPosition({-20.f, 35.f});
    itemCost5->setPosition({5.f, 35.f});
    itemCost5->setScale(0.5f);
    item5Menu->addChild(bgItem5);
    item5Menu->addChild(petShadow5);
    item5Menu->addChild(pet5);
    item5Menu->addChild(itemBuyBtn5);
    item5Menu->addChild(itemCostSpr5);
    item5Menu->addChild(itemCost5);
    decoMenu->m_contentLayer->addChild(item5Menu);

    auto item6Menu = CCMenu::create();
    item6Menu->setPosition(280.f, 20.f);
    auto bgItem6 = CCScale9Sprite::create("GJ_square03.png");
    bgItem6->setContentHeight(100.f);
    auto petShadow6 = CCSprite::createWithSpriteFrameName("chest_shadow_001.png");
    petShadow6->setOpacity(50.f);
    petShadow6->setScale(0.5f);
    petShadow6->setPositionY(-10.f);
    auto pet6 = CCSprite::createWithSpriteFrameName("player_01_001.png");
    pet6->setPositionY(5.f);
    auto itemBuyBtn6 = ButtonSprite::create("Buy");
    itemBuyBtn6->setScale(0.75f);
    itemBuyBtn6->setPositionY(-30.f);
    auto itemCostSpr6 = CCSprite::create("moonSpr.png"_spr);
    auto itemCost6 = CCLabelBMFont::create("300", "bigFont.fnt");
    itemCostSpr6->setPosition({-20.f, 35.f});
    itemCost6->setPosition({5.f, 35.f});
    itemCost6->setScale(0.5f);
    item6Menu->addChild(bgItem6);
    item6Menu->addChild(petShadow6);
    item6Menu->addChild(pet6);
    item6Menu->addChild(itemBuyBtn6);
    item6Menu->addChild(itemCostSpr6);
    item6Menu->addChild(itemCost6);
    decoMenu->m_contentLayer->addChild(item6Menu);

    mainLayer->addChild(decoMenu);
}

void PetShop::onEffectsBtn(CCObject* sender) {
    selectTab(1);
    auto foundDecoMenu = mainLayer->getChildByIDRecursive("deco-menu");
    if (foundDecoMenu) foundDecoMenu->removeFromParent();
}

void PetShop::onRepliesBtn(CCObject* sender) {
    selectTab(2);
    auto foundDecoMenu = mainLayer->getChildByIDRecursive("deco-menu");
    if (foundDecoMenu) foundDecoMenu->removeFromParent();
}

PetShop* PetShop::create() {
    auto ret = new PetShop();

    if (ret && ret->initAnchored(370.f, 230.f, "Pet Shop")) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}