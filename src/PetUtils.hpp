#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

class PetUtils {
public:
    static void getTotalStats();
    static void checkStats();
    static EventListener<web::WebTask> m_listener;
    static EventListener<web::WebTask> m_listener1;
    static EventListener<web::WebTask> m_listener2;
    static EventListener<web::WebTask> m_listener3;
    static EventListener<web::WebTask> m_nameListener;

    static bool is_ready1;
    static bool is_ready2;

    static int accountID;
    static std::string username;

    static void getUser();
    static void newCreateUser();
};

class PetReadyEvent : public Event {};
