
/*
** @file Enemy.hpp
** @brief Definition of the Enemy class.
** @date 2023-12-14 17:30:39.
**
*/

#ifndef E2359F7A_1BE1_4E6A_B3E0_981C71D03CB9
#define E2359F7A_1BE1_4E6A_B3E0_981C71D03CB9

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
 * @brief This class is a script to control the Enemy.
 * It will depend on a controller, to enable controlling the Enemy
 * from the network, the local player, or the ai.
 *
 */
serialize class Enemy : public AUserComponent, public ecs::AActor {
public:
    GENERATE_METADATA(Enemy)
    Enemy() = default;
    ~Enemy() = default;

    static const std::string COMMAND_LEFT;
    static const std::string COMMAND_SHOOT;
    static const std::string COMMAND_UP;
    static const std::string COMMAND_DOWN;

    void Start() override;
    void OnAddComponent(int entityID) override;
    void Update(int entityID) override;

    void SetID(int id);

    void SetBonusOnDeath(int bonus);

private:
    /**
     * @brief Applies the various directives to the Enemy.
     *
     */
    void applyDirectives();

    /**
     * @brief broadcast the new position to all clients
     *
     */
    void broadcastPosition();

    /**
     * @brief broadcast the new velocity to all clients
     *
     */
    void broadcastVelocity();

    /**
     * @brief Checks if the enemy should die, and if so, kills it and
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
    void broadcastDeath();

    serialize float _speed = 1.0f;
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
    bool _first = true;

    int _bonusOnDeath = -1;

    eng::Timer _broadcastTimer;

    std::map<std::string, void (Enemy::*)()> _actions = {
        { COMMAND_LEFT, &Enemy::moveLeft },
        { COMMAND_SHOOT, &Enemy::shoot },
        { COMMAND_UP, &Enemy::moveUp },
        { COMMAND_DOWN, &Enemy::moveDown },

    };
    void shoot();
    void moveLeft();
    void moveUp();
    void moveDown();

    std::function<void(int bonus)> _directivesBonus;
};

#endif /* E2359F7A_1BE1_4E6A_B3E0_981C71D03CB9 */
