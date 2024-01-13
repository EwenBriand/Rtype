/*
** EPITECH PROJECT, 2023
** safe
** File description:
** BossBody.hpp
*/

#pragma once

#include "Components.Vanilla/UserComponentWrapper.hpp"
#include "Engine.hpp"
#include "IActor.hpp"
#include "IController.hpp"
#include "metadata.hpp"
#include <map>

BUILD_MANUAL

serialize class BossBody : public AUserComponent {
public:
    GENERATE_METADATA(BossBody)

    BossBody() = default;
    ~BossBody() = default;

    // ==============================================================
    // COMPONENT
    // ==============================================================
    void Start() override;
    void OnAddComponent(int entityID) override;
    void Update(int entityID) override;

    // ==============================================================
    // GETTERS & SETTERS
    // ==============================================================
    /**
     * @brief Set the Rank object, which represents how many body parts
     * are left after this one. Upon setting the rank, the next body part
     * will be set to the rank - 1 and instantiated.
     *
     * @param id
     */
    void SetRank(int id);

    /**
     * @brief Set the Parent transform, which the body part
     * will follow around.
     *
     * @param id
     */
    void SetParent(CoreTransform* parent);

    // ==============================================================
    // PUBLIC METHODS
    // ==============================================================

private:
    // ==============================================================
    // PRIVATE METHODS
    // ==============================================================

    /**
     * @brief Follows the parent transform by moving its own transform
     *
     */
    void followParent();

    // ==============================================================
    // ATTRIBUTES
    // ==============================================================
    int _rank = 0;
    int _entityID = -1;
    int _hpBossMax = 0;
    CoreTransform* _parent = nullptr;
    graph::vec2f _previousParentPos = { 0, 0 };
};
