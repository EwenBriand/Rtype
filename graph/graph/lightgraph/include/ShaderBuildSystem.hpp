/*
** EPITECH PROJECT, 2023
** graph
** File description:
** ShaderBuildSystem.hpp
*/

#pragma once

#include <string>
#include <memory>
#include <vector>
#include <map>
#include "Shader.hpp"
#include "ShaderBuilder.hpp"

namespace lg {
    /**
     * @brief This class lets different parts of a code build shaders
     * progressively without needing to be aware of the whole shader.
     *
     */
    class ShaderBuildSystem {
        public:
            static std::shared_ptr<ShaderBuildSystem> Singleton;
            ShaderBuildSystem();

            /**
             * @brief Returns the singleton instance of the ShaderBuildSystem.
             *
             * @return std::shared_ptr<ShaderBuildSystem>
             */
            static ShaderBuildSystem &Get();

            /**
             * @brief Returns a reference to the shader builder with the given name.
             * If the builder does not exist, it is created.
             *
             */
            ShaderBuilder &operator[](const std::string &name);

            /**
             * @brief Builds all existing shaders.
             *
             */
            void BuildAll();
        private:
            std::map<std::string, ShaderBuilder> _builders;
    };
}
