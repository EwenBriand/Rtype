/*
** EPITECH PROJECT, 2023
** graph
** File description:
** VAO.hpp
*/

#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <stdexcept>
#include "glad.h"
#include <iostream>
#include <vector>
#include "VBO.hpp"
#include "EBO.hpp"
#include "Shader.hpp"

namespace lg {
    /**
     * @brief This class encapsulates a Vertex Array Object from opengl.
     * Use jointly with VBO.
     */
    class VAO {
        public:
            VAO() {
                glGenVertexArrays(1, &_id);
                glBindVertexArray(_id);
            }

            ~VAO() {
                glDeleteVertexArrays(1, &_id);
            }

            void Bind() {
                glBindVertexArray(_id);
            }

            void Unbind() {
                glBindVertexArray(0);
            }

        private:
            GLuint _id;
    };

    /**
     * @brief Regroups a VAO, a VBO and an optional EBO together.
     *
     */
    class VOCouple {
        public:
            VOCouple(void *data, size_t size, GLenum usage = GL_STATIC_DRAW, void *indices = nullptr, size_t indicesSize = 0, GLenum indicesUsage = GL_STATIC_DRAW) :
                _vao(), _vbo(data, size, usage)
            {
                if (indices) {
                    _ebo = std::make_shared<EBO>(indices, indicesSize, indicesUsage);
                }
                Unbind();
            }

            ~VOCouple()
            {
            }

            void Bind()
            {
                _vao.Bind();
                _vbo.Bind();
                if (_ebo)
                    _ebo->Bind();
            }

            void VAOBind() {
                _vao.Bind();
            }

            void Unbind()
            {
                _vao.Unbind();
                _vbo.Unbind();
                if (_ebo)
                    _ebo->Unbind();
            }

            void Draw(GLenum mode = GL_TRIANGLES, int nElem = -1)
            {
                _shaderProgram->Use();
                _shaderProgram->AssignVBO(_vbo);
                _vao.Bind();
                if (nElem == -1 && _ebo)
                    nElem = _ebo->GetSize() / sizeof(GL_UNSIGNED_INT);
                if (_ebo)
                    _ebo->Draw();
                else
                    glDrawArrays(mode, 0, nElem);
            }

            VAO &getVAO()
            {
                return _vao;
            }

            VBO &getVBO()
            {
                return _vbo;
            }

            std::shared_ptr<EBO> getEBO()
            {
                return _ebo;
            }

            void UseShaderProgram(ShaderProgram &shaderProgram)
            {
                _shaderProgram = &shaderProgram;
            }
        private:
            VAO _vao;
            VBO _vbo;
            ShaderProgram *_shaderProgram = nullptr;
            std::shared_ptr<EBO> _ebo = nullptr;
    };
}
