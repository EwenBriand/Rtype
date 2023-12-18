#include "GameRtype.hpp"
#include "Engine.hpp"
#include "SceneManager.hpp"
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
    }

    // =========================================================================================================== //
    // ============================================== PRIVATE ==================================================== //
    // =========================================================================================================== //

} // namespace eng