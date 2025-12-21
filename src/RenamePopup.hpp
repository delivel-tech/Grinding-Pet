#pragma once
#include <Geode/Geode.hpp>

class RenamePopup : public geode::Popup<std::string const&> {
protected:
    bool setup(std::string const& value) override;

public:
    static RenamePopup* create(
        std::string const& currentName
    );

    geode::Task<void> onOKTask();
    void onOK(CCObject*);

private:
    geode::TextInput* m_input = nullptr;
};
