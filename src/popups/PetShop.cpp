#include <Geode/Geode.hpp>
#include "popups/PetShop.hpp"

using namespace geode::prelude;

bool PetShop::setup(std::string const& value) {
    this->setTitle("Pet Shop", "goldFont.fnt", 0.8f);

    auto size = m_mainLayer->getContentSize();
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