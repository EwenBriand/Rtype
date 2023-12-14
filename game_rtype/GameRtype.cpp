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
} // namespace eng