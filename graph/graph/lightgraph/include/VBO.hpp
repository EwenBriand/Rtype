/*
** EPITECH PROJECT, 2023
** graph
** File description:
** VBO.hpp
*/

#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <stdexcept>
#include "glad.h"
#include <iostream>

namespace lg
{
    /**
     * @brief This class encapsulates a Vertex Buffer Object from opengl.
     *
     */
    class VBO {
        public:
            /**
             * @brief Creates the VBO.
             *
             */
            VBO(void *data, size_t size, GLenum usage = GL_STATIC_DRAW)
            {
                if (!data)
                    throw std::runtime_error("VBO: data is null");
                if (usage != GL_STATIC_DRAW && usage != GL_DYNAMIC_DRAW && usage != GL_STREAM_DRAW)
                    throw std::runtime_error("VBO: invalid usage");

                glGenBuffers(1, &_id);
                glBindBuffer(GL_ARRAY_BUFFER, _id);
                _size = size;
                glBufferData(GL_ARRAY_BUFFER, _size, data, usage);
            }

            /**
             * @brief Destroys the VBO.
             *
             */
            ~VBO()
            {
                glDeleteBuffers(1, &_id);
            }

            /**
             * @brief Binds the VBO.
             *
             */
            void Bind()
            {
                glBindBuffer(GL_ARRAY_BUFFER, _id);
            }

            /**
             * @brief Unbinds the VBO.
             *
             */
            void Unbind()
            {
                glBindBuffer(GL_ARRAY_BUFFER, 0);
            }

            /**
             * @brief Returns the id of the VBO.
             *
             * @return unsigned int
             */
            unsigned int GetId() const
            {
                return _id;
            }

            /**
             * @brief Returns the size of the VBO.
             *
             */
            size_t GetSize() const
            {
                return _size;
            }

            /**
             * @brief Sets the data of the VBO. It should
             * have the same size as the previous data.
             *
             */
            void SetData(void *data, size_t size = 0)
            {
                if (!data)
                    throw std::runtime_error("VBO: data is null");
                if (size != 0)
                    _size = size;
                glBindBuffer(GL_ARRAY_BUFFER, _id);
                glBufferSubData(GL_ARRAY_BUFFER, 0, _size, data);
            }

            void SetVertexAttribPointer(GLuint loc, GLint nElem, GLenum elemType, GLboolean normalized, GLint stride=0, void *offset = 0)
            {
                Bind();
                glVertexAttribPointer(loc, nElem, elemType, normalized, stride, offset);
                glEnableVertexAttribArray(loc);
            }
        private:
            unsigned int _id;
            size_t _size;
    };
} // namespace lg
