#pragma once
#include <Geode/Geode.hpp>

class PetShop : public geode::Popup<std::string const&> {
    public:
    bool setup(std::string const& value) override;
    static PetShop* create();
};