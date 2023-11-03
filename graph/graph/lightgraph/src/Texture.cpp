/*
** EPITECH PROJECT, 2023
** graph
** File description:
** Texture.cpp
*/

#include <filesystem>
#include "../include/Texture.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"

namespace lg {
    Texture::Texture(
        const std::string &pathToImage,
        GLenum format,
        GLenum storageFormat,
        int mipmapLevel
    )
    {
        std::filesystem::path path(pathToImage);
        if (!std::filesystem::exists(path))
            throw std::runtime_error("Texture file not found: " + pathToImage);
        int width, height, nrChannels;
        unsigned char *data = stbi_load(pathToImage.c_str(), &width, &height, &nrChannels, 0);
        if (!data)
            throw std::runtime_error("Failed to load texture: " + pathToImage);
        glGenTextures(1, &_id);
        glBindTexture(GL_TEXTURE_2D, _id);
        // TODO add flags to handle these parameters and give more control to the user
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, mipmapLevel, format, width, height, 0, storageFormat, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);

        int err = glGetError();
        if (err != GL_NO_ERROR)
            throw std::runtime_error("Failed to create texture: " + std::to_string(err));
    }

    Texture::~Texture()
    {
        glDeleteTextures(1, &_id);
    }
}
