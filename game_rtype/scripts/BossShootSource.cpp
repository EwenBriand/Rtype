/*
** EPITECH PROJECT, 2023
** safe
** File description:
** BossShootSource.cpp
*/

#include "BossShootSource.hpp"
#include "BossLaser.hpp"

#define BOSS_SHOOT 85236 // normally in a header file GameRType.hpp but circular dll dependencies on windows so we put it here

MANAGED_RESOURCE(BossShootSource)

int BossShootSource::Ids = 0;

BossShootSource::BossShootSource()
{
    Ids++;
    _id = Ids;
    eng::Engine::GetEngine()->SetGlobal<graph::vec2i>("bossShoot" + std::to_string(_id), graph::vec2i { -1, -1 });
}

void BossShootSource::Start()
{
}

void BossShootSource::OnAddComponent(int entityID)
{
    _entityID = entityID;
    _shootTimer.Restart();
}

void BossShootSource::Update(int entityID)
{
    std::string globName = "bossShoot" + std::to_string(_id);
    if (eng::Engine::GetEngine()->IsClient()) {
        auto shootCoord = eng::Engine::GetEngine()->GetGlobal<graph::vec2i>(globName);
        if (shootCoord.x == -1 && shootCoord.y == -1)
            return;
        shoot();
    } else {
        if (_shootTimer.GetElapsedTime() > 4 * _id) {
            _shootTimer.Restart();
            shoot();
        }
    }
}

void BossShootSource::shoot()
{
    std::string globName = "bossShoot" + std::to_string(_id);
    try {
        auto& transform = SYS.GetComponent<CoreTransform>(_entityID);
        graph::vec2i from = { static_cast<int>(transform.x), static_cast<int>(transform.y) };

        if (eng::Engine::GetEngine()->IsClient()) {
            from = eng::Engine::GetEngine()->GetGlobal<graph::vec2i>(globName);
            eng::Engine::GetEngine()->SetGlobal<graph::vec2i>(globName, graph::vec2i { -1, -1 });
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
            eng::Engine::GetEngine()->GetServer().Broadcast(serv::Instruction(BOSS_SHOOT, 0, serv::bytes(std::vector<int> { _id, static_cast<int>(from.x), static_cast<int>(from.y) })));
        }
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}