#include <Geode/Geode.hpp>
#include "RenamePopup.hpp"

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
        ButtonSprite::create("OK"),
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

RenamePopup* RenamePopup::create(std::string const& currentName, std::function<void(std::string)> onDone) {
    auto ret = new RenamePopup();
    ret->m_onDone = std::move(onDone);

    if (ret && ret->initAnchored(220.f, 140.f, currentName)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

void RenamePopup::onOK(CCObject*) {
    if (!m_input) return;

    std::string newName = m_input->getString();

    if (m_onDone) {
        m_onDone(std::move(newName));
    }

    Notification::create("Successfully set a new name!", NotificationIcon::Success)->show();
    this->onClose(nullptr);
}
