/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** CLI.cpp
*/

#include "CLI.hpp"
#include "ECSImpl.hpp"
#include "Engine.hpp"
#include "UIButton.hpp"
#include <filesystem>
#include <sstream>
#include <thread>
#include <variant>

CLI::CLI()
{
}

CLI::CLI(const CLI& cpy)
{
    m_commands = cpy.m_commands;
}

void CLI::OnLoad()
{
    UIButton::RegisterCallback("cli::save", [&] {
        save({});
    });
    UIButton::RegisterCallback("cli::reload", [&] {
        reloadEntities({});
    });
    UIButton::RegisterCallback("cli::hotreload", [&] {
        hotReload({});
    });
}

std::string CLI::consoleGetLine()
{
    std::string line;

    Console::prompt << "CLI _> " << std::flush;
    std::getline(std::cin, line);
    return line;
}

void CLI::Update(int entityID)
{
    std::string command;

    if (m_skipFrames > 1) {
        m_skipFrames--;
        return;
    }
    if (!m_prompted) {
        m_future = std::async(std::launch::async, &CLI::consoleGetLine, this);
        m_prompted = true;
    }
    if (m_future.wait_for(m_timeOut) == std::future_status::ready) {
        command = m_future.get();
        m_prompted = false;
        parseCommand(command);
    }
}

void CLI::parseCommand(const std::string& command)
{
    std::vector<std::string> args;
    std::string keyword;
    std::stringstream ss(command);

    ss >> keyword;
    while (ss) {
        std::string arg;
        ss >> arg;
        args.push_back(arg);
    }
    if (m_commands.find(keyword) == m_commands.end()) {
        system(command.c_str());
        return;
    }
    try {
        m_commands[keyword](*this, args);
    } catch (std::exception& e) {
        Console::err << "Error: " << e.what() << std::endl;
    }
}

void CLI::RegisterCustomCommand(const std::string& command, std::function<void(CLI&, std::vector<std::string>)> func, const std::string& help)
{
    m_commands[command] = func;
    if (help != "")
        m_commandsHelp[command] = help;
}

void CLI::showHelp(__attribute__((unused)) std::vector<std::string> args)
{
    Console::info << "Available commands (" << m_commands.size() << " in total):" << std::endl;
    for (auto& command : m_commands) {
        std::cout << "\t" << ecs::cyan << command.first << ecs::reset;
        if (m_commandsHelp.find(command.first) != m_commandsHelp.end())
            std::cout << " : " << m_commandsHelp[command.first];
        std::cout << std::endl;
    }
}

void CLI::createEntity(__attribute__((unused)) std::vector<std::string> args)
{
    Sys.SetEditorEntityContext(Sys.RegisterEntity());
    Sys.AddComponent<CoreTransform>(Sys.GetEditorEntityContext());
    Console::info << "Created entity: " << Sys.GetEditorEntityContext() << std::endl;
}

void CLI::setContext(std::vector<std::string> args)
{
    if (args.size() - 1 != 1) {
        Console::err << "Invalid number of arguments" << std::endl;
        return;
    }
    int value = std::stoi(args[0]);
    auto entities = Sys.GetEntities();
    if (std::find(entities.begin(), entities.end(), value) == entities.end()) {
        Console::err << "Invalid entity ID" << std::endl;
        return;
    }
    Sys.SetEditorEntityContext(value);
    Console::info << "Set context to entity: " << Sys.GetEditorEntityContext() << std::endl;
}

void CLI::moveEntity(std::vector<std::string> args)
{
    if (args.size() - 1 != 3) {
        Console::err << "Invalid number of arguments" << std::endl;
        return;
    }
    auto& transform = Sys.GetComponent<CoreTransform>(Sys.GetEditorEntityContext());
    transform.x += std::stof(args[0]);
    transform.y += std::stof(args[1]);
    transform.z += std::stof(args[2]);
    Console::info << "Moved entity: " << Sys.GetEditorEntityContext() << std::endl;
}

void CLI::save(std::vector<std::string> args)
{
    if (args.size() - 1 != 1) {
        if (args.size() == 0)
            args.push_back(Sys.GetSavePath());
        else
            args[0] = Sys.GetSavePath();
    } else {
        args[0] = Sys.GetSavePath() + "/" + args[0];
    }

    try {
        for (const auto& entry : std::filesystem::directory_iterator(args[0])) {
            std::filesystem::remove_all(entry.path());
        }
    } catch (std::exception& e) {
    }
    std::filesystem::create_directory(args[0]);
    Sys.SetSavePath(std::string(args[0]));
    for (auto& entity : Sys.GetEntities()) {
        Sys.SaveEntity(entity);
    }
    Console::info << "Saved entities to " << args[0] << std::endl;
}

void CLI::skipNFrames(std::vector<std::string> args)
{
    if (args.size() - 1 != 1) {
        Console::err << "Invalid number of arguments" << std::endl;
        return;
    }
    m_skipFrames = std::stoi(args[0]);
    Console::info << "Skipping " << m_skipFrames << " frames" << std::endl;
}

void CLI::addVanillaCptFromIndex(std::vector<std::string> args)
{
    if (args.size() - 1 != 1) {
        Console::err << "Invalid number of arguments" << std::endl;
        return;
    }
    if (Sys.GetEditorEntityContext() == -1) {
        Console::err << "No entity context set" << std::endl;
        return;
    }
    try {
        Sys.AddVanillaComponentFromIndex(Sys.GetEditorEntityContext(), std::stoi(args[0]));
    } catch (std::exception& e) {
        Console::err << "Invalid Index" << std::endl;
    }
}

void CLI::addVanillaCptFromName(std::vector<std::string> args)
{
    if (args.size() - 1 != 1) {
        Console::err << "Invalid number of arguments" << std::endl;
        return;
    }
    if (Sys.GetEditorEntityContext() == -1) {
        Console::err << "No entity context set" << std::endl;
        return;
    }
    try {
        Sys.AddVanillaComponentFromName(Sys.GetEditorEntityContext(), args[0]);
    } catch (std::exception& e) {
        Console::err << "Invalid Name" << std::endl;
    }
}

void CLI::reloadEntities(std::vector<std::string> args)
{
    if (args.size() - 1 != 1) {
        Console::info << "Defaulting reload path to " << Sys.GetSavePath() << std::endl;
        if (args.size() == 0)
            args.push_back(Sys.GetSavePath());
        else
            args[0] = Sys.GetSavePath();
    } else {
        args[0] = eng::Engine::GetEngine()->GetConfigValue("scenesSavePath") + "/" + args[0];
    }
    std::cout << "Reloading entities from " << args[0] << std::endl;
    try {
        Sys.SetSavePath(std::string(args[0]));
        Sys.ReloadEntities();
    } catch (std::exception& e) {
        Console::err << "Error while reloading entities: " << e.what() << std::endl;
    }
}

void CLI::listEntities(__attribute__((unused)) std::vector<std::string> args)
{
    Console::info << "Entities (" << Sys.GetEntities().size() << " in total)" << std::endl;
    for (auto& entity : Sys.GetEntities()) {
        Console::info << "\t" << entity << " :\n";
        Sys.ForEachComponent(entity, [&](ecs::ECSImpl::AnyCpt& cpt) {
            std::visit([&](auto&& arg) {
                Console::info << "\t\t" << arg.GetClassName() << std::endl;
            },
                cpt);
        });
    }
}

std::string CLI::GetClassName() const
{
    return "CLI";
}

void CLI::hotReload(__attribute__((unused)) std::vector<std::string> args)
{
    Sys.GetResourceManager().CheckHotReload();
}

void CLI::removeEntity(std::vector<std::string> args)
{
    if (args.size() - 1 != 1) {
        Console::err << "Invalid number of arguments" << std::endl;
        return;
    }
    if (std::stoi(args[0]) == Sys.GetSystemHolder()) {
        Console::warn << "Cannot remove system" << std::endl;
        return;
    }
    try {
        Sys.UnregisterEntity(std::stoi(args[0]));
    } catch (std::exception& e) {
        Console::err << "Invalid Index" << std::endl;
    }
}

Entity CLI::GetContext() const
{
    return Sys.GetEditorEntityContext();
}

void CLI::setMember(std::vector<std::string> args)
{
    for (auto& arg : args)
        std::cout << arg << std::endl;
    if (args.size() - 1 < 2) {
        Console::err << "Invalid number of arguments" << std::endl;
        return;
    }
    if (Sys.GetEditorEntityContext() == -1) {
        Console::err << "No entity context set" << std::endl;
        return;
    }

    int cptIdx = std::stoi(args[0]);
    std::string memberName = args[1];
    while (memberName[memberName.size() - 1] <= 32) {
        memberName[memberName.size() - 1] = '\0';
    }
    std::string value = "";
    for (int i = 2; i < args.size(); i++)
        value += args[i] + " ";
    try {
        Sys.SetMember(Sys.GetEditorEntityContext(), cptIdx, memberName, value);
    } catch (std::exception& e) {
        Console::err << "Error: " << e.what() << std::endl;
    }
}

void CLI::listExposedMembers(std::vector<std::string> args)
{
    if (args.size() - 1 != 1) {
        Console::err << "Invalid number of arguments" << std::endl;
        return;
    }
    if (Sys.GetEditorEntityContext() == -1) {
        Console::err << "No entity context set" << std::endl;
        return;
    }

    int cptIdx = std::stoi(args[0]);

    try {
        std::map<std::string, metadata_t> meta = Sys.GetMetadataFromIndex(Sys.GetEditorEntityContext(), cptIdx);
        for (auto& m : meta) {
            Console::info << "\t" << m.first << " : " << m.second.type << " = " << m.second.tostring() << std::endl;
        }
    } catch (std::exception& e) {
        Console::err << "Error: " << e.what() << std::endl;
    }
}

void CLI::showContext(__attribute__((unused)) std::vector<std::string> args)
{
    Entity context = Sys.GetEditorEntityContext();
    if (context == -1) {
        Console::err << "No entity context set" << std::endl;
        return;
    }
    Console::info << "Focused entity: " << context << std::endl;
    Sys.ForEachComponent(context, [&](ecs::ECSImpl::AnyCpt& cpt) {
        std::visit([&](auto&& arg) {
            Console::info << "\t\t" << arg.GetClassName() << std::endl;
        },
            cpt);
    });
}
