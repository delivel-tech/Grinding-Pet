#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

class PetUtils {
public:
    static void getTotalStats();
    static geode::Task<void> checkStats();

    static int accountID;
    static std::string username;

    static geode::Task<void> getUser();
    static geode::Task<void> newCreateUser();
    static geode::Task<void> upgradeLevelBy(int level, int stars);
    static geode::Task<void> upgradeRarenessToRare();
    static geode::Task<void> upgradeRarenessToEpic();
    static geode::Task<void> upgradeRarenessToMythic();
};

class PetReadyEvent : public Event {};
