/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** Rectangle.hpp
*/

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>
#include "VAO.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

namespace lg {
    class Rectangle {
        public:
            static bool ShaderIsSetup;
            static unsigned int Indices[6];

            Rectangle(vec2f pos, vec2f size, vec4f color, float borderSize, vec4f borderColor);
            void Draw();
            void SetRotation(vec3f rotation);
            void SetColor(vec4f color);

        private:
            float _vertices[12] = {
                // positions
                0.5f, 0.5f, 0.0f,    // top right
                0.5f, -0.5f, 0.0f,   // bottom right
                -0.5f, -0.5f, 0.0f,  // bottom left
                -0.5f, 0.5f, 0.0f,    // top left
            };
            glm::mat4 _rotation = glm::mat4(1.0f);

            std::shared_ptr<VAO> _vao;
            std::shared_ptr<VBO> _vbo;
            std::shared_ptr<EBO> _ebo;

            glm::vec4 _color = glm::vec4(1.0f);

    };
}
