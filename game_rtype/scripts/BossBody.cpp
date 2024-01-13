/*
** EPITECH PROJECT, 2023
** safe
** File description:
** BossBody.cpp
*/

#include "BossBody.hpp"
#include "CoreTransform.hpp"
#include "Engine.hpp"

MANAGED_RESOURCE(BossBody)

void BossBody::Start()
{
    _hpBossMax = eng::Engine::GetEngine()->GetGlobal<int>("bossHp");
}

void BossBody::OnAddComponent(int entityID)
{
    _entityID = entityID;
}

void BossBody::Update(int entityID)
{
    followParent();
    int hpBoss = eng::Engine::GetEngine()->GetGlobal<int>("bossHp");
    if (hpBoss < _hpBossMax - _rank * 10) {
        SYS.UnregisterEntity(_entityID);
    }
}

// ===========================================================================================================
// GETTERS & SETTERS
// ===========================================================================================================

void BossBody::SetRank(int id)
{
    _rank = id;
    if (_rank <= 1)
        return;
    if (_entityID == -1)
        throw std::runtime_error("BossBody::SetRank: entityID not set");
    try {
        int nextBodyE = SYS.GetResourceManager().LoadPrefab("boss-body");
        auto& nextBody = SYS.GetComponent<BossBody>(nextBodyE, "BossBody");
        auto& thisTransform = SYS.GetComponent<CoreTransform>(_entityID);
        auto& nextTransform = SYS.GetComponent<CoreTransform>(nextBodyE);

        nextBody.SetRank(id - 1);
        nextBody.SetParent(&SYS.GetComponent<CoreTransform>(_entityID));
        nextTransform.x = thisTransform.x;
        nextTransform.y = thisTransform.y;
        _previousParentPos = { thisTransform.x, thisTransform.y };
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    // instantiate body parts here
}

void BossBody::SetParent(CoreTransform* parent)
{
    _parent = parent;
}

void BossBody::followParent()
{
    if (_parent == nullptr)
        return;
    try {
        auto& thisTransform = SYS.GetComponent<CoreTransform>(_entityID);

        while (graph::vec2f { thisTransform.x, thisTransform.y }.distance({ _parent->x, _parent->y }) > 70) {
            thisTransform.x += (_parent->x - thisTransform.x) / 100;
            thisTransform.y += (_parent->y - thisTransform.y) / 100;
        }
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}
