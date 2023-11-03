/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** UserComponentWrapper.cpp
*/

#include "ECSImpl.hpp"
#include "UserComponentWrapper.hpp"

void UserComponentWrapper::Update(int entityID)
{
    if (m_internalComponent)
        m_internalComponent->Update(entityID);
}

void UserComponentWrapper::OnLoad()
{
    try {
        Entity e = Sys.GetSystemHolder();
        CLI &cli = Sys.GetComponent<CLI>(e);
        cli.RegisterCustomCommand("usercpt-load", [&]( CLI &c, std::vector<std::string> args) {
            if (args.size() < 2)
                return;
            Entity currE = cli.GetContext();
            if (currE == Sys.GetSystemHolder())
                throw std::runtime_error("Cannot add user component to system holder");
            Sys.AddComponent<UserComponentWrapper>(currE);
            try {
                UserComponentWrapper &wrapper = Sys.GetComponent<UserComponentWrapper>(currE);
                std::shared_ptr<AUserComponent> internal = Sys.GetResourceManager().LoadUserComponent(std::string(args[0]));
                internal->OnAddComponent(currE);
                wrapper.SetInternalComponent(internal);
                wrapper.SetResourceID(args[0]);
            } catch (std::exception &e) {
                Console::err << "Failed to load component: " << e.what() << std::endl;
                Console::warn << "If you think this is because the binary has not been rebuilt, try manually deleting the " << args[0] << ".checksum file associated with the component." << std::endl;
            }
        });

    } catch (std::exception &e) {
        Console::err << "Failed to load user component wrapper: " << e.what() << std::endl;
    }
}

void UserComponentWrapper::Start()
{
    if (m_internalComponent)
        m_internalComponent->Start();
}

void UserComponentWrapper::Load(const std::string &path)
{
    libconfig::Config config;
    config.readFile(path.c_str());
    libconfig::Setting &setting = config.getRoot();
    setting.lookupValue("resourceID", resourceID);
    if (resourceID == "")
        return;
    m_internalComponent = Sys.GetResourceManager().LoadUserComponent(resourceID);
    if (m_internalComponent) {
        m_internalComponent->Load(path + "_internal");
        m_internalComponent->OnLoad();
    }
}

void UserComponentWrapper::Save(const std::string &path)
{
    if (resourceID == "")
        return;
    if (m_internalComponent)
        m_internalComponent->Save(path + "_internal");
    libconfig::Config config;
    libconfig::Setting &setting = config.getRoot();
    setting.add("resourceID", libconfig::Setting::TypeString) = resourceID;
    config.writeFile(path.c_str());
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

void UserComponentWrapper::SetResourceID(const std::string &resourceID)
{
    this->resourceID = resourceID;
}

std::string UserComponentWrapper::GetResourceID() const
{
    return resourceID;
}
