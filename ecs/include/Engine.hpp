/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** Engine.hpp
*/

#pragma once
#include "ECSImpl.hpp"
#include "IGame.hpp"
#include "IGraphicalModule.hpp"
#include "SceneManager.hpp"
#include "Types.hpp"
#include <exception>
#include <fstream>
#include <memory>
#include <vector>

#define pushPipeline(action, priority) \
    AddToPipeline(action, priority, __FILE__ + std::string(" ") + __FUNCTION__ + std::string(" ") + std::to_string(__LINE__) + std::string(" (") + std::to_string(priority) + std::string(") ") + #action)

namespace eng {

    /**
     * @brief The engine class provides the setup and environnement for both the
     * game and the editor to run. It is the glue that holds all the parts of the
     * code together.
     *
     * It also holds the frame pipeline, which is a list of actions that will be
     * executed every frame. The actions are sorted by priority, and the lower
     * the priority, the sooner the action will be executed. Priority can be negative.
     * By default, polling events is set to 0, and updating the ECS is set to 500.
     *
     */
    class Engine {
    public:
        struct Options {
            static const std::string EDITOR;
            static const std::string GAME;
            static const std::string CONFIG_DIR;
            static const std::string VERBOSE;
        };

        Engine();
        ~Engine();

        /**
         * @brief Get method to implement the singleton pattern.
         *
         * @param msg
         */
        static Engine* GetEngine();

        static void Log(const std::string& msg);

        /**
         * @brief Starts the engine, sets it up and runs the game loop.
         *
         */
        void Run();

        /**
         * @brief Stops the engine and frees all the resources.
         *
         */
        void Stop();

        /**
         * @brief Parses the options from the command line.
         *
         */
        void ParseOptions(int argc, char** argv);

        /**
         * @brief Returns true if the option has been set in the command line.
         *
         */
        bool IsOptionSet(const std::string& option);

        /**
         * @brief Returns the value of the option if it exists, else throws
         * an exception.
         *
         */
        std::string GetOptionValue(const std::string& option);

        /**
         * @brief Returns the value of the config field if it exists, else throws
         * an exception.
         *
         */
        std::string GetConfigValue(const std::string& option);

        /**
         * @brief Clears the default pipeline.
         *
         */
        void ClearPipeline();

        /**
         * @brief Adds an action to the pipeline. Negative priority means
         * the action will be executed before the Update() call.
         *
         */
        void AddToPipeline(Action action, int priority, const std::string& name = "");

        /**
         * @brief Notifies the engine that the pipeline may have changed
         * manually and needs to be updated.
         *
         */
        void NotifyPipelineChange();

        /**
         * @brief Returns a shared pointer to the graphical module.
         *
         */
        std::shared_ptr<graph::IGraphicalModule> GetGraphicalModule() const;

        /**
         * @brief Returns a reference to the scene manager.
         *
         */
        SceneManager& GetSceneManager();

        /**
         * @brief Returns a reference to the ecs.
         *
         */
        ecs::ECSImpl& GetECS();

        static std::ofstream m_logFile;

        /**
         * @brief Sets the engine up in the case of being ran as the editor
         *
         */
        void SetupEditor();

        /**
         * @brief Retuns a pointer to the game.
         *
         */
        std::shared_ptr<IGame> GetGame() const;

    private:
        /**
         * @brief Discovers the configuration in the .engine folder and
         * sets up the engine accordingly.
         *
         */
        void discoverConfig(const std::string& configPath);

        /**
         * @brief Loads a single configuration file.
         *
         */
        void loadConfig(const std::string& path);

        /**
         * @brief Sorts the pipeline by priority. Lower priority means the
         * action will be executed first. Negative priority means the action
         * will be executed before the Update() call.
         *
         */
        void sortPipeline();
        void setupPipeline();

        /**
         * @brief Updates all the path red from the config file to be absolute.
         * Relative path are relative to the config file.
         *
         */
        void configUpdateRelativePath();

        std::map<std::string, std::string> m_config;
        std::map<std::string, std::string> m_options = {
            { eng::Engine::Options::CONFIG_DIR, "../.engine/config" }
        };

        std::shared_ptr<SceneManager> m_sceneManager;
        std::shared_ptr<graph::IGraphicalModule> m_graphicalModule;

        using Pipeline = std::vector<Action>;
        std::shared_ptr<Pipeline> m_preUpdatePipeline;
        std::shared_ptr<Pipeline> m_postUpdatePipeline;
        bool m_pipelineChanged = false;
        std::shared_ptr<std::vector<std::tuple<int, std::tuple<std::string, Action>>>> m_unsortedPipeline;
        std::shared_ptr<IGame> m_game = nullptr;
        ecs::ECSImpl& m_ecs;
    };

    class EngineException : public std::exception {
    public:
        /**
         * @brief Construct a new Engine Exception object while keeping track
         * of the line at which the exception was thrown.
         *
         * @param msg
         * @param file
         * @param function
         * @param line
         */
        EngineException(const std::string& msg, const std::string& file, const std::string& function, int line)
            : m_msg(msg)
            , m_file(file)
            , m_function(function)
            , m_line(line)
        {
            m_what = std::string("[Engine Exception] ") + "(" + m_file + ":" + std::to_string(m_line) + " in " + m_function + ") " + m_msg;
            Engine::Log(m_what);
        }
        ~EngineException() = default;

        const char* what() const noexcept override
        {
            return (m_what).c_str();
        }

    private:
        std::string m_msg;
        std::string m_file;
        std::string m_function;
        int m_line;
        std::string m_what;
    };

    class ConfigException : public std::exception {
    public:
        /**
         * @brief Signals the fact that the config file is missing or
         * has an invalid format.
         *
         * @param msg
         */
        ConfigException(const std::string& msg)
            : m_msg(msg)
        {
            m_what = "[Config exception] " + m_msg;
            Engine::Log(m_what);
        }
        ~ConfigException() = default;

        const char* what() const noexcept override
        {
            return (m_what).c_str();
        }

    private:
        std::string m_msg;
        std::string m_what;
    };

    class CompatibilityException : public std::exception {
    public:
        /**
         * @brief Signals the fact that the current plugin is not compatible
         * with one of the settings of the engine.
         *
         * @param msg
         */
        CompatibilityException(const std::string& msg)
            : m_msg(msg)
        {
            m_what = "[Compatibility exception] " + m_msg;
            Engine::Log(m_what);
        }
        ~CompatibilityException() = default;

        const char* what() const noexcept override
        {
            return (m_what).c_str();
        }

    private:
        std::string m_msg;
        std::string m_what;
    };
}
