
/*
** @file Block.hpp
** @brief Definition of the Block class.
** @date 2023-12-14 17:30:39.
**
*/


#pragma once

// #ifndef E2359F7A_1BE1_4E6A_B3E0_981C71D03CB9123
// #define E2359F7A_1BE1_4E6A_B3E0_981C71D03CB9123
#include "Components.Vanilla/Collider2D.hpp"
#include "Components.Vanilla/RigidBody2D.hpp"
#include "Components.Vanilla/TextField.hpp"
#include "Components.Vanilla/UIDiv.hpp"
#include "Components.Vanilla/UserComponentWrapper.hpp"
#include "IActor.hpp"
#include "IController.hpp"
#include "Observer.hpp"
#include "metadata.hpp"

BUILD_MANUAL    

/**
 * @brief This class is a script to control the Block.
 * It will depend on a controller, to enable controlling the Block
 * from the network, the local player, or the ai.
 *
 */
serialize class Block : public AUserComponent, public ecs::AActor {
public:
    GENERATE_METADATA(Block)
    Block() = default;
    ~Block() = default;

    static const std::string COMMAND_LEFT;
    static const std::string COMMAND_SHOOT;
    static const std::string COMMAND_UP;
    static const std::string COMMAND_DOWN;

    void Start() override;
    void OnAddComponent(int entityID) override;
    void Update(int entityID) override;

    void SetID(int id);

private:
    /**
     * @brief Applies the various directives to the Block.
     *
     */
    void applyDirectives();

    /**
     * @brief Checks if the Block should die, and if so, kills it and
     * notifies the game and clients.
     *
     */
    void checkDeath();
    void instantiateRedLaser();
    void broadcastShoot();
    /**
     * @brief Sends the death message to all clients
     *
     */
    // void broadcastDeath();

    serialize float _speed = 100.0f;
    serialize int _damage = 1;
    serialize int _health = 10;
    serialize int _delay = 2;
    eng::Timer _timer;

    RigidBody2D* _rb = nullptr;
    Collider2D* _collider = nullptr;
    CoreTransform* _core = nullptr;
    TextField* _textField = nullptr;
    int _entity;
    eng::Observer _observer;
    int _id = -1;

    // std::map<std::string, void (Block::*)()> _actions = {
    //     { COMMAND_LEFT, &Block::moveLeft },
    //     { COMMAND_SHOOT, &Block::shoot },
    //     { COMMAND_UP, &Block::moveUp },
    //     { COMMAND_DOWN, &Block::moveDown },

    // };
};

// #endif /* E2359F7A_1BE1_4E6A_B3E0_981C71D03CB9123 */

