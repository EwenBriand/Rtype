/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** ScopeLocation.hpp
*/

#pragma once
#include <string>
#include <filesystem>

/**
 * @brief A macro to change the current working directory to the one specified in the argument.
 * Use as follows:
 * @example withLocation("path/to/new/working/directory") {
 *     // code
 * }
 *
 * The location will be reset to the previous one when the scope is exited.
 */
#define withLocation(x) if (ecs::ScopeLocation _ = ecs::ScopeLocation(x))


namespace ecs
{
    /**
     * @brief Moves the execution directory of the program to the directory
     * given in the constructor.
     * Returns to the previous directory when the object is destroyed.
     *
     */
    class ScopeLocation
    {
        public:
            /**
             * @brief Moves the execution directory of the program to the
             * directory given in the constructor.
             *
             * @param path
             */
            ScopeLocation(const std::string &path);

            ScopeLocation(const std::filesystem::path &path);


            /**
             * @brief Returns to the previous directory.
             *
             */
            ~ScopeLocation();

            operator bool() const {
                return true;
            }
        private:
            std::string m_previousLocation;
    };
} // namespace ecs
