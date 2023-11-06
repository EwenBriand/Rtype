/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** ECSImpl.hpp
*/

#ifndef AB1794DB_72E6_4ECF_848A_91CD7A70EDE6
#define AB1794DB_72E6_4ECF_848A_91CD7A70EDE6
#include "ECS.hpp"

#include "Components.Vanilla/CLI.hpp"
#include "Components.Vanilla/CoreTransform.hpp"
#include "Components.Vanilla/EventGraph.hpp"
#include "Components.Vanilla/UserComponentWrapper.hpp"
#include "Components.Vanilla/NodePrintStr.hpp"
#include "Components.Vanilla/NodeStr.hpp"
#include "Components.Vanilla/UIDiv.hpp"
#include "Components.Vanilla/UIMenu.hpp"
#include "Components.Vanilla/TextField.hpp"
#include "Components.Vanilla/UIButton.hpp"
#include "Components.Vanilla/EditorMouseManager.hpp"
#include "Components.Vanilla/EntityExplorer.hpp"
#include "Components.Vanilla/Animator.hpp"

namespace ecs {
    using ECSImpl = ECS<
        CoreTransform,
        CLI,
        EventGraph,
        UserComponentWrapper,
        NodePrintStr,
        NodeStr,
        UIDiv,
        UIButton,
        UIMenu,
        TextField,
        EditorMouseManager,
        EntityExplorer,
        Animator
    >;

    #define Sys ecs::ECSImpl::GetSystem()
    #define Console ecs::ECSImpl
}

#endif /* AB1794DB_72E6_4ECF_848A_91CD7A70EDE6 */