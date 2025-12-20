#include "PetUtils.hpp"
#include <Geode/utils/web.hpp>

using namespace geode::prelude;

void PetUtils::getTotalStats() {
    auto gsm = GameStatsManager::sharedState();
    auto stars = gsm->getStat("6");
    auto moons = gsm->getStat("28");
    Mod::get()->setSavedValue("current-stars", stars);
    Mod::get()->setSavedValue("current-moons", moons);
}

void PetUtils::checkStats() {
    Mod::get()->setSavedValue("last-stars", Mod::get()->getSavedValue<int>("current-stars"));
    Mod::get()->setSavedValue("last-moons", Mod::get()->getSavedValue<int>("current-moons"));
    PetUtils::getTotalStats();

    if (Mod::get()->getSavedValue<int>("current-stars") != Mod::get()->getSavedValue<int>("last-stars")) {
        auto delta = Mod::get()->getSavedValue<int>("current-stars") - Mod::get()->getSavedValue<int>("last-stars");

        PetUtils::getUser();
        
        web::WebRequest req;
        matjson::Value body;
        body["account_id"] = PetUtils::accountID;
        matjson::Value updates;
        updates["pet_stars"] = Mod::get()->getSavedValue<int>("pet-stars") + delta;
        body["updates"] = updates;

        req.bodyJSON(body);

        auto task = req.patch("https://delivel.tech/petapi/update_user");

        PetUtils::m_listener2.bind([](web::WebTask::Event* e) {
        if (web::WebResponse* value = e->getValue()) {
            if (value->ok()) {
                PetUtils::is_ready2 = true;
            } else {
                PetUtils::is_ready2 = true;
            }
        }
        });
        PetUtils::m_listener2.setFilter(task);

        PetUtils::getUser();
    }

    if (Mod::get()->getSavedValue<int>("current-moons") != Mod::get()->getSavedValue<int>("last-moons")) {
        auto delta = Mod::get()->getSavedValue<int>("current-moons") - Mod::get()->getSavedValue<int>("last-moons");

        PetUtils::getUser();
        
        web::WebRequest req;
        matjson::Value body;
        body["account_id"] = PetUtils::accountID;
        matjson::Value updates;
        updates["pet_moons"] = Mod::get()->getSavedValue<int>("pet-moons") + delta;
        body["updates"] = updates;

        req.bodyJSON(body);
        auto task = req.patch("https://delivel.tech/petapi/update_user");
        PetUtils::m_listener3.bind([](web::WebTask::Event* e) {
        if (web::WebResponse* value = e->getValue()) {
            if (value->ok()) {
                is_ready2 = true;
            } else {
                PetUtils::is_ready2 = true;
            }
        }
    });
    PetUtils::m_listener3.setFilter(task);
    PetUtils::getUser();
    }
    PetUtils::is_ready2 = true;
}

int PetUtils::accountID = GJAccountManager::get()->m_accountID;
std::string PetUtils::username = GJAccountManager::get()->m_username;
EventListener<web::WebTask> PetUtils::m_listener;
EventListener<web::WebTask> PetUtils::m_listener1;
EventListener<web::WebTask> PetUtils::m_listener2;
EventListener<web::WebTask> PetUtils::m_listener3;
EventListener<web::WebTask> PetUtils::m_nameListener;
bool PetUtils::is_ready1;
bool PetUtils::is_ready2;

void PetUtils::newCreateUser() {
    web::WebRequest req;

    matjson::Value body;
    body["account_id"] = PetUtils::accountID;
    body["username"] = PetUtils::username;

    log::info("POST check_create_user: account_id={}, username='{}'",
        PetUtils::accountID, PetUtils::username
    );

    req.bodyJSON(body);

    auto task = req.post("https://delivel.tech/petapi/check_create_user");

    PetUtils::m_listener1.bind([](web::WebTask::Event* e) {
        if (web::WebResponse* value = e->getValue()) {
            if (value->ok()) {
                PetUtils::is_ready1 = true;
            } else {
                log::error("req failed");
            }
        }
    });
    PetUtils::m_listener1.setFilter(task);
}

void PetUtils::getUser() {
    web::WebRequest req;

    auto task = req.get(fmt::format("https://delivel.tech/petapi/get_user?account_id={}", PetUtils::accountID));

    PetUtils::m_listener.bind([](web::WebTask::Event* e) {

        if (web::WebResponse* value = e->getValue()) {
            if (value->ok()) {
            auto jsonRes = value->json();
            auto json = jsonRes.unwrap();
            auto user = json["user"];

            Mod::get()->setSavedValue("pet-stars", user["pet_stars"].asInt().unwrapOrDefault());
            Mod::get()->setSavedValue("pet-moons", user["pet_moons"].asInt().unwrapOrDefault());
            Mod::get()->setSavedValue("pet-name", user["pet_name"].asString().unwrapOrDefault());
            Mod::get()->setSavedValue("isAdmin", user["isAdmin"].asInt().unwrapOrDefault());
            } else {
                log::error("req failed");
            }
        }
    });
    PetUtils::m_listener.setFilter(task);
}

