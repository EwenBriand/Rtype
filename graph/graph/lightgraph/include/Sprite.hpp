/*
** EPITECH PROJECT, 2023
** graph
** File description:
** Sprite.hpp
*/

#pragma once

#include <string>
#include <memory>
#include <array>
#include "Window.hpp"
#include "Texture.hpp"
#include "Uniform.hpp"

namespace lg {


    static const std::string spriteShaderVertexSrc = R"(
        #version 420 core

        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec2 aTexCoord;

        out vec2 TexCoord;

        void main()
        {
            gl_Position = vec4(aPos, 1.0);
            TexCoord = aTexCoord;
        }
    )";

    static const std::string spriteShaderFragmentSrc = R"(
        #version 420 core

        in vec2 TexCoord;

        out vec4 FragColor;

        uniform sampler2D texture1;

        void main()
        {
            FragColor = texture(texture1, TexCoord);
        }
    )";
    /**
     * @brief This class lets the user draw a sprite on the screen.
     *
     */
    class Sprite {
        public:
            // static std::shared_ptr<ShaderProgram> SpriteShaderProgram;
            static bool ShaderIsSetup;
            static std::shared_ptr<Uniform<glm::mat4>> SpriteTransformUniform;


            /**
             * @brief Represents the rectangle of the texture that will be drawn.
             * all values should go from 0 to 1. I fthe values exceed this range,
             * the texture will be repeated. The represent the margin to the
             * left, right, top and bottom of the texture.
             *
             */
            struct Rect {
                float top;
                float bottom;
                float right;
                float left;
            };

            Sprite(const std::string &pathToImage, Rect rect = {1.0f, 0.0f, 1.0f, 0.0f});
            ~Sprite();

            /**
             * @brief Draws the sprite on the screen.
             *
             * @param window
             */
            void Draw();

            /**
             * @brief Sets the position of the sprite.
             *
             * @param position
             */
            void SetPosition(const vec3f &position);

            /**
             * @brief Sets the scale of the sprite.
             *
             * @param scale
             */
            void SetScale(const vec3f &scale);

            /**
             * @brief Sets the rotation of the sprite.
             *
             * @param rotation
             */
            void SetRotation(const vec3f &rotation);

            /**
             * @brief Sets the rect of the sprite.
             *
             */
            void SetRect(const Rect &rect);

            /**
             * @brief Returns the position of the sprite.
             *
             * @return vec3f
             */
            vec3f GetPosition() const;

            /**
             * @brief Returns the scale of the sprite.
             *
             * @return vec3f
             */
            vec3f GetScale() const;

            /**
             * @brief Returns the rotation of the sprite.
             *
             * @return vec3f
             */
            vec3f GetRotation() const;

        private:
            Rect _rect;
            vec3f _position = {0, 0, 0};
            vec3f _scale = {1, 1, 1};
            vec3f _rotation = {0, 0, 0};


            float _vertices[12] = {
                // positions
                0.5f, 0.5f, 0.0f,    // top right
                0.5f, -0.5f, 0.0f,   // bottom right
                -0.5f, -0.5f, 0.0f,  // bottom left
                -0.5f, 0.5f, 0.0f,    // top left
            };

            float _texCoords[12] = {
                // texture coords
                1.0f, 1.0f, // top right
                1.0f, 0.0f, // bottom right
                0.0f, 0.0f, // bottom left
                0.0f, 1.0f  // top left
            };

            unsigned int _triangleIndices[12] = {
                0, 1, 3,
                1, 2, 3,
            };

            glm::mat4 _transformMatrix = glm::mat4(1.0f);

            std::shared_ptr<VAO> _vao;
            std::shared_ptr<VBO> _vboPositions;
            std::shared_ptr<VBO> _vboTexCoords;
            std::shared_ptr<EBO> _ebo;
            std::shared_ptr<Texture> _texture;
    };
}
