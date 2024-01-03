/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** ECSImpl.hpp
*/

#ifndef AB1794DB_72E6_4ECF_848A_91CD7A70EDE6
#define AB1794DB_72E6_4ECF_848A_91CD7A70EDE6
#include "ECS.hpp"

#include "Components.Vanilla/Animator.hpp"
#include "Components.Vanilla/AudioSource.hpp"
#include "Components.Vanilla/CLI.hpp"
#include "Components.Vanilla/Collider2D.hpp"
#include "Components.Vanilla/CoreTransform.hpp"
#include "Components.Vanilla/EditorMouseManager.hpp"
#include "Components.Vanilla/EntityExplorer.hpp"
#include "Components.Vanilla/EventGraph.hpp"
#include "Components.Vanilla/LuaScript.hpp"
#include "Components.Vanilla/NodePrintStr.hpp"
#include "Components.Vanilla/NodeStr.hpp"
#include "Components.Vanilla/RigidBody2D.hpp"
#include "Components.Vanilla/SplashIcon.hpp"
#include "Components.Vanilla/Tag.hpp"
#include "Components.Vanilla/TextField.hpp"
#include "Components.Vanilla/UIButton.hpp"
#include "Components.Vanilla/UIDiv.hpp"
#include "Components.Vanilla/UIMenu.hpp"
#include "Components.Vanilla/UserComponentWrapper.hpp"

namespace ecs {
    /**
     * @brief This defines an instantiation of the ECS class for a specific
     * application.
     *
     */
    using ECSImpl = ECS<
        CoreTransform,
        CLI,
        UserComponentWrapper,
        UIDiv,
        UIButton,
        UIMenu,
        TextField,
        EditorMouseManager,
        EntityExplorer,
        Animator,
        Collider2D,
        AudioSource,
        RigidBody2D,
        Tag,
        SplashIcon,
        LuaScript>;

}

#define SYS ecs::ECSImpl::GetSystem()
#define CONSOLE ecs::ECSImpl

#endif /* AB1794DB_72E6_4ECF_848A_91CD7A70EDE6 */
