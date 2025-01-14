/*
** EPITECH PROJECT, 2023
** safe
** File description:
** GamePool.hpp
*/

#include "ClientUDP.hpp"
#include "IGame.hpp"
#include "ServerUdp.hpp"
#include "States.hpp"

#pragma once
namespace eng {
    class Engine;

    struct SessionData {
    };

    class GamePool : public eng::IGame {

        void Init(Engine* engine) override;
        void Cleanup(Engine*) override;
        bool IsOnLine(Engine*) override;
        void WaitConnect(Engine*) override;
        void LoadFirstScene(Engine*) override;
        void PreSceneInstantiationHook(Engine*, const std::string& sceneName) override;
        void ModPipeline(Engine*) override;

        inline SessionData& GetSessionData() { return _sessionData; }

    private:
        // =========================================================================================================== //
        // =============================================== NETWORK =================================================== //
        // =========================================================================================================== //

        // =========================================================================================================== //
        // =============================================== SCENE ===================================================== //
        // =========================================================================================================== //

        void initRace();

        // =========================================================================================================== //
        // =============================================== MEMBERS =================================================== //
        // =========================================================================================================== //

        SessionData _sessionData = {};
        Engine* _engine = nullptr;
        serv::ServerUDP* _server = nullptr;
        serv::ClientUDP* _client = nullptr;
        ecs::States _stateMachine;
    };
}