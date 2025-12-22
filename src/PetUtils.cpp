#include "PetUtils.hpp"
#include <Geode/utils/web.hpp>
#include <Geode/utils/coro.hpp>

using namespace geode::prelude;

void PetUtils::getTotalStats() {
    auto gsm = GameStatsManager::sharedState();
    auto stars = gsm->getStat("6");
    auto moons = gsm->getStat("28");
    Mod::get()->setSavedValue("current-stars", stars);
    Mod::get()->setSavedValue("current-moons", moons);
}

Task<void> PetUtils::checkStats() {
    Mod::get()->setSavedValue("last-stars", Mod::get()->getSavedValue<int>("current-stars"));
    Mod::get()->setSavedValue("last-moons", Mod::get()->getSavedValue<int>("current-moons"));
    PetUtils::getTotalStats();
    co_await PetUtils::getUser();

    if (Mod::get()->getSavedValue<int>("current-stars") != Mod::get()->getSavedValue<int>("last-stars")) {
        Mod::get()->setSavedValue("delta-stars", Mod::get()->getSavedValue<int>("current-stars") - Mod::get()->getSavedValue<int>("last-stars"));
    }

    if (Mod::get()->getSavedValue<int>("current-moons") != Mod::get()->getSavedValue<int>("last-moons")) {
        Mod::get()->setSavedValue("delta-moons", Mod::get()->getSavedValue<int>("current-moons") - Mod::get()->getSavedValue<int>("last-moons"));
    }

    if (Mod::get()->getSavedValue<int>("current-moons") != Mod::get()->getSavedValue<int>("last-moons") || Mod::get()->getSavedValue<int>("current-stars") != Mod::get()->getSavedValue<int>("last-stars")) {
        web::WebRequest req;
        matjson::Value body;
        body["account_id"] = PetUtils::accountID;
        body["authtoken"] = Mod::get()->getSavedValue<std::string>("argon-token");
        matjson::Value updates;
        if (Mod::get()->getSavedValue<int>("current-stars") != Mod::get()->getSavedValue<int>("last-stars")) {
            updates["pet_stars"] = Mod::get()->getSavedValue<int>("pet-stars") + Mod::get()->getSavedValue<int>("delta-stars");
        }
        if (Mod::get()->getSavedValue<int>("current-moons") != Mod::get()->getSavedValue<int>("last-moons")) {
            updates["pet_moons"] = Mod::get()->getSavedValue<int>("pet-moons") + Mod::get()->getSavedValue<int>("delta-moons");
        }
        body["updates"] = updates;

        req.bodyJSON(body);
        auto response = co_await req.patch("https://delivel.tech/petapi/update_user");

        if (!response.ok()) {
            co_return;
        }
    }
    co_await PetUtils::getUser();
    co_return;
}

int PetUtils::accountID = GJAccountManager::get()->m_accountID;
std::string PetUtils::username = GJAccountManager::get()->m_username;

Task<void> PetUtils::newCreateUser() {
    web::WebRequest req;

    matjson::Value body;
    body["account_id"] = PetUtils::accountID;
    body["authtoken"] = Mod::get()->getSavedValue<std::string>("argon-token");
    body["username"] = PetUtils::username;

    log::info("POST check_create_user: account_id={}, username='{}'",
        PetUtils::accountID, PetUtils::username
    );

    req.bodyJSON(body);

    auto response = co_await req.post("https://delivel.tech/petapi/check_create_user");

    if (!response.ok()) {
        co_return;
    }
}

Task<void> PetUtils::getUser() {
    web::WebRequest req;

    auto response = co_await req.get(fmt::format("https://delivel.tech/petapi/get_user?account_id={}", PetUtils::accountID));

    if (!response.ok()) {
        log::error("getUser request failed");
        co_return;
    }
    auto jsonRes = response.json();
    if (!jsonRes) {
        log::error("invalid json");
        co_return;
    }
    auto json = jsonRes.unwrap();
    auto user = json["user"];

    Mod::get()->setSavedValue("pet-stars", user["pet_stars"].asInt().unwrapOrDefault());
    Mod::get()->setSavedValue("pet-moons", user["pet_moons"].asInt().unwrapOrDefault());
    Mod::get()->setSavedValue("pet-name", user["pet_name"].asString().unwrapOrDefault());
    Mod::get()->setSavedValue("is-admin", user["isAdmin"].asInt().unwrapOrDefault());
    Mod::get()->setSavedValue("pet-level", user["pet_level"].asInt().unwrapOrDefault());

    co_return;
}

