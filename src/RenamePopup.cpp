#include <Geode/Geode.hpp>
#include "RenamePopup.hpp"
#include "PetUtils.hpp"

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

void RenamePopup::onOK(CCObject*) {
    if (!m_input) return;

    web::WebRequest req;
    matjson::Value body;
    body["account_id"] = PetUtils::accountID;
    matjson::Value updates;
    updates["pet_name"] = m_input->getString();
    body["updates"] = updates;
    req.bodyJSON(body);

    auto task = req.patch("https://delivel.tech/petapi/update_user");
    PetUtils::m_nameListener.bind([](web::WebTask::Event* e) {
        if (web::WebResponse* value = e->getValue()) {
            if (value->ok()) {
                Notification::create("Successfully set a new name!", NotificationIcon::Success)->show();
            } else {
                Notification::create("Something went wrong", NotificationIcon::Error)->show();
            }
        }
    });
    PetUtils::m_nameListener.setFilter(task);

    this->onClose(nullptr);
}
