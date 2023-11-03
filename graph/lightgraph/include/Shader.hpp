/*
** EPITECH PROJECT, 2023
** graph
** File description:
** Shader.hpp
*/

#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>
#include <memory>
#include "VBO.hpp"
#include "glad.h"

namespace lg {
    /**
     * @brief This class holds the source of a shader.
     * It is meant to be compiled in a ShaderProgram.
     *
     */
    class Shader {
        public:
            Shader(const std::string &pathToSrc, int priority = 0, GLenum type = GL_VERTEX_SHADER, bool fromFile = true);
            ~Shader();

            inline unsigned int getId() const { return _id; }
        private:
            std::string loadFile(const std::string &pathToSrc);
            void compile();


            int _priority;
            GLuint _id;
            GLenum _type;
            std::string _source;
    };

    class ShaderProgram {
        public:
            ShaderProgram(const std::string &name);
            ~ShaderProgram();

            /**
             * @brief Adds a shader to the program. The lower the priority is, the earlier the shader will be compiled.
             *
             * @param pathToSrc
             * @param priority
             * @param type
             */
            void AddShader(const std::string &pathToSrc, int priority, GLenum type, bool fromFile = true);

            /**
             * @brief Adds a shader to the program from a shared pointer to a shader.
             * The lower the priority is, the earlier the shader will be compiled.
            */
            void AddShader(std::shared_ptr<Shader> shader, int priority);

            /**
             * @brief Links the program.
             *
             */
            void Link(bool showLog = false);

            /**
             * @brief Uses the program, meaning that it will be used for the next draw calls.
             *
             */
            void Use();

            /**
             * @brief Returns the ID of the program
             *
            */
            inline unsigned int GetId() const { return _id; }

            /**
             * @brief Returns the name of the program
             *
            */
            inline std::string GetName() const { return _name; }

            /**
             * @brief Assigns a VBO to the program and sets the vertex attribute pointer.
             *
             * @param vbo
             * @param startIdx
             * @param size
             * @param stride
             * @param type
             * @param normalized
             * @param offset
             */
            void AssignVBO(
                VBO &vbo,
                int startIdx = 0,
                int size = 3,
                int stride = 0,
                GLenum type = GL_FLOAT,
                bool normalized = GL_FALSE,
                void *offset = 0
            );

            /**
             * @brief Returns true if the shader program is currently the
             * one being used by opengl
             *
             */
            bool IsUsed() const;

        private:
            std::string _name;
            GLuint _id;
            std::vector<std::tuple<int, std::shared_ptr<Shader>>> _shaders;
    };
}
