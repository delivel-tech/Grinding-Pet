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
    static geode::Task<void> upgradeLevelTo2();
    static geode::Task<void> upgradeLevelTo3();
    static geode::Task<void> upgradeLevelTo4();
    static geode::Task<void> upgradeLevelTo5();
};

class PetReadyEvent : public Event {};
