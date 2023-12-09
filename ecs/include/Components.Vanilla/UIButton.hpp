/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** UIButton.hpp
*/

#ifndef C463D936_34D9_4022_8654_20F1ACFBB7F9
#define C463D936_34D9_4022_8654_20F1ACFBB7F9
#include "../Component.hpp"
#include "../Hoverable.hpp"
#include "../IGraphicalModule.hpp"
#include "UIDiv.hpp"
#include "metadata.hpp"
#include <iostream>
#include <unordered_map>

serialize class UIButton : public ecs::Component<UIButton>, public graph::Clickable {
public:
    GENERATE_METADATA(UIButton)

    static std::unordered_map<std::string, std::function<void()>> AvailableActionsClick;

    static void RegisterCallback(const std::string& name, std::function<void()>&& callback);

    /**
     * @brief Draws the button and checks for user input.
     *
     * @param entityID
     */
    void Update(int entityID);

    /**
     * @brief The button will attach itself to the div in the entity.
     */
    void OnAddComponent(int entityID);

    /**
     * @brief Registers utility commands to the cli.
     *
     */
    void OnLoad();

private:
    UIDiv* m_uiDiv = nullptr;
    serialize std::string m_description = "";
    serialize std::string m_text = "";
    serialize std::string m_clickCallbackName = "";
    std::string m_prevClickCallbackName = "";
};

#endif /* C463D936_34D9_4022_8654_20F1ACFBB7F9 */
