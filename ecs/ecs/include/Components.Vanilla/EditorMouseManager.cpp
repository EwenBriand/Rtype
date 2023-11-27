/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** EditorMouseManager.cpp
*/

#include <functional>
#include "../Engine.hpp"
#include "../ECSImpl.hpp"
#include "../IGraphicalModule.hpp"
#include "EditorMouseManager.hpp"

void EditorMouseManager::OnAddComponent(int entityID)
{
    Sys.GetInputManager().RegisterBinding(
        "MouseClick-Editor",
        {
            .testTriggered = [&](__attribute__((unused)) InputManager::EventInfo &info) {
                bool clicked = Sys.GetGraphicalModule()->WindowIsMouseLeftDown();
                if (!clicked) {
                    m_isPressed = false;
                    m_mousePrevPos = Sys.GetGraphicalModule()->WindowGetMousePos();
                }
                return clicked;
            },
            .onTriggerCallback = [&](__attribute__((unused)) InputManager::EventInfo info){
                setCtxtClosestEntity();
            }
        }
    );
}

void EditorMouseManager::setCtxtClosestEntity()
{
    std::vector<Entity> entities = Sys.GetEntities();
    Entity closestEntity = -1;
    float closestDistance = INT32_MAX;

    if (m_isPressed)
        return;
    m_isPressed = true;
    for (auto &entity : entities) {
        try {
            auto &transform = Sys.GetComponent<CoreTransform>(entity);
            if (closestEntity == -1) {
                closestEntity = entity;
                closestDistance = graph::distance(transform.GetScreenPosition(), Sys.GetGraphicalModule()->WindowGetMousePos());
                continue;
            }
            float distance = graph::distance(transform.GetScreenPosition(), Sys.GetGraphicalModule()->WindowGetMousePos());
            if (distance < closestDistance) {
                closestEntity = entity;
                closestDistance = distance;
            }
        } catch (std::exception &e) {
            continue;
        }
    }
    if (closestEntity != -1 && closestDistance < 5) {
        Sys.SetEditorEntityContext(closestEntity);
        Console::info << "Selected entity: " << closestEntity << std::endl;
    }
}

void EditorMouseManager::checkForDrag()
{
    graph::vec2f mousePos = Sys.GetGraphicalModule()->WindowGetMousePos();
    graph::vec2f delta = {
        mousePos.x - m_mousePrevPos.x,
        mousePos.y - m_mousePrevPos.y};
    if (graph::distance(delta, {0, 0}) < 5) {
        m_mousePrevPos = mousePos;
        return;
    }
    m_mousePrevPos = mousePos;
    Entity entity = Sys.GetEditorEntityContext();
    if (entity == -1)
        return;
    try {
        auto &transform = Sys.GetComponent<CoreTransform>(entity);
        transform.x += delta.x;
        transform.y += delta.y;
    } catch (std::exception &e) {
        return;
    }
}

void EditorMouseManager::Update(int entityID)
{
    Entity ctxt = Sys.GetEditorEntityContext();

    if (ctxt == -1)
        return;
    try {
        auto &transform = Sys.GetComponent<CoreTransform>(ctxt);
        Sys.GetGraphicalModule()->WindowDrawCircle({
            .pos = transform.GetScreenPosition(),
            .radius = 5,
            .borderColor = {255, 255, 255, 255},
            .borderSize = 1
        });
    } catch (std::exception &e) {
        return;
    }
    if (m_isPressed)
        checkForDrag();
}

std::string EditorMouseManager::GetClassName() const
{
    return "EditorMouseManager";
}

void EditorMouseManager::Save(const std::string &path)
{

}
