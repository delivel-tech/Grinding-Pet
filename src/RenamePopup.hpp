#pragma once
#include <Geode/Geode.hpp>

class RenamePopup : public geode::Popup<std::string const&> {
protected:
    bool setup(std::string const& value) override;

public:
    static RenamePopup* create(
        std::string const& currentName,
        std::function<void(std::string)> onDone
    );

    void onOK(cocos2d::CCObject*);

private:
    geode::TextInput* m_input = nullptr;
    std::function<void(std::string)> m_onDone;
};
