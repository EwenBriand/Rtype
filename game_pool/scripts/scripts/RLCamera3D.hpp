/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** RLCamera3D.hpp .
*/

#pragma once

#include "Component.hpp"
#include "Components.Vanilla/UserComponentWrapper.hpp"
#include "Engine.hpp"
#include "IGraphicalModule.hpp"
#include "metadata.hpp"
#include <iostream>
#include <raylib.h>
#include <string>

BUILD_MANUAL

serialize class RLCamera3D : public AUserComponent {
public:
    GENERATE_METADATA(RLCamera3D)
    ~RLCamera3D();

    void Update(int entityID) override;
    void OnAddComponent(int entityID) override;
    void Start() override;
    void OnLoad() override;

    void SetTarget(int entity);
    void SetTargetOffset(float x, float y, float z);

private:
    /**
     * @brief updates the camera's position to follow the target's position and rotation.
     * If the target is not set, the camera will not move.
     *If the camera is not in perspective mode, the rotation will not be updated.
     *
     */
    void updatePosition();

    /**
     * @brief px, py, pz, the 3D position of the camera.
     *
     */
    serialize float m_px
        = 5.0f;
    serialize float m_py = 5.0f;
    serialize float m_pz = 5.0f;

    /**
     * @brief tx, ty, tz, the 3D position of the target, which is the point
     * in space the camera is looking at.
     *
     */
    serialize float m_tx = 0.0f;
    serialize float m_ty = 0.0f;
    serialize float m_tz = 0.0f;

    /**
     * @brief ux, uy, uz, the 3D vector representing the up direction of the
     * camera.
     *
     */
    serialize float m_ux = 0.0f;
    serialize float m_uy = 0.1f;
    serialize float m_uz = 0.0f;

    /**
     * @brief fov, the field of view in Y (in degrees), i.e. the extent of
     * the screen that is seen in the camera.
     *
     */
    serialize float m_fov = 45.0f;

    /**
     * @brief projection, the camera type, either perspective or orthographic.
     *
     */
    serialize std::string m_projection = "ORBITAL";

    int m_target = -1;
    Vector3 m_targetOffset = { 0, 0, 0 };

    bool m_mode3DEnabled = false;
    Camera m_camera = { { 5.0f, 5.0f, 5.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, 45.0f, 0 };
};
