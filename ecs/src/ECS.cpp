/*
** EPITECH PROJECT, 2022
** ecs
** File description:
** ECS.cpp
*/

#include <libconfig.h++>
#include "ECSImpl.hpp"
#include "Engine.hpp"

namespace ecs {
    template <>
    void ECSImpl::NotifyEnginePipelineErased()
    {
        eng::Engine::GetEngine()->NotifyPipelineChange();
    }

    template<>
    void ECSImpl::RequestEngineClearPipeline()
    {
        eng::Engine::GetEngine()->ClearPipeline();
    }

    template<>
    void ECSImpl::Run(Action preUpdate, Action postUpdate)
    {
        while (_running) {
            preUpdate();
            Update();
            postUpdate();
            _skipFrame = false;
        }
    }

    // template <>
    // UserComponentWrapper &ECSImpl::AddComponent(Entity e, const std::string &cptTypeName)
    // {
    //     try {
    //         UserComponentWrapper &wrapper = AddComponent<UserComponentWrapper>(e);
    //         std::shared_ptr<AUserComponent> internal = _resourceManager.LoadUserComponent(cptTypeName);
    //         internal->OnAddComponent(e);
    //         wrapper.SetInternalComponent(internal);
    //         wrapper.SetResourceID(cptTypeName);
    //         return wrapper;
    //     } catch (std::exception &ex) {
    //         std::cout << red << "Failed to load component: " << ex.what() << std::endl;
    //         std::cout << yellow << "If you think this is because the binary has not been rebuilt, try manually deleting the " << cptTypeName << ".checksum file associated with the component." << std::endl;
    //         throw ex;
    //     }
    // }
}
