/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** WowGraphics.hpp
*/

#pragma once

#include <functional>
#include <string>
#include <map>
#include <raylib.h>
#include <memory>
#include <vector>
#include "VAO.hpp"
#include "VBO.hpp"
#include "EBO.hpp"
#include "Components.User/RLCamera3D.hpp"

#include "Sprite.hpp"

namespace wow {
    class WowGraphicsEngine {
        public:
            /**
             * @brief Enables the WowGraphicsEngine pipeline.
             *
             */
            static void Enable();
            static std::shared_ptr<WowGraphicsEngine> Instance;
            WowGraphicsEngine();
            ~WowGraphicsEngine();

            std::shared_ptr<WowGraphicsEngine> Get();

            /**
             * @brief Starts the mode for rendering
             * 3D objects using voxel graphics and raymarching.
             *
             */
            void InitWowGraphicsEngine();

            void StartRenderMode();
            void EndRenderMode();

        private:
            Shader _raymarchingShader;
            UserComponentWrapper &_camera;

            float _screenQuadVertices[16] = {
                -1.0f, -1.0f, 0.0f, 0.0f,
                1.0f, -1.0f, 1.0f, 0.0f,
                1.0f,  1.0f, 1.0f, 1.0f,
                -1.0f,  1.0f, 0.0f, 1.0f
            };

            unsigned int _screenQuadIndices[6] = {
                0, 1, 2,
                2, 3, 0
            };
            std::shared_ptr<lg::VAO> _vao;
            std::shared_ptr<lg::VBO> _screenQuadVBO;
            std::shared_ptr<lg::EBO> _screenQuadEBO;

            std::shared_ptr<lg::Sprite> _sprite;
    };
}
