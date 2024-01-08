/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** Text.cpp
*/

#include "Text.hpp"
#include "../ECSImpl.hpp"
#include "CLI.hpp"
#include "Engine.hpp"


// void updateText(int entityID)

void Text::UpdateText(std::string text)
{
    // m_text += text 
    // push the new text in the std::string m_text and add a \n 
    m_text += text + "\n";
}

void Text::Update(int entityID)
{
    // display the text with raylib
    DrawText(m_text.c_str(), m_position.x, m_position.y, 12, { 255, 255, 255, 255 });
}


std::string Text::GetText()
{
    // return the text
    return m_text;
}

void Text::SetPosition(graph::vec2f pos)
{
    // set the position of the text
    // m_uiDiv->SetPosition(pos);
    m_position = pos;
}