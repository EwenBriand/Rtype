
/*
** @file Player.hpp
** @brief Definition of the Player class.
** @date 2023-12-12 15:52:38.
**
*/

#pragma once

#include "../Component.hpp"
#include "Animator.hpp"
#include "AudioSource.hpp"
#include "CLI.hpp"
#include "Components.Vanilla/Animator.hpp"
#include "Components.Vanilla/CLI.hpp"
#include "Components.Vanilla/UserComponentWrapper.hpp"
#include "ECSImpl.hpp"
#include "Engine.hpp"
#include "Entity.hpp"
#include "ResourceManager.hpp"
#include "TextField.hpp"
#include "ToBytes.hpp"
#include "UIButton.hpp"
#include "UIManager.hpp"
#include "metadata.hpp"
#include "raylib.h"
#include <algorithm>
#include <memory>
#include <string>
#include <vector>

serialize class Player : public AUserComponent {
public:
    GENERATE_METADATA(Player)
    Player() = default;
    ~Player() = default;

    void Start() override;
    void Update(int entityID) override;
    void OnLoad() override;

private:
};
