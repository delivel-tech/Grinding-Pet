#include <Geode/Geode.hpp>
#include "RenamePopup.hpp"
#include "PetUtils.hpp"
#include <Geode/utils/coro.hpp>

using namespace geode::prelude;

bool RenamePopup::setup(std::string const& value) {
    this->setTitle("Rename Pet", "goldFont.fnt", 0.8f);

    m_input = TextInput::create(160.f, "Enter name", "bigFont.fnt");
    m_input->setMaxCharCount(20);
    m_input->setString(value);

    auto size = m_mainLayer->getContentSize();
    m_input->setPosition({ size.width / 2.f, size.height / 2.f + 5.f });
    m_mainLayer->addChild(m_input);

    auto okBtn = CCMenuItemSpriteExtra::create(
        ButtonSprite::create("Submit"),
        this,
        menu_selector(RenamePopup::onOK)
    );

    okBtn->setPositionY(-10.f);

    auto menu = CCMenu::create();
    menu->addChild(okBtn);
    menu->setPosition({ size.width / 2.f, size.height / 2.f - 28.f });
    m_mainLayer->addChild(menu);

    return true;
}

RenamePopup* RenamePopup::create(std::string const& currentName) {
    auto ret = new RenamePopup();

    if (ret && ret->initAnchored(220.f, 140.f, currentName)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

Task<void> RenamePopup::onOKTask() {
    if (!m_input) co_return;

    auto popup = UploadActionPopup::create(nullptr, "Changing pet's name...");
    popup->show();

    web::WebRequest req;
    matjson::Value body;
    body["account_id"] = PetUtils::accountID;
    body["authtoken"] = Mod::get()->getSavedValue<std::string>("argon-token");
    matjson::Value updates;
    updates["pet_name"] = m_input->getString();
    body["updates"] = updates;
    req.bodyJSON(body);

    auto response = co_await req.patch("https://delivel.tech/petapi/update_user");
    
    if (response.ok()) {
        popup->showSuccessMessage("Pet's name changed!");
    } else {
        popup->showFailMessage("Something went wrong.");
    }
    co_return;
}

void RenamePopup::onOK(CCObject*) {
    coro::spawn << onOKTask();
}
