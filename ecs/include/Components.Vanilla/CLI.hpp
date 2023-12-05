/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** CLI.hpp
*/

#pragma once

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

    void RegisterCustomCommand(const std::string& command, std::function<void(CLI&, std::vector<std::string>)> func, const std::string& help = "");
    std::string GetClassName() const;
    Entity GetContext() const;

private:
    void parseCommand(const std::string& command);
    std::string consoleGetLine();

    std::string m_buffer;
    std::future<std::string> m_future;
    bool m_prompted = false;

    std::chrono::milliseconds m_timeOut = std::chrono::milliseconds(100);
    int m_skipFrames = 0;
    std::map<std::string, std::function<void(CLI&, std::vector<std::string>)>> m_commands = {
        { "help", [](CLI& cli, std::vector<std::string> args) { cli.showHelp(); } },
        { "ne", [](CLI& cli, std::vector<std::string> args) { cli.createEntity(); } },
        { "ctxt", [](CLI& cli, std::vector<std::string> args) { cli.setContext(args); } },
        { "mv", [](CLI& cli, std::vector<std::string> args) { cli.moveEntity(args); } },
        { "save", [](CLI& cli, std::vector<std::string> args) { cli.save(args); } },
        { "exit", [](CLI& cli, std::vector<std::string> args) { exit(0); } },
        { "skip", [](CLI& cli, std::vector<std::string> args) { cli.skipNFrames(args); } },
        { "addcpt", [](CLI& cli, std::vector<std::string> args) { cli.addVanillaCptFromIndex(args); } },
        { "add", [](CLI& cli, std::vector<std::string> args) { cli.addVanillaCptFromName(args); } },
        { "clear", [](CLI& cli, std::vector<std::string> args) { system("clear"); } },
        { "reload", [](CLI& cli, std::vector<std::string> args) { cli.reloadEntities(args); } },
        { "le", [](CLI& cli, std::vector<std::string> args) { cli.listEntities(); } },
        { "hotreload", [](CLI& cli, std::vector<std::string> args) { cli.hotReload(); } },
        { "rme", [](CLI& cli, std::vector<std::string> args) { cli.removeEntity(args); } },
        { "setmbr", [](CLI& cli, std::vector<std::string> args) { cli.setMember(args); } },
        { "listmbr", [](CLI& cli, std::vector<std::string> args) { cli.listExposedMembers(args); } },
        { "lsctxt", [](CLI& cli, std::vector<std::string> args) { cli.showContext(); } },
    };

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
    };
};
