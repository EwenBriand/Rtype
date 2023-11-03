/*
** EPITECH PROJECT, 2023
** graph
** File description:
** Texture.hpp
*/

#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <string>
#include <iostream>
#include "glad.h"

namespace lg {

    /**
     * @brief A vertice is composed of xyz space coordinates,
     * rgb color values and uv texture coordinates.
     * All these values are between 0 and 1.
     *
     */
    struct vertice {
        float x;
        float y;
        float z;
        float r;
        float g;
        float b;
        float u;
        float v;
    };

    /**
     * @brief This class encapsulates a Texture from opengl.
     *
     */
    class Texture {
        public:
            /**
             * @brief Construct a new Texture object.
             *
             * @param pathToImage
             * @param format rgb, rgba, etc... the format in the image file
             * @param storageFormat rgb, rgba, etc... the wanted format in the gpu
             * @param mipmapLevel the level at which the mipmap is generated
             */
            Texture(
                const std::string &pathToImage,
                GLenum format = GL_RGB,
                GLenum storageFormat = GL_RGB,
                int mipmapLevel = 0
            );
            ~Texture();

            /**
             * @brief Binds the texture. Use before VAO::Bind().
             *
             */
            inline void Bind() const { glBindTexture(GL_TEXTURE_2D, _id); }
            inline void Unbind() const { glBindTexture(GL_TEXTURE_2D, 0); }

        private:
            GLuint _id;

    };
}
