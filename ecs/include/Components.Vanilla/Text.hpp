/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** Text.hpp
*/

#ifndef C463D936_34D9_4022_864_20F1ACFBB7F912B
#define C463D936_34D9_4022_864_20F1ACFBB7F912B
#include "../Component.hpp"
#include "../Hoverable.hpp"
#include "../IGraphicalModule.hpp"
#include "UIDiv.hpp"
#include "metadata.hpp"
#include <iostream>
#include <unordered_map>

serialize class Text : public ecs::Component<Text>, public graph::Clickable {
public:
    GENERATE_METADATA(Text)

    // static std::unordered_map<std::string, std::function<void()>> AvailableActionsClick;

    // static void RegisterCallback(const std::string& name, std::function<void()>&& callback);

    /**
     * @brief Draws the button and checks for user input.
     *
     * @param entityID
     */
    void UpdateText(std::string text);
    

    /**
     * @brief
     * update the text
     * @param entityID
     */

    void Update(int entityID);
    /**
     * @brief Registers utility commands to the cli.
     *
     */
    // void OnLoad();


    /**
     * @brief Set position of the text
     *
     */
    void SetPosition(graph::vec2f pos);

    /**
     * @brief Set size of the text
     *
     */
    // void SetSize(graph::vec2f size);

     std::string GetText();

private:
    UIDiv* m_uiDiv = nullptr;
    // serialize std::string m_text = "";
    // vector of strings
    std::string m_text;
    graph::vec2f m_position = { 0, 0 };
    graph::vec2f m_size;
};

#endif
