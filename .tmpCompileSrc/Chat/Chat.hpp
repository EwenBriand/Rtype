/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** Chat.hpp
*/

#ifndef D3B0DB9C_B659_4BE4_8F47_894357FC12F90
#define D3B0DB9C_B659_4BE4_8F47_894357FC12F90

#include "metadata.hpp"
#include "Components.Vanilla/UserComponentWrapper.hpp"
#include "Components.Vanilla/TextField.hpp"
#include "Components.Vanilla/InputField.hpp"
#include "Components.Vanilla/Text.hpp"
// #include "Components.Vanilla/UIDiv.hpp"

BUILD_MANUAL

/**
 * @brief displays buttons to pause the game and to quit the game
 *
 */
serialize class Chat : public AUserComponent {
public:
    GENERATE_METADATA(Chat)
    Chat() = default;
    ~Chat() = default;

    void Start() override;
    void Update(int e) override;
    void OnAddComponent(int entityID) override;
    void AddMessage(std::string message);

private:
    TextField *_textField; 
    InputField *_inputField;
    std::string _textBuffer = "";
    Text *_text;
    // UIDiv div;
    int _entity;
    void sendMessage();
    void checkSizeOldText();
};

#endif /* D3B0DB9C_B659_4BE4_8F47_894357FC12F90 */
    