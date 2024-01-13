/*
** EPITECH PROJECT, 2023
** safe
** File description:
** Boss.cpp
*/

#include "Boss.hpp"
#include "BossBody.hpp"
#include "BossController.hpp"
#include "BossLaser.hpp"
#include "CoreTransform.hpp"
#include "ECSImpl.hpp"
#include "Engine.hpp"
#include <cmath>
#include <memory>

MANAGED_RESOURCE(Boss)
#define BOSS_SHOOT 85236 // normally in a header file GameRType.hpp but circular dll dependencies on windows so we put it here

// ===========================================================================================================
// Component
// ===========================================================================================================

void Boss::OnAddComponent(int entityID)
{
    _entityID = entityID;
    eng::Engine::GetEngine()->SetGlobal("bossHp", 10);

    try {
        int body1 = SYS.GetResourceManager().LoadPrefab("boss-body");
        auto& body1Component = SYS.GetComponent<BossBody>(body1, "BossBody");
        auto& thisTransform = SYS.GetComponent<CoreTransform>(_entityID);
        auto& bodyTransform = SYS.GetComponent<CoreTransform>(body1);

        body1Component.SetParent(&thisTransform);
        body1Component.SetRank(_bodyParts);
        bodyTransform.x = thisTransform.x;
        bodyTransform.y = thisTransform.y;

        Possess(_entityID, std::make_shared<rtype::BossController>());
        dynamic_cast<rtype::BossController*>(_controller.get())->SetEntityID(_entityID);
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

void Boss::Start()
{
}

void Boss::Update(int entityID)
{
    if (not _controller)
        return;
    _controller->PollDirectives();
    auto directives = _controller->GetDirectives();
    for (auto& directive : directives) {
        try {
            (this->*_directivesCallbacks.at(directive))();
        } catch (std::out_of_range& e) {
            std::cerr << "Directive " << directive << " not found" << std::endl;
        }
    }
}

// ===========================================================================================================
// Actor
// ===========================================================================================================

void Boss::Possess(int entity, std::shared_ptr<ecs::IController> controller)
{
    _entityID = entity;
    _controller = controller;
}

// ===========================================================================================================
// Getters & Setters
// ===========================================================================================================

// ===========================================================================================================
// Public methods
// ===========================================================================================================

// ===========================================================================================================
// Private methods
// ===========================================================================================================

void Boss::moveUp()
{
    try {
        auto& transform = SYS.GetComponent<CoreTransform>(_entityID);
        transform.y -= _speed * SYS.GetDeltaTime();
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

void Boss::moveDown()
{
    try {
        auto& transform = SYS.GetComponent<CoreTransform>(_entityID);
        transform.y += _speed * SYS.GetDeltaTime();
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

void Boss::moveLeft()
{
    try {
        auto& transform = SYS.GetComponent<CoreTransform>(_entityID);
        transform.x -= _speed * SYS.GetDeltaTime();
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

void Boss::moveRight()
{
    try {
        auto& transform = SYS.GetComponent<CoreTransform>(_entityID);
        transform.x += _speed * SYS.GetDeltaTime();
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

void Boss::shoot()
{
    try {
        auto& transform = SYS.GetComponent<CoreTransform>(_entityID);
        graph::vec2i from = { static_cast<int>(transform.x), static_cast<int>(transform.y) };

        if (eng::Engine::GetEngine()->IsClient()) {
            from = eng::Engine::GetEngine()->GetGlobal<graph::vec2i>("bossShoot");
            eng::Engine::GetEngine()->SetGlobal<graph::vec2i>("bossShoot", graph::vec2i { -1, -1 });
        }

        for (int i = 0; i < 10; i++) {
            int laser = SYS.GetResourceManager().LoadPrefab("boss-laser");
            auto& laserComponent = SYS.GetComponent<BossLaser>(laser, "BossLaser");
            auto& laserTransform = SYS.GetComponent<CoreTransform>(laser);

            laserComponent.SetDirection({ cosf(i * 2 * M_PI / 10), sinf(i * 2 * M_PI / 10) });
            laserTransform.x = from.x;
            laserTransform.y = from.y;
        }
        if (eng::Engine::GetEngine()->IsServer()) {
            eng::Engine::GetEngine()->GetServer().Broadcast(serv::Instruction(BOSS_SHOOT, 0, serv::bytes(std::vector<int> { static_cast<int>(from.x), static_cast<int>(from.y) })));
        }
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}
