
/*
** @file Player.cpp
** @brief Implementation of the Player class.
** @date 2023-12-12 15:52:38.
**
*/

#include "Player.hpp"

MANAGED_RESOURCE(Player)

void Player::Start()
{
    std::cout << "Player Start" << std::endl;
    if (!eng::Engine::GetEngine()->IsOptionSet(eng::Engine::Options::EDITOR))
        return;
    Entity e = Sys.GetSystemHolder();
    CLI& cli = Sys.GetComponent<CLI>(e);
    Animation anim("animations/ship.anim");
    try {
        Animator* animator = &Sys.SafeGet<Animator>(cli);
        animator->AddAnimation("ship", anim);
        animator->Play("ship");
    } catch (std::exception& e) {
        Console::err << "Error in adding a anim : " << e.what() << std::endl;
    }

    try {
        AudioSource audio = Sys.SafeGet<AudioSource>(cli);
        audio.AddMusicName("Muse/gas_gas_gas.ogg");
        audio.Play<Music>("Muse/gas_gas_gas.ogg");
    } catch (std::exception& e) {
        Console::err << "Error in adding a anim : " << e.what() << std::endl;
    }
}

void Player::Update(int entityID)
{
}

void Player::OnLoad()
{
    std::cout << "Player OnLoad" << std::endl;
}
