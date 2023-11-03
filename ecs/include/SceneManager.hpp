/*
** EPITECH PROJECT, 2023
** repo
** File description:
** SceneManager.hpp
*/

#pragma once
#include <map>
#include <memory>
#include <string>
#include <future>
#include <atomic>
#include <thread>
#include "ECSImpl.hpp"

namespace ecs
{
    /**
     * @brief A class to contain a scene while it is being loaded.
     *
     */
    class SceneBuffer {
        public:
            /**
             * @brief Construct a new Scene Buffer object and loads the scene
             * in the background. When the scene is ready, it sets the _isReady
             * flag to true.
             *
             * @param sceneName
             */
            SceneBuffer(const std::string &sceneName);

            /**
             * @brief Returns true if the scene is ready to be used, false
             * otherwise.
             *
             * @return true
             * @return false
             */
            bool IsReady() const;

            /**
             * @brief Returns the components of the scene.
             *
             * @return ECSImpl::AllCpt
             */
            ECSImpl::AllCpt &GetComponents();

        private:
            /**
             * @brief Loads the scene.
             *
             * @param sceneName
             */
            void load(const std::string &sceneName);
            std::future<void> _future;
            std::atomic_bool _isReady;
            ECSImpl::AllCpt _components;
    };

    /**
     * @brief Singleton for the scene manager, which can be used to load
     * scenes in the background and switch between them.
     *
     */
    class SceneManager {
        public:
            static std::unique_ptr<SceneManager> _instance;
            /**
             * @brief Returns a reference to the SceneManager singleton.
             *
             * @return SceneManager&
             */
            static SceneManager &Get();

            /**
             * @brief Inits the editor mode of the scenemanager to allow for
             * manual commands
             *
            */
            void InitEditorMode();

            /**
             * @brief Loads a scene in the background.
             *
             * @param sceneName
             */
            void LoadSceneAsync(const std::string &sceneName);

            /**
             * @brief switches the current scene to the one specified
             *
             */
            void SwitchScene(const std::string &sceneName);

            /**
             * @brief Unloads a scene
             *
             */
            void UnloadScene(const std::string &sceneName);

            /**
             * @brief returns true if the scene in argument is ready to be used,
             * false else
             *
             */
            bool IsSceneReady(const std::string &sceneName);

        private:
            std::map<std::string, SceneBuffer> _scenes;
    };
} // namespace ecs
