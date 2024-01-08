/*
** EPITECH PROJECT, 2023
** safe
** File description:
** GamePool.cpp
*/

#include "GamePool.hpp"
#include "Car.hpp"
#include "Components.Vanilla/UIButton.hpp"
#include "Engine.hpp"
#include "LocalCarController.hpp"
#include "RLCamera3D.hpp"

extern "C" {
std::shared_ptr<eng::IGame> create()
{
    std::cout << "Loading game_pool" << std::endl;
    return std::make_shared<eng::GamePool>();
}
}

namespace eng {

    // =========================================================================================================== //
    // =============================================== PUBLIC ==================================================== //
    // =========================================================================================================== //

    void GamePool::Init(Engine* engine)
    {
        _engine = engine;
        UIButton::RegisterCallback("pool::start", [this, engine]() {
            engine->GetSceneManager().SwitchScene("race");
        });
        engine->SetGlobal<int>("mapSize", 10);
    }

    void GamePool::Cleanup(Engine*)
    {
    }

    bool GamePool::IsOnLine(Engine*)
    {
        return false;
    }

    void GamePool::WaitConnect(Engine*)
    {
    }

    void GamePool::LoadFirstScene(Engine* e)
    {
        e->GetSceneManager().SwitchScene("menu");
    }

    void GamePool::PreSceneInstantiationHook(Engine* e, const std::string& sceneName)
    {
        std::cout << "pre scene instantiation hook start" << std::endl;

        if (sceneName == "race")
            initRace();
    }

    void GamePool::ModPipeline(Engine* engine)
    {
    }

    // =========================================================================================================== //
    // ============================================== PRIVATE ==================================================== //
    // =========================================================================================================== //

    void GamePool::initRace()
    {
        auto entities = SYS.GetEntities();
        int cameraID = -1;
        for (auto& entity : entities) {
            try {
                SYS.GetComponent<RLCamera3D>(entity, "RLCamera3D").OnLoad();
                cameraID = entity;
            } catch (std::exception& e) {
                continue;
            }
        }

        try {
            int carID = SYS.GetResourceManager().LoadPrefab("car");
            auto& car = SYS.GetComponent<Car>(carID, "Car");
            std::shared_ptr<LocalCarController> controller = std::make_shared<LocalCarController>();

            car.Possess(carID, controller);
            SYS.AddComponent<Tag>(carID).Set("car");

            if (cameraID != -1) {
                auto& camera = SYS.GetComponent<RLCamera3D>(cameraID, "RLCamera3D");
                camera.SetTarget(carID);
                camera.SetTargetOffset(0, 2, 5);
            }
        } catch (std::exception& e) {
            std::cout << "Error while starting race: " << e.what() << std::endl;
        }
    }
}