/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** InputManager.cpp
*/

#include <stdexcept>
#include <algorithm>
#include <string>
#include "InputManager.hpp"
#include "ECSImpl.hpp"

std::unordered_map<std::string, InputManager::EventDescription> InputManager::Bindings;
std::unordered_map<std::string, bool> InputManager::Flags;


void InputManager::SetupDefaults() {
    for (auto letter : m_defaultKeys) {
        RegisterBinding(
            "PressedKey " + std::string(1, letter),
            {
                std::bind(m_defaultCharPressedTest, letter, std::placeholders::_1),
            }
        );
    }
}

bool InputManager::WasCharPressed(char c)
{
    for (int i = 0; i < m_charPressed.size(); ++i) {
        if (m_charPressed[i] == c) {
            return true;
        }
    }
    return false;
}

void InputManager::RegisterBinding(const std::string &key, InputManager::EventDescription description)
{
    Bindings[key] = description;
}

void InputManager::PollEvents()
{
    char tmpChar = 1;
    auto graphical = Sys.GetGraphicalModule();
    m_polledEvents.clear();
    m_charPressed.clear();
    while ((tmpChar = graphical->GetNextCharPressed()) != -1) {
        if (tmpChar != 0) {
            m_charPressed.push_back(tmpChar);
        }
    }
    bool duplicate = false;
    for (auto &binding : Bindings) {
        EventInfo tmp;
        duplicate = false;
        if (binding.second.testTriggered(tmp)) {
            for (auto evt : m_polledEvents) {
                if (evt.name == binding.first) {
                    duplicate = true;
                    break;
                }
            }
            if (duplicate)
                continue;
            if (binding.second.onTriggerCallback)
                binding.second.onTriggerCallback(tmp);
            m_polledEvents.push_back({binding.first, tmp.infoChar, tmp.infoInt, tmp.infoFloat});
        }
    }
}

std::vector<InputManager::EventInfo> InputManager::GetEvents()
{
    return m_polledEvents;
}

void InputManager::RaiseFlag(const std::string &flag)
{
    Flags[flag] = true;
}

void InputManager::LowerFlag(const std::string &flag)
{
    Flags[flag] = false;
}

bool InputManager::IsFlagUp(const std::string &flag)
{
    try {
        return Flags.at(flag);
    } catch (std::out_of_range &e) {
        return false;
    }
}

std::vector<InputManager::EventInfo> &InputManager::GetPolled()
{
    return m_polledEvents;
}

void InputManager::Consume(const std::string &name)
{
    for (int i = 0; i < m_polledEvents.size(); ++i) {
        if (m_polledEvents[i].name == name) {
            m_polledEvents.erase(m_polledEvents.begin() + i);
            return;
        }
    }
}
