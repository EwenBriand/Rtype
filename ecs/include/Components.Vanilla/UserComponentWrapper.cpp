/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** UserComponentWrapper.cpp
*/

#include "UserComponentWrapper.hpp"
#include "ECSImpl.hpp"

void UserComponentWrapper::Update(int entityID)
{
    if (m_internalComponent)
        m_internalComponent->Update(entityID);
}

void UserComponentWrapper::OnLoad()
{
    try {
        Entity e = SYS.GetSystemHolder();
        CLI& cli = SYS.GetComponent<CLI>(e);
        cli.RegisterCustomCommand(
            "usercpt-load", [&](CLI& c, std::vector<std::string> args) {
                if (args.size() < 2)
                    return;
                Entity currE = cli.GetContext();
                if (currE == SYS.GetSystemHolder())
                    throw std::runtime_error("Cannot add user component to system holder");
                SYS.AddComponent<UserComponentWrapper>(currE);
                try {
                    UserComponentWrapper& wrapper = SYS.GetComponent<UserComponentWrapper>(currE);
                    std::shared_ptr<AUserComponent> internal = SYS.GetResourceManager().LoadUserComponent(std::string(args[0]));
                    internal->OnAddComponent(currE);
                    wrapper.SetInternalComponent(internal);
                    std::cout << "setting resource Id to " << args[0] << std::endl; // TODO: remove this line
                    wrapper.SetResourceID(args[0]);
                } catch (std::exception& e) {
                    CONSOLE::err << "Failed to load component: " << e.what() << std::endl;
                    CONSOLE::warn << "If you think this is because the binary has not been rebuilt, try manually deleting the " << args[0] << ".checksum file associated with the component." << std::endl;
                }
            },
            "Load a user component into the current entity");

    } catch (std::exception& e) {
        CONSOLE::err << "Failed to load user component wrapper: " << e.what() << std::endl;
    }
}

void UserComponentWrapper::Start()
{
    if (m_internalComponent)
        m_internalComponent->Start();
}

void UserComponentWrapper::Load(const std::string& path)
{
    // libconfig::Config config;
    // config.readFile(path.c_str());
    // libconfig::Setting &setting = config.getRoot();
    // setting.lookupValue("resourceID", resourceID);
    // if (resourceID == "")
    //     return;
    // m_internalComponent = SYS.GetResourceManager().LoadUserComponent(resourceID);
    // if (m_internalComponent) {
    //     m_internalComponent->Load(path + "_internal");
    //     m_internalComponent->OnLoad();
    // }
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + path);
    }

    std::getline(file, resourceID);
    file.close();
    if (resourceID == "")
        return;
    m_internalComponent = SYS.GetResourceManager().LoadUserComponent(resourceID);
    if (m_internalComponent) {
        m_internalComponent->Load(path + "_internal");
        m_internalComponent->OnLoad();
    }
}

void UserComponentWrapper::Save(const std::string& path)
{
    if (resourceID == "")
        return;
    if (m_internalComponent)
        m_internalComponent->Save(path + "_internal");
    std::ofstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + path);
    }
    file << resourceID;
    file.close();
}

void UserComponentWrapper::OnAddComponent(int entityID)
{
    if (m_internalComponent)
        m_internalComponent->OnAddComponent(entityID);
}

std::map<std::string, metadata_t> UserComponentWrapper::GetMetadata()
{
    if (m_internalComponent)
        return m_internalComponent->GetMetadata();
    return std::map<std::string, metadata_t>();
}

std::string UserComponentWrapper::GetClassName() const
{
    if (m_internalComponent != nullptr)
        return m_internalComponent->GetClassName();
    return "UserComponentWrapper";
}

void UserComponentWrapper::SetInternalComponent(std::shared_ptr<AUserComponent> component)
{
    m_internalComponent = component;
}

std::shared_ptr<AUserComponent> UserComponentWrapper::GetInternalComponent() const
{
    return m_internalComponent;
}

void UserComponentWrapper::SetResourceID(const std::string& resourceID)
{
    this->resourceID = resourceID;
}

std::string UserComponentWrapper::GetResourceID() const
{
    return resourceID;
}
