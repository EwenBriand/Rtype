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
    int act_event = GetKeyPressed();

    while (act_event != 0) {
        event_list.push_back(act_event);
        act_event = GetKeyPressed();
    }

    for (const auto& event : m_polledEvents) {
        event->countPressing = 0;
    }

    for (int i = 0; i < event_list.size(); i++) {
        std::string current = "key code " + std::to_string(event_list[i]);
        if (m_polledEvents.empty()) {
            EventInfo newEvent;
            newEvent.name = current;
            newEvent.status = true;
            newEvent.countPressing = 1;
            newEvent.key_code = event_list[i];
            newEvent.type = 0;
            m_polledEvents.push_back(std::make_shared<EventInfo>(newEvent));
            continue;
        }
        for (const auto& event : m_polledEvents) {
            if (event->key_code == event_list[i] && event->type == 0) {
                event->status = true;
                event->countPressing++;
                break;
            }
            if (event == m_polledEvents.back()) {
                EventInfo newEvent;
                newEvent.name = current;
                newEvent.status = true;
                newEvent.countPressing = 1;
                newEvent.key_code = event_list[i];
                newEvent.type = 0;
                m_polledEvents.push_back(std::make_shared<EventInfo>(newEvent));
                break;
            }
        }
    }

    for (const auto& event : m_polledEvents) {
        if (event->countPressing > 0 || event->status == true) {
            if (IsKeyReleased(event->key_code)) {
                event->status = false;
                event->countPressing = 1;
                // std::cout << "up2" << std::endl;
            }
            // else
            // std::cout << "down2" << std::endl;
            // std::cout << event->name << " " << event->countPressing << " " << event->status << std::endl;
        }
    }

    check_event_binding();
}

bool InputManager::isDown(const std::string& name)
{
    for (const auto& event : m_polledEvents) {
        if (event->name == name && event->status) {
            return event->status;
        }
    }
    throw std::runtime_error("Event not found");
}

bool InputManager::isUp(const std::string& name)
{
    for (const auto& event : m_polledEvents) {
        if (event->name == name && event->countPressing > 0 && !event->status) {
            return true;
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
    std::cout << "GetPolledEvents " << m_polledEvents.size() << std::endl;
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

std::string InputManager::KeyCodeTOName(int keyCode)
{
    std::string keyName = "key code " + std::to_string(keyCode);
    return keyName;
}

char InputManager::KeyCodeTOChar(int keyCode)
{
    std::map<int, char> map = {
        { 0, '\0' },
        { 39, '\'' },
        { 44, ',' },
        { 45, '-' },
        { 46, '.' },
        { 47, '/' },
        { 48, '0' },
        { 49, '1' },
        { 50, '2' },
        { 51, '3' },
        { 52, '4' },
        { 53, '5' },
        { 54, '6' },
        { 55, '7' },
        { 56, '8' },
        { 57, '9' },
        { 59, ';' },
        { 61, '=' },
        { 65, 'A' },
        { 66, 'B' },
        { 67, 'C' },
        { 68, 'D' },
        { 69, 'E' },
        { 70, 'F' },
        { 71, 'G' },
        { 72, 'H' },
        { 73, 'I' },
        { 74, 'J' },
        { 75, 'K' },
        { 76, 'L' },
        { 77, 'M' },
        { 78, 'N' },
        { 79, 'O' },
        { 80, 'P' },
        { 81, 'Q' },
        { 82, 'R' },
        { 83, 'S' },
        { 84, 'T' },
        { 85, 'U' },
        { 86, 'V' },
        { 87, 'W' },
        { 88, 'X' },
        { 89, 'Y' },
        { 90, 'Z' },
        { 91, '[' },
        { 92, '\\' },
        { 93, ']' },
        { 96, '`' },
        { 32, ' ' },
        { 257, '\n' },
        { 258, '\t' },
        { 320, '0' },
        { 321, '1' },
        { 322, '2' },
        { 323, '3' },
        { 324, '4' },
        { 325, '5' },
        { 326, '6' },
        { 327, '7' },
        { 328, '8' },
        { 329, '9' },
        { 330, '.' },
        { 331, '/' },
        { 332, '*' },
        { 333, '-' },
        { 334, '+' },
        { 335, '\n' },
        { 336, '=' }
    };
    return map[keyCode];
}

char InputManager::GetLastCharPressed()
{
    if (isDown(KeyCodeTOName(KEY_LEFT_SHIFT)) || isDown(KeyCodeTOName(KEY_RIGHT_SHIFT)) || isDown(KeyCodeTOName(KEY_CAPS_LOCK))) {
        for (const auto& event : m_polledEvents) {
            if (event->type == 0 && isDown(event->name)) {
                if (event->key_code >= 65 && event->key_code <= 90)
                    return KeyCodeTOChar(event->key_code) + 32;
                return KeyCodeTOChar(event->key_code);
            }
        }
    } else {
        for (const auto& event : m_polledEvents) {
            if (event->type == 0 && isDown(event->name)) {
                return KeyCodeTOChar(event->key_code);
            }
        }
    }
    return '\0';
}

Vector2 InputManager::MousePosition()
{
    return GetMousePosition();
}

Vector2 InputManager::MouseDelta()
{
    return GetMouseDelta();
}

Vector2 InputManager::MouseWheelMove()
{
    return GetMouseWheelMoveV();
}

bool InputManager::MouseButtonPressed(int button)
{
    return IsMouseButtonPressed(button);
}

bool InputManager::MouseButtonReleased(int button)
{
    return IsMouseButtonReleased(button);
}

void InputManager::check_event_binding()
{

    for (const auto& bind : Bindings)
        for (const auto& event : m_polledEvents)
            if (bind.second.testTriggered(*event)) {
                bind.second.onTriggerCallback(*event);
            }
}
