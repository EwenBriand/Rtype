/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** InputManager.cpp
*/

#include "InputManager.hpp"
#include "ECSImpl.hpp"
#include <algorithm>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

std::unordered_map<std::string, InputManager::EventDescription> InputManager::Bindings;

void InputManager::RegisterBinding(const std::string& key, InputManager::EventDescription description)
{
    Bindings[key] = description;
}

void InputManager::PollEvents()
{
    std::vector<int> event_list;
    int act_event = GetCharPressed();

    while (act_event != 0) {
        event_list.push_back(act_event);
        act_event = GetCharPressed();
    }

    for (const auto& event : m_polledEvents) {
        event->countPressing = 0;
    }

    for (int i = 0; i < event_list.size(); i++) {
        std::string current(1, static_cast<char>(event_list[i]));
        if (m_polledEvents.empty()) {
            EventInfo newEvent;
            newEvent.name = current;
            newEvent.status = false;
            newEvent.countPressing = 1;
            m_polledEvents.push_back(std::make_shared<EventInfo>(newEvent));
            continue;
        }
        for (const auto& event : m_polledEvents) {
            if (event->name == current) {
                event->status = false;
                event->countPressing++;
                break;
            }
            if (event == m_polledEvents.back()) {
                EventInfo newEvent;
                newEvent.name = current;
                newEvent.status = false;
                newEvent.countPressing = 1;
                m_polledEvents.push_back(std::make_shared<EventInfo>(newEvent));
                break;
            }
        }
    }

    for (const auto& event : m_polledEvents) {
        if (event->countPressing > 0)
            std::cout << event->name << " " << event->countPressing << " " << event->status << std::endl;
    }
}

bool InputManager::isDown(const std::string& name)
{
    for (const auto& event : m_polledEvents) {
        if (event->name == name && event->countPressing > 0) {
            return event->status;
        }
    }
    return false;
}

bool InputManager::isUp(const std::string& name)
{
    for (const auto& event : m_polledEvents) {
        if (event->name == name && event->countPressing > 0) {
            return event->countPressing == 1;
        }
    }
    return false;
}

std::shared_ptr<InputManager::EventInfo> InputManager::GetEventInfo(const std::string& name)
{
    for (const auto& event : m_polledEvents) {
        if (event->name == name) {
            return event;
        }
    }
    throw std::runtime_error("Event not found");
}

std::vector<std::shared_ptr<InputManager::EventInfo>> InputManager::GetPolledEvents()
{
    return m_polledEvents;
}

int InputManager::GetCountPressing(const std::string& name)
{
    for (const auto& event : m_polledEvents) {
        if (event->name == name) {
            return event->countPressing;
        }
    }
    return 0;
}
