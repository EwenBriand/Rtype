/*
** EPITECH PROJECT, 2023
** graph
** File description:
** Uniform.hpp
*/

#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <stdexcept>
#include "glad.h"
#include <string>
#include "Window.hpp"
#include "Shader.hpp"

namespace lg
{
    /**
     * @brief This class wraps the uniform variables from opengl.
     * It provides ways to set their values and access them.
     *
     */
    template <typename T>
    class Uniform {
        public:
            static void ListUniforms(ShaderProgram &shaderProgram)
            {
                shaderProgram.Use();
                int count;
                glGetProgramiv(shaderProgram.GetId(), GL_ACTIVE_UNIFORMS, &count);
                std::cout << "There are " << count << " active uniforms in program " << shaderProgram.GetName() << std::endl;
                for (int i = 0; i < count; i++) {
                    int size;
                    GLenum type;
                    char name[256];
                    glGetActiveUniform(shaderProgram.GetId(), i, 256, nullptr, &size, &type, name);
                    std::cout << "Uniform #" << i << " is " << name << " of type " << type << " and size " << size << std::endl;
                }
            }

            Uniform(const std::string &name, const T &value, ShaderProgram &shaderProgram) :
                _name(name), _shaderProgram(shaderProgram)
            {
                _shaderProgram.Use();
                _location = glGetUniformLocation(shaderProgram.GetId(), name.c_str());
                if (_location == -1)
                    throw LightGraphException("Uniform variable " + name + " not found in shader program " + shaderProgram.GetName());
                *this << value;
            }

            /**
             * @brief Sets the value of the uniform variable.
             * It also uses the shader program beforehand.
             *
             * @param value
             */
            void operator <<(const T &value)
            {
                _value = value;
                _shaderProgram.Use();
                setValue();
            }

            /**
             * @brief Returns the value of the uniform variable.
             *
             * @return T
             */
            T GetValue() const
            {
                return _value;
            }

            /**
             * @brief Returns the name of the uniform variable.
             *
             * @return std::string
             */
            std::string GetName() const
            {
                return _name;
            }

            /**
             * @brief Returns the location of the uniform variable.
             *
             * @return int
             */
            int GetLocation() const
            {
                return _location;
            }

            /**
             * @brief Returns the shader program of the uniform variable.
             *
             * @return ShaderProgram&
             */
            ShaderProgram &GetShaderProgram() const
            {
                return _shaderProgram;
            }

            /**
             * @brief Sets the value of the uniform variable without
             * using the shader program nor setting the value in the opengl context.
             *
             */
            void HoldValue(const T &value)
            {
                _value = value;
            }

        private:
            /**
             * @brief Set the Value object in the opengl context.
             * Does not use the shader program and lets the user do it manually.
             *
             */
            void setValue();
            std::string _name;
            T _value;

            int _location;
            ShaderProgram &_shaderProgram;
    };
} // namespace lg
