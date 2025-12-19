#include <Geode/Geode.hpp>
#include "PetUtils.hpp"

using namespace geode::prelude;

void PetUtils::getTotalStars() {
    auto gsm = GameStatsManager::sharedState();
    auto stars = gsm->getStat("6");
    Mod::get()->setSavedValue("current-stars", stars);
}

void PetUtils::checkStars() {
    Mod::get()->setSavedValue("last-stars", Mod::get()->getSavedValue<int>("current-stars"));
    PetUtils::getTotalStars();

    if (Mod::get()->getSavedValue<int>("current-stars") != Mod::get()->getSavedValue<int>("last-stars")) {
        auto delta = Mod::get()->getSavedValue<int>("current-stars") - Mod::get()->getSavedValue<int>("last-stars");
        Mod::get()->setSavedValue("starAmountV", Mod::get()->getSavedValue<int>("starAmountV") + delta);
    }
}