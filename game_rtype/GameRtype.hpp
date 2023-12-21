#include "ClientUDP.hpp"
#include "IGame.hpp"
#include "ServerUdp.hpp"
#include "States.hpp"

namespace eng {
    class Engine;
    class RType : public eng::IGame {
    public:
        RType() = default;
        ~RType() = default;

        void Init(Engine*) override;
        void Cleanup(Engine*) override;
        bool IsOnLine(Engine*) override;
        void WaitConnect(Engine*) override;
        void LoadFirstScene(Engine*) override;
        void PreSceneInstantiationHook(Engine*, const std::string& sceneName) override;
        void ModPipeline(Engine*) override;

    private:
        /**
         * @brief Loads any dependencies that the game might need
         *
         */
        void loadDependencies(Engine*);

        /**
         * @brief Inits the game's connection to the server, or the server itself
         * if this instance of the game is the server.
         *
         */
        void initServerConnection(Engine*);

        void startServer(Engine* e);
        void connectToServer(Engine* e);

        bool m_isServer = false;
        std::shared_ptr<serv::ServerUDP> m_server = nullptr;
        std::shared_ptr<serv::ClientUDP> m_client = nullptr;

        ecs::States _stateMachine;
    };
} // namespace eng