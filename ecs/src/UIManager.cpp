/*
** EPITECH PROJECT, 2023
** tmp2
** File description:
** UIManager.cpp
*/

#include "ECSImpl.hpp"
#include "Engine.hpp"
#include "UIManager.hpp"
#include "CLI.hpp"

namespace ui {
// ----------------------------------------------------------------------------
// Public methods
// ----------------------------------------------------------------------------

    UIManager &UIManager::Get() {
        static UIManager instance;
        return instance;
    }

    UIManager::UIManager() : _storage() {
        registerSystems();

        // here be the declaration of all ui elements
        // _storage.RegisterComponent<Text>(_systemText);


        if (eng::Engine::GetEngine()->IsOptionSet(eng::Engine::Options::EDITOR)) {
            registerEditorHandles();
        }

        eng::Engine::GetEngine()->pushPipeline([&]() {
            Draw();
        }, 499);
    }

    // ! Capture the pointers by value to avoid dangling pointers !
    void UIManager::registerSystems() {

            auto systemTextField = ([&](TextField *textField) {
                _drawBuffer.push_back(std::make_tuple(textField->layer, [textField, this]() {
                    Color colorToDraw = textField->text == "" ? GRAY : textField->color;
                    if (textField->text.size() > textField->length) {
                        textField->text = textField->text.substr(0, textField->length);
                    }
                    DrawRectangle(textField->position.x, textField->position.y, textField->length * textField->fontSize, textField->fontSize, WHITE);
                    DrawText(textField->text.c_str(), textField->position.x, textField->position.y, textField->fontSize, colorToDraw);
                    if (CheckCollisionPointRec(GetMousePosition(), {textField->position.x, textField->position.y, static_cast<float>(textField->length * textField->fontSize), static_cast<float>(textField->fontSize)})) {
                        _focusedTextField = textField;
                    } else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && _focusedTextField == textField) {
                        _focusedTextField = nullptr;
                    }
                    if (_focusedTextField == textField) {
                        if (IsKeyPressed(KEY_BACKSPACE)) {
                            textField->text = textField->text.substr(0, textField->text.size() - 1);
                        } else if (IsKeyPressed(KEY_ENTER)) {
                            textField->callback(textField->text);
                        } else {
                            for (int i = 32; i < 127; i++) {
                                if (IsKeyPressed(i)) {
                                    textField->text += static_cast<char>(i);
                                }
                            }
                        }
                    }
                }));
            });
            _storage.RegisterComponent<TextField>(systemTextField);


            auto _systemText = ([&](Text *text) {
                _drawBuffer.push_back(std::make_tuple(text->layer, [text]() {
                    DrawText(text->text.c_str(), text->position.x, text->position.y, text->fontSize, text->color);
                }));
            });
            _storage.RegisterComponent<Text>(_systemText);
    }


    void UIManager::Draw() {
        _storage.RunSystems<
            Text,
            TextField
        >();
        std::sort(_drawBuffer.begin(), _drawBuffer.end(), [](auto &a, auto &b) {
            return std::get<0>(a) < std::get<0>(b);
        });
        for (auto &it : _drawBuffer) {
            std::get<1>(it)();
        }
        _drawBuffer.clear();
    }

// ----------------------------------------------------------------------------
// Text utils
// ----------------------------------------------------------------------------
    std::size_t UIManager::AddText(const struct Text &text) {
        return _storage.AddComponent<Text>(new Text(text));
    }

// ----------------------------------------------------------------------------
// Private methods
// ----------------------------------------------------------------------------
    void UIManager::registerEditorHandles() {
        try {
            auto  &cli = Sys.GetComponent<CLI>(Sys.GetSystemHolder());

    // TEXT ///////////////////////////////////////////////////////////////////

            cli.RegisterCustomCommand("ui-ls-texts", [&](__attribute__((unused)) CLI &, __attribute__((unused)) const std::vector<std::string> &) {
                _storage.ForEach<Text>([&](Text *text, std::size_t index) {
                    Console::info << "[" << index << "]: '" << text->text << "'" << std::endl;
                });
            }, "List all texts in the ui manager with their id");

            cli.RegisterCustomCommand("ui-set-text", [&](CLI &, const std::vector<std::string> &args) {
                if (args.size() - 1 != 2) {
                    Console::err << "Invalid number of arguments" << std::endl;
                    return;
                }
                std::size_t index = std::stoi(args[0]);
                try {
                    auto text = _storage.GetComponent<Text>(index);
                    text->text = args[1];
                } catch (std::exception &e) {
                    Console::err << "Invalid text id" << std::endl;
                }
            }, "Set the text of a text element in the ui manager");

            cli.RegisterCustomCommand("ui-mv-text", [&](CLI &, const std::vector<std::string> &args) {
                if (args.size() - 1 != 3) {
                    Console::err << "Invalid number of arguments" << std::endl;
                    return;
                }
                std::size_t index = std::stoi(args[0]);
                try {
                    auto text = _storage.GetComponent<Text>(index);
                    text->position.x = std::stoi(args[1]);
                    text->position.y = std::stoi(args[2]);
                } catch (std::exception &e) {
                    Console::err << "Invalid text id" << std::endl;
                }
            }, "Move a text element in the ui manager");

        } catch (std::exception &e) {
            Console::warn << "No CLI component found in the system holder entity. "
                          << "The ui manager's editor handles will not be registered." << std::endl;
        }

    }
}
