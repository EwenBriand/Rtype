/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** CLI.hpp
*/

#pragma once

#include "../../linenoise/linenoise.h"
#include "../Component.hpp"
#include <chrono>
#include <functional>
#include <future>
#include <map>
#include <string>
#include <vector>

class CLI : public ecs::Component<CLI> {
public:
    using Entity = unsigned int;
    CLI();
    CLI(const CLI& cpy);
    void Update(int entityID);

    /**
     * @brief Registers the functions to be callable from buttons instead.
     *
     */
    void OnLoad();

    /**
     * @brief Prints the help message on the console.
     *
     * @param args
     */
    void showHelp(std::vector<std::string> args = std::vector<std::string>());

    /**
     * @brief Creates a new entity and displays its ID on the console.
     *
     * @param args
     */
    void createEntity(std::vector<std::string> args = std::vector<std::string>());

    /**
     * @brief Sets the context to the given entity id.
     *
     * @param args : the entity id.
     */
    void setContext(std::vector<std::string> args);

    /**
     * @brief Moves the entity to the given position.
     *
     * @param args : id dx dy
     */
    void moveEntity(std::vector<std::string> args);

    /**
     * @brief Saves the current state of the ECS.
     *
     * @param args
     */
    void save(std::vector<std::string> args);

    /**
     * @brief Skips n frames without prompting the user.
     *
     * @param args : n
     */
    void skipNFrames(std::vector<std::string> args);

    /**
     * @brief Adds a vanilla component to the entity.
     *
     * @param args : index
     */
    void addVanillaCptFromIndex(std::vector<std::string> args);

    /**
     * @brief Adds a vanilla component to the entity from its name.
     *
     * @param args : name
     */
    void addVanillaCptFromName(std::vector<std::string> args);

    /**
     * @brief Reloads the entities from the save file.
     *
     * @param args
     */
    void reloadEntities(std::vector<std::string> args);

    /**
     * @brief Lists all the entities and their components.
     *
     * @param args
     */
    void listEntities(std::vector<std::string> args = std::vector<std::string>());

    /**
     * @brief Reloads the user scripts and compiles them if needed.
     *
     * @param args
     */
    void hotReload(std::vector<std::string> args = std::vector<std::string>());

    /**
     * @brief Removes the entity from the ECS.
     *
     * @param args
     */
    void removeEntity(std::vector<std::string> args);

    /**
     * @brief Sets the member of the component to the given value.
     *
     * @param args : componentIdx memberName value
     */
    void setMember(std::vector<std::string> args);

    /**
     * @brief Lists the exposed members of the component at the given index in the entitiy's components.
     *
     * @param args : componentIdx
     */
    void listExposedMembers(std::vector<std::string> args);

    /**
     * @brief Show information on the entity set as the context.
     *
     * @param command
     * @param func
     */
    void showContext(std::vector<std::string> args = std::vector<std::string>());

    /**
     * @brief Saves the current entity to a prefab file.
     *
     * @param command
     * @param func
     * @param help
     */
    void savePrefab(std::vector<std::string> args = std::vector<std::string>());

    /**
     * @brief Loads a prefab from a file.
     *
     * @param command
     * @param func
     * @param help
     */
    void loadPrefab(std::vector<std::string> args = std::vector<std::string>());

    /**
     * @brief Lists all the prefabs.
     *
     * @param command
     * @param func
     * @param help
     */
    void listPrefabs(std::vector<std::string> args = std::vector<std::string>());

    /**
     * @brief Removes a prefab.
     *
     * @param command
     * @param func
     * @param help
     */
    void removePrefab(std::vector<std::string> args = std::vector<std::string>());

    void RegisterCustomCommand(const std::string& command, std::function<void(CLI&, std::vector<std::string>)> func, const std::string& help = "");
    std::string GetClassName() const;
    Entity GetContext() const;

    static const std::map<std::string, std::function<void(CLI&, std::vector<std::string>)>> getM_commands();
    static void setM_commands(const std::map<std::string, std::function<void(CLI&, std::vector<std::string>)>>& m_commands);

    // void SetCompletion(const char* buf, linenoiseCompletions* lc);

private:
    void parseCommand(const std::string& command);
    std::string consoleGetLine();

    std::string m_buffer;
    std::future<std::string> m_future;
    std::vector<std::string> m_history;
    std::vector<std::string>::iterator m_historyIt;
    bool m_prompted = false;

    std::chrono::milliseconds m_timeOut = std::chrono::milliseconds(100);
    int m_skipFrames = 0;

    static std::map<std::string, std::function<void(CLI&, std::vector<std::string>)>> m_commands;

    std::map<std::string, std::string> m_commandsHelp = {
        { "help", "Shows this help message." },
        { "ne", "Creates a new entity." },
        { "ctxt", "Sets the context to the given entity id." },
        { "mv", "Moves the entity to the given position." },
        { "save", "Saves the current state of the ECS." },
        { "exit", "Exits the program." },
        { "skip", "Skips n frames without prompting the user." },
        { "addcpt", "Adds a vanilla component to the entity." },
        { "add", "Adds a vanilla component to the entity from its name." },
        { "clear", "Clears the console." },
        { "reload", "Reloads the entities from the save file." },
        { "le", "Lists all the entities and their components." },
        { "hotreload", "Reloads the user scripts and compiles them if needed." },
        { "rme", "Removes the entity from the ECS." },
        { "setmbr", "Sets the member of the component to the given value." },
        { "listmbr", "Lists the exposed members of the component at the given index in the entitiy's components." },
        { "lsctxt", "Show information on the entity set as the context." },
        { "exit", "Quit the cli" },
        { "prefab.save", "Save the current entity as a prefab" },
        { "prefab.load", "Load a prefab" },
        { "prefab.list", "List all the prefabs" },
        { "prefab.remove", "Remove a prefab" }
    };
};
