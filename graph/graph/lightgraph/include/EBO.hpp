/*
** EPITECH PROJECT, 2023
** graph
** File description:
** EBO.hpp
*/

#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <stdexcept>
#include "glad.h"

namespace lg {
    /**
     * @brief This class encapsulates an Element Buffer Object from opengl.
     * Use jointly with VBO.
     */
    class EBO {
        public:
            EBO(void *indices, size_t size, GLenum usage = GL_STATIC_DRAW)
            {
                if (!indices)
                    throw std::runtime_error("EBO: indices is null");
                if (usage != GL_STATIC_DRAW && usage != GL_DYNAMIC_DRAW && usage != GL_STREAM_DRAW)
                    throw std::runtime_error("EBO: invalid usage");

                glGenBuffers(1, &_id);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
                _size = size;
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, _size, indices, usage);
            }

            ~EBO()
            {
                glDeleteBuffers(1, &_id);
            }

            void Bind()
            {
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
            }

            void Unbind()
            {
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            }

            void Draw(GLenum mode = GL_TRIANGLES, int nElem = -1)
            {
                if (nElem == -1)
                    nElem = _size / sizeof(GL_UNSIGNED_INT);
                glDrawElements(mode, nElem, GL_UNSIGNED_INT, 0);
            }

            size_t GetSize() const
            {
                return _size;
            }
        private:
            GLuint _id;
            size_t _size;
    };
}
