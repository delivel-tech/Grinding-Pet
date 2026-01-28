#include "utils/PetUtils.hpp"
#include "layers/PetLayer.hpp"
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

Task<void> PetUtils::upgradeRarenessToRare() {
    auto popup = UploadActionPopup::create(nullptr, "Upgrading rareness...");
    popup->show();
    co_await PetUtils::getUser();
    web::WebRequest req;
    matjson::Value body;
    body["account_id"] = PetUtils::accountID;
    body["authtoken"] = Mod::get()->getSavedValue<std::string>("argon-token");
    matjson::Value updates;
    updates["pet_rareness"] = "Rare";
    updates["delta_moons"] = -500;
    body["updates"] = updates;

    req.bodyJSON(body);

    auto response = co_await req.patch("https://delivel.tech/petapi/adv_update_user");

    if (!response.ok()) {
        popup->showFailMessage("Upgrading failed.");
        co_return;
    }
    popup->showSuccessMessage("Rareness upgraded!");
    coro::spawn << PetLayer::runSyncFlow();

    co_return;
}

Task<void> PetUtils::upgradeRarenessToEpic() {
    auto popup = UploadActionPopup::create(nullptr, "Upgrading rareness...");
    popup->show();
    co_await PetUtils::getUser();
    web::WebRequest req;
    matjson::Value body;
    body["account_id"] = PetUtils::accountID;
    body["authtoken"] = Mod::get()->getSavedValue<std::string>("argon-token");
    matjson::Value updates;
    updates["pet_rareness"] = "Epic";
    updates["delta_moons"] = -3000;
    body["updates"] = updates;

    req.bodyJSON(body);

    auto response = co_await req.patch("https://delivel.tech/petapi/adv_update_user");

    if (!response.ok()) {
        popup->showFailMessage("Upgrading failed.");
        co_return;
    }
    popup->showSuccessMessage("Rareness upgraded!");
    coro::spawn << PetLayer::runSyncFlow();

    co_return;
}

Task<void> PetUtils::upgradeRarenessToMythic() {
    auto popup = UploadActionPopup::create(nullptr, "Upgrading rareness...");
    popup->show();
    co_await PetUtils::getUser();
    web::WebRequest req;
    matjson::Value body;
    body["account_id"] = PetUtils::accountID;
    body["authtoken"] = Mod::get()->getSavedValue<std::string>("argon-token");
    matjson::Value updates;
    updates["pet_rareness"] = "Mythic";
    updates["delta_moons"] = -10000;
    body["updates"] = updates;

    req.bodyJSON(body);

    auto response = co_await req.patch("https://delivel.tech/petapi/adv_update_user");

    if (!response.ok()) {
        popup->showFailMessage("Upgrading failed.");
        co_return;
    }
    popup->showSuccessMessage("Rareness upgraded!");
    coro::spawn << PetLayer::runSyncFlow();

    co_return;
}

Task<void> PetUtils::upgradeLevelBy(int level, int stars) {
    auto popup = UploadActionPopup::create(nullptr, "Upgrading pet...");
    popup->show();
    co_await PetUtils::getUser();
    web::WebRequest req;
    matjson::Value body;
    body["account_id"] = PetUtils::accountID;
    body["authtoken"] = Mod::get()->getSavedValue<std::string>("argon-token");
    matjson::Value updates;
    updates["delta_pet_level"] = level;
    updates["delta_stars"] = stars;
    body["updates"] = updates;

    req.bodyJSON(body);

    auto response = co_await req.patch("https://delivel.tech/petapi/adv_update_user");

    if (!response.ok()) {
        popup->showFailMessage("Upgrading failed.");
        co_return;
    }
    popup->showSuccessMessage("Pet upgraded!");
    coro::spawn << PetLayer::runSyncFlow();

    co_return;
}

Task<void> PetUtils::checkStats() {
    Mod::get()->setSavedValue("last-stars", Mod::get()->getSavedValue<int>("current-stars"));
    Mod::get()->setSavedValue("last-moons", Mod::get()->getSavedValue<int>("current-moons"));
    PetUtils::getTotalStats();

    int deltaStars = 0;
    int deltaMoons = 0;

    if (Mod::get()->getSavedValue<int>("current-stars") != Mod::get()->getSavedValue<int>("last-stars")) {
        deltaStars = Mod::get()->getSavedValue<int>("current-stars") - Mod::get()->getSavedValue<int>("last-stars");
        Mod::get()->setSavedValue("delta-stars", deltaStars);
    }

    if (Mod::get()->getSavedValue<int>("current-moons") != Mod::get()->getSavedValue<int>("last-moons")) {
        deltaMoons = Mod::get()->getSavedValue<int>("current-moons") - Mod::get()->getSavedValue<int>("last-moons");
        Mod::get()->setSavedValue("delta-moons", deltaMoons);
    }

    web::WebRequest req;
    matjson::Value body;
    body["account_id"] = PetUtils::accountID;
    body["authtoken"] = Mod::get()->getSavedValue<std::string>("argon-token");
    
    matjson::Value updates;
    if (deltaStars > 0) {
        updates["delta_stars"] = deltaStars;
    }
    if (deltaMoons > 0) {
        updates["delta_moons"] = deltaMoons;
    }
    
    body["updates"] = updates;
    req.bodyJSON(body);
    
    auto response = co_await req.patch("https://delivel.tech/petapi/adv_update_user");

    if (!response.ok()) {
        Notification::create("[Grinding Pet] Something went wrong...", NotificationIcon::Error)->show();
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
    Mod::get()->setSavedValue("pet-rareness", user["pet_rareness"].asString().unwrapOrDefault());
    Mod::get()->setSavedValue("is-banned", user["isBanned"].asInt().unwrapOrDefault());
    Mod::get()->setSavedValue("ban-reason", user["ban_reason"].asString().unwrapOrDefault());

    if (deltaMoons <= 0 && deltaStars <= 0) {
        Notification::create("[Grinding Pet] No stats updated", NotificationIcon::Info)->show();
        co_return;
    }
    
    Notification::create("[Grinding Pet] Successfully synced!", NotificationIcon::Success)->show();
    
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
    Mod::get()->setSavedValue("pet-rareness", user["pet_rareness"].asString().unwrapOrDefault());
    Mod::get()->setSavedValue("is-banned", user["isBanned"].asInt().unwrapOrDefault());
    Mod::get()->setSavedValue("ban-reason", user["ban_reason"].asString().unwrapOrDefault());

    co_return;
}

