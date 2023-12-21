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

std::map<std::string, std::function<void(CLI&, std::vector<std::string>)>> CLI::m_commands = {
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
    { "prefab.save", [](CLI& cli, std::vector<std::string> args) { cli.savePrefab(args); } },
    { "prefab.load", [](CLI& cli, std::vector<std::string> args) { cli.loadPrefab(args); } },
    { "prefab.list", [](CLI& cli, std::vector<std::string> args) { cli.listPrefabs(args); } },
    { "prefab.remove", [](CLI& cli, std::vector<std::string> args) { cli.removePrefab(args); } },
    { "ssend", [](CLI& cli, std::vector<std::string> args) { cli.sendMessageToServer(args); } },
    { "csend", [](CLI& cli, std::vector<std::string> args) { cli.sendMessageToClients(args); } },
};

const std::map<std::string, std::function<void(CLI&, std::vector<std::string>)>> CLI::getM_commands()
{
    return m_commands;
}

void CLI::setM_commands(const std::map<std::string, std::function<void(CLI&, std::vector<std::string>)>>& m_commands)
{
    CLI::m_commands = m_commands;
}

CLI::CLI(const CLI& cpy)
{
    m_commands = cpy.m_commands;
}

void feedVector(std::vector<std::string>& vector, std::vector<std::string>& files)
{
    for (const auto& kv : CLI::getM_commands()) {
        vector.push_back(kv.first);
    }

    std::string path = eng::Engine::GetEngine()->GetConfigValue("assetRoot");
    if (!std::filesystem::exists(path)) {
        throw std::runtime_error("Path does not exist");
    }

    for (const auto& entry : std::filesystem::recursive_directory_iterator(path)) {
        if (std::filesystem::is_regular_file(entry)) {
            std::filesystem::path relative_path = std::filesystem::relative(entry.path(), path);
            files.push_back(relative_path.string());
        }
    }
}

void SetCompletion(const char* buf, linenoiseCompletions* lc)
{
    static std::vector<std::string> commands;
    static std::vector<std::string> files;
    std::string bufStr(buf);
    std::string fileName;
    std::string::size_type pos = bufStr.rfind(' ');

    if (commands.size() == 0 || files.size() == 0)
        feedVector(commands, files);

    if (pos != std::string::npos && pos != bufStr.length() - 1) {
        fileName = bufStr.substr(pos + 1);
        bufStr = bufStr.substr(0, pos + 1);
    } else {
        fileName = bufStr;
        bufStr = "";
    }

    for (const auto& command : commands)
        if (command.compare(0, fileName.length(), fileName) == 0)
            linenoiseAddCompletion(lc, (bufStr + command.c_str()).c_str());

    for (const auto& file : files)
        if (file.compare(0, fileName.length(), fileName) == 0)
            linenoiseAddCompletion(lc, (bufStr + file.c_str()).c_str());
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

    linenoiseSetCompletionCallback(SetCompletion);
}

std::string CLI::consoleGetLine()
{
    char* line = linenoise("CLI _> ");
    if (line == nullptr) {
        return "";
    }
    std::string result(line);
    free(line);
    return result;
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
        linenoiseHistoryAdd(command.c_str());
        m_history.push_back(command);
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
        CONSOLE::err << "Error: " << e.what() << std::endl;
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
    CONSOLE::info << "Available commands (" << m_commands.size() << " in total):" << std::endl;
    for (auto& command : m_commands) {
        std::cout << "\t" << ecs::cyan << command.first << ecs::reset;
        if (m_commandsHelp.find(command.first) != m_commandsHelp.end())
            std::cout << " : " << m_commandsHelp[command.first];
        std::cout << std::endl;
    }
}

void CLI::createEntity(__attribute__((unused)) std::vector<std::string> args)
{
    SYS.SetEditorEntityContext(SYS.RegisterEntity());
    SYS.AddComponent<CoreTransform>(SYS.GetEditorEntityContext());
    CONSOLE::info << "Created entity: " << SYS.GetEditorEntityContext() << std::endl;
}

void CLI::setContext(std::vector<std::string> args)
{
    if (args.size() - 1 != 1) {
        CONSOLE::err << "Invalid number of arguments" << std::endl;
        return;
    }
    int value = std::stoi(args[0]);
    auto entities = SYS.GetEntities();
    if (std::find(entities.begin(), entities.end(), value) == entities.end()) {
        CONSOLE::err << "Invalid entity ID" << std::endl;
        return;
    }
    SYS.SetEditorEntityContext(value);
    CONSOLE::info << "Set context to entity: " << SYS.GetEditorEntityContext() << std::endl;
}

void CLI::moveEntity(std::vector<std::string> args)
{
    if (args.size() - 1 != 3) {
        CONSOLE::err << "Invalid number of arguments" << std::endl;
        return;
    }
    auto& transform = SYS.GetComponent<CoreTransform>(SYS.GetEditorEntityContext());
    transform.x += std::stof(args[0]);
    transform.y += std::stof(args[1]);
    transform.z += std::stof(args[2]);
    CONSOLE::info << "Moved entity: " << SYS.GetEditorEntityContext() << std::endl;
}

void CLI::save(std::vector<std::string> args)
{
    if (args.size() - 1 != 1) {
        if (args.size() == 0)
            args.push_back(SYS.GetSavePath());
        else
            args[0] = SYS.GetSavePath();
    } else {
        args[0] = SYS.GetSavePath() + "/" + args[0];
    }

    try {
        for (const auto& entry : std::filesystem::directory_iterator(args[0])) {
            std::filesystem::remove_all(entry.path());
        }
    } catch (std::exception& e) {
    }
    std::filesystem::create_directory(args[0]);
    SYS.SetSavePath(std::string(args[0]));
    for (auto& entity : SYS.GetEntities()) {
        if (entity == SYS.GetSystemHolder())
            continue;
        SYS.SaveEntity(entity);
    }
    CONSOLE::info << "Saved entities to " << args[0] << std::endl;
}

void CLI::skipNFrames(std::vector<std::string> args)
{
    if (args.size() - 1 != 1) {
        CONSOLE::err << "Invalid number of arguments" << std::endl;
        return;
    }
    m_skipFrames = std::stoi(args[0]);
    CONSOLE::info << "Skipping " << m_skipFrames << " frames" << std::endl;
}

void CLI::addVanillaCptFromIndex(std::vector<std::string> args)
{
    if (args.size() - 1 != 1) {
        CONSOLE::err << "Invalid number of arguments" << std::endl;
        return;
    }
    if (SYS.GetEditorEntityContext() == -1) {
        CONSOLE::err << "No entity context set" << std::endl;
        return;
    }
    try {
        SYS.AddVanillaComponentFromIndex(SYS.GetEditorEntityContext(), std::stoi(args[0]));
    } catch (std::exception& e) {
        CONSOLE::err << "Invalid Index" << std::endl;
    }
}

void CLI::addVanillaCptFromName(std::vector<std::string> args)
{
    if (args.size() - 1 != 1) {
        CONSOLE::err << "Invalid number of arguments" << std::endl;
        return;
    }
    if (SYS.GetEditorEntityContext() == -1) {
        CONSOLE::err << "No entity context set" << std::endl;
        return;
    }
    try {
        SYS.AddVanillaComponentFromName(SYS.GetEditorEntityContext(), args[0]);
    } catch (std::exception& e) {
        CONSOLE::err << "Invalid Name" << std::endl;
    }
}

void CLI::reloadEntities(std::vector<std::string> args)
{
    if (args.size() - 1 != 1) {
        CONSOLE::info << "Defaulting reload path to " << SYS.GetSavePath() << std::endl;
        if (args.size() == 0)
            args.push_back(SYS.GetSavePath());
        else
            args[0] = SYS.GetSavePath();
    } else {
        args[0] = eng::Engine::GetEngine()->GetConfigValue("scenesSavePath") + "/" + args[0];
    }
    std::cout << "Reloading entities from " << args[0] << std::endl;
    try {
        SYS.SetSavePath(std::string(args[0]));
        SYS.ReloadEntities();
    } catch (std::exception& e) {
        CONSOLE::err << "Error while reloading entities: " << e.what() << std::endl;
    }
}

void CLI::listEntities(__attribute__((unused)) std::vector<std::string> args)
{
    CONSOLE::info << "Entities (" << SYS.GetEntities().size() << " in total)" << std::endl;
    for (auto& entity : SYS.GetEntities()) {
        CONSOLE::info << "\t" << entity << " :\n";
        SYS.ForEachComponent(entity, [&](ecs::ECSImpl::AnyCpt& cpt) {
            std::visit([&](auto&& arg) {
                CONSOLE::info << "\t\t" << arg.GetClassName() << std::endl;
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
    SYS.GetResourceManager().CheckHotReload();
}

void CLI::removeEntity(std::vector<std::string> args)
{
    if (args.size() - 1 != 1) {
        CONSOLE::err << "Invalid number of arguments" << std::endl;
        return;
    }
    if (std::stoi(args[0]) == SYS.GetSystemHolder()) {
        CONSOLE::warn << "Cannot remove system" << std::endl;
        return;
    }
    try {
        SYS.UnregisterEntity(std::stoi(args[0]));
    } catch (std::exception& e) {
        CONSOLE::err << "Invalid Index" << std::endl;
    }
}

Entity CLI::GetContext() const
{
    return SYS.GetEditorEntityContext();
}

void CLI::setMember(std::vector<std::string> args)
{
    for (auto& arg : args)
        std::cout << arg << std::endl;
    if (args.size() - 1 < 2) {
        CONSOLE::err << "Invalid number of arguments" << std::endl;
        return;
    }
    if (SYS.GetEditorEntityContext() == -1) {
        CONSOLE::err << "No entity context set" << std::endl;
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
        SYS.SetMember(SYS.GetEditorEntityContext(), cptIdx, memberName, value);
    } catch (std::exception& e) {
        CONSOLE::err << "Error: " << e.what() << std::endl;
    }
}

void CLI::listExposedMembers(std::vector<std::string> args)
{
    if (args.size() - 1 != 1) {
        CONSOLE::err << "Invalid number of arguments" << std::endl;
        return;
    }
    if (SYS.GetEditorEntityContext() == -1) {
        CONSOLE::err << "No entity context set" << std::endl;
        return;
    }

    int cptIdx = std::stoi(args[0]);

    try {
        std::map<std::string, metadata_t> meta = SYS.GetMetadataFromIndex(SYS.GetEditorEntityContext(), cptIdx);
        for (auto& m : meta) {
            CONSOLE::info << "\t" << m.first << " : " << m.second.type << " = " << m.second.tostring() << std::endl;
        }
    } catch (std::exception& e) {
        CONSOLE::err << "Error: " << e.what() << std::endl;
    }
}

void CLI::showContext(__attribute__((unused)) std::vector<std::string> args)
{
    Entity context = SYS.GetEditorEntityContext();
    if (context == -1) {
        CONSOLE::err << "No entity context set" << std::endl;
        return;
    }
    CONSOLE::info << "Focused entity: " << context << std::endl;
    SYS.ForEachComponent(context, [&](ecs::ECSImpl::AnyCpt& cpt) {
        std::visit([&](auto&& arg) {
            CONSOLE::info << "\t\t" << arg.GetClassName() << std::endl;
        },
            cpt);
    });
}

void CLI::savePrefab(std::vector<std::string> args)
{
    if (args.size() - 1 != 1) {
        CONSOLE::err << "Invalid number of arguments" << std::endl;
        return;
    }
    if (SYS.GetEditorEntityContext() == SYS.GetSystemHolder()) {
        CONSOLE::err << "Cannot save system" << std::endl;
        return;
    }

    SYS.GetResourceManager().SavePrefab(args[0], SYS.GetEditorEntityContext());
}

void CLI::loadPrefab(std::vector<std::string> args)
{
    if (args.size() - 1 != 1) {
        CONSOLE::err << "Invalid number of arguments" << std::endl;
        return;
    }

    int entity = SYS.GetResourceManager().LoadPrefab(args[0]);
    if (entity == -1) {
        CONSOLE::err << "Failed to load prefab" << std::endl;
        return;
    }
    SYS.SetEditorEntityContext(entity);
}

void CLI::listPrefabs(std::vector<std::string> args)
{
    std::string assetRoot = eng::Engine::GetEngine()->GetConfigValue("assetRoot");
    std::string prefabRoot = ecs::ResourceManager::MakePath({ assetRoot, "prefabs" });
    std::vector<std::string> prefabs;

    CONSOLE::info << "Prefabs root: " << prefabRoot << std::endl;
    for (const auto& entry : std::filesystem::directory_iterator(prefabRoot)) {
        if (std::filesystem::is_directory(entry)) {
            std::filesystem::path relative_path = std::filesystem::relative(entry.path(), prefabRoot);
            prefabs.push_back(relative_path.string());
        }
    }
    CONSOLE::info << "Prefabs (" << prefabs.size() << " in total)" << std::endl;
    for (auto& prefab : prefabs) {
        CONSOLE::info << "\t" << prefab << std::endl;
    }
}

void CLI::removePrefab(std::vector<std::string> args)
{
    if (args.size() - 1 != 1) {
        CONSOLE::err << "Invalid number of arguments" << std::endl;
        return;
    }

    std::string assetRoot = eng::Engine::GetEngine()->GetConfigValue("assetRoot");
    std::string prefabRoot = ecs::ResourceManager::MakePath({ assetRoot, "prefabs" });
    std::string prefabPath = ecs::ResourceManager::MakePath({ prefabRoot, args[0] });

    if (!std::filesystem::exists(prefabPath)) {
        CONSOLE::err << "Prefab does not exist" << std::endl;
        return;
    }
    std::filesystem::remove(prefabPath);
}

void CLI::sendMessageToServer(std::vector<std::string> args)
{
    if (args.size() - 1 < 1) {
        CONSOLE::err << "Invalid number of arguments" << std::endl;
        return;
    }
    auto engine = eng::Engine::GetEngine();
    if (engine->IsOptionSet(eng::Engine::Options::SERVER_MODE)) {
        CONSOLE::err << "Cannot send message to server in server mode" << std::endl;
        return;
    }
    try {
        auto& clientEndpoint = engine->GetClient();
        std::string message = "";
        for (auto& arg : args)
            message += arg + " ";
        clientEndpoint.Send(serv::bytes(message) + serv::SEPARATOR);
    } catch (std::exception& e) {
        CONSOLE::err << "Not connected to server" << std::endl;
        return;
    }
}

void CLI::sendMessageToClients(std::vector<std::string> args)
{
    if (args.size() - 1 < 1) {
        CONSOLE::err << "Invalid number of arguments" << std::endl;
        return;
    }
    auto engine = eng::Engine::GetEngine();
    if (!engine->IsOptionSet(eng::Engine::Options::SERVER_MODE)) {
        CONSOLE::err << "Cannot send message to clients in client mode" << std::endl;
        return;
    }
    try {
        auto& serverEndpoint = engine->GetServer();
        std::string message = "";
        for (auto& arg : args)
            message += arg + " ";
        serverEndpoint.Broadcast(serv::bytes(message) + serv::SEPARATOR);
    } catch (std::exception& e) {
        CONSOLE::err << "Not connected to server" << std::endl;
        return;
    }
}
