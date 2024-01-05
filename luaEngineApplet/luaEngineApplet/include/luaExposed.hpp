
/**
    * @file luaExposed.hpp
    * @brief File that contains all the exposed classes and functions to Lua
    * @date 2020
    * DISCLAIMER: This file is generated automatically by the parseExposed.py script.
    *             Do not modify it directly, or your changes will be lost.
*/
#pragma once
#include <LuaBridge/LuaBridge.h>


#include "ECS.hpp"

#include "Entity.hpp"

#include "GenericStorageSystem.hpp"

#include "Hoverable.hpp"

#include "IActor.hpp"

#include "IController.hpp"

#include "IGame.hpp"

#include "IGraphicalModule.hpp"

#include "InputManager.hpp"

#include "LibUtils.hpp"

#include "NamedScopeLock.hpp"

#include "NoGraphics.hpp"

#include "Observer.hpp"

#include "ResourceManager.hpp"

#include "SceneManager.hpp"

#include "ScopeLocation.hpp"

#include "States.hpp"

#include "ThreadPool.hpp"

#include "Timer.hpp"

#include "Types.hpp"

#include "UIManager.hpp"

#include "Component.hpp"

#include "ECSImpl.hpp"

#include "Engine.hpp"

#include "Animation.hpp"

#include "Animator.hpp"

#include "AudioSource.hpp"

#include "CLI.hpp"

#include "Collider2D.hpp"

#include "EditorMouseManager.hpp"

#include "EntityExplorer.hpp"

#include "EventGraph.hpp"

#include "INode.hpp"

#include "NodePrintStr.hpp"

#include "NodeStr.hpp"

#include "RigidBody2D.hpp"

#include "SplashIcon.hpp"

#include "Tag.hpp"

#include "TextField.hpp"

#include "UIButton.hpp"

#include "UIDiv.hpp"

#include "UIMenu.hpp"

#include "UIScrollableList.hpp"

#include "UserComponentWrapper.hpp"

#include "CoreTransform.hpp"

#include "LuaScript.hpp"

#include "GraphicalRayLib.hpp"

#include "Bytes.hpp"

#include "CircularBuffer.hpp"

#include "ClientUDP.hpp"

#include "Coroutine.hpp"

#include "Message.hpp"

#include "NetworkExceptions.hpp"

#include "ServerUdp.hpp"

#include "ThreadSafeQueue.hpp"

static void lua_init_exposed(lua_State *L) {

    luabridge::getGlobalNamespace(L)
        .beginNamespace("eng")
            .beginClass<eng::Engine>("Engine")
                .addConstructor<void (*) (void)>()
                .addFunction("IsOptionSet", &eng::Engine::IsOptionSet)
                .addFunction("GetOptionValue", &eng::Engine::GetOptionValue)
                .addFunction("GetConfigValue", &eng::Engine::GetConfigValue)
            .endClass();

    luabridge::getGlobalNamespace(L)
            .beginClass<CoreTransform>("CoreTransform")
                .addConstructor<void (*) (void)>()
                .addFunction("SetX", &CoreTransform::SetX)
                .addFunction("SetY", &CoreTransform::SetY)
                .addFunction("SetZ", &CoreTransform::SetZ)
                .addFunction("SetRotationX", &CoreTransform::SetRotationX)
                .addFunction("SetRotationY", &CoreTransform::SetRotationY)
                .addFunction("SetRotationZ", &CoreTransform::SetRotationZ)
                .addFunction("SetScaleX", &CoreTransform::SetScaleX)
                .addFunction("SetScaleY", &CoreTransform::SetScaleY)
                .addFunction("SetScaleZ", &CoreTransform::SetScaleZ)
                .addFunction("GetX", &CoreTransform::GetX)
                .addFunction("GetY", &CoreTransform::GetY)
                .addFunction("GetZ", &CoreTransform::GetZ)
                .addFunction("GetRotationX", &CoreTransform::GetRotationX)
                .addFunction("GetRotationY", &CoreTransform::GetRotationY)
                .addFunction("GetRotationZ", &CoreTransform::GetRotationZ)
                .addFunction("GetScaleX", &CoreTransform::GetScaleX)
                .addFunction("GetScaleY", &CoreTransform::GetScaleY)
                .addFunction("GetScaleZ", &CoreTransform::GetScaleZ)
            .endClass();

}
