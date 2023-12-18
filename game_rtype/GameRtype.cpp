#include "GameRtype.hpp"
#include "ECSImpl.hpp"
#include "Engine.hpp"
#include "LocalPlayerController.hpp"
#include "SceneManager.hpp"
#include "Ship.hpp"
#include <iostream>
#include <memory>

extern "C" {
std::shared_ptr<eng::IGame> create()
{
    std::cout << "Loading game_rtype" << std::endl;
    return std::make_shared<eng::RType>();
}
}

namespace eng {

    // =========================================================================================================== //
    // =============================================== PUBLIC ==================================================== //
    // =========================================================================================================== //

    void RType::Init(eng::Engine* e)
    {
        loadDependencies(e);
    }

    void RType::Cleanup(eng::Engine* e)
    {
    }

    bool RType::IsOnLine(eng::Engine* e)
    {
        return false;
    }

    void RType::WaitConnect(eng::Engine* e)
    {
    }

    void RType::LoadFirstScene(eng::Engine* e)
    {
        e->GetSceneManager().SwitchScene("menu");
    }

    void RType::PreSceneInstantiationHook(eng::Engine* e, const std::string& sceneName)
    {
        if (sceneName == "menu")
            return;

        // update this to account for networking, other players, etc
        int player = SYS.GetResourceManager().LoadPrefab("ship");
        Ship* ship = GetUComponent(player, Ship);
        ship->Possess(player, std::make_shared<LocalPlayerController>());
    }

    // =========================================================================================================== //
    // ============================================== PRIVATE ==================================================== //
    // =========================================================================================================== //

    void RType::loadDependencies(Engine*)
    {
    }

} // namespace eng