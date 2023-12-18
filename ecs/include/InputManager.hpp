/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** InputManager.hpp
*/

#ifndef B93CCB91_6BD6_4169_B10A_5BE69A806B2A
#define B93CCB91_6BD6_4169_B10A_5BE69A806B2A
#include "raylib.h"
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class InputManager {
public:
    enum {
        KEYBOARD = 0,
        MBUTTON = 1,
        MPOSITION = 2,
    };

    struct EventInfo {
        std::string name;
        bool status; // true if the event is still down, false if the event is still up
        int countPressing;
        int key_code;
        int type; // 0 = keyboard, 1 = mouse button, 2 = mouse position
        std::vector<char> infoChar;
        Vector2 infoInt; // for mouse position
        Vector2 infoFloat; // for mouse wheel
    };

    struct EventDescription {
        /**
         * @brief The function to call to check if the event is triggered.
         * If the function returns true, the event is triggered.
         * If needed, fills the EventInfo structure with information about the event.
         * All char pressed are consumed before this function is called, and can be retrieved
         * using the WasCharPressed function.
         *
         */
        std::function<bool(EventInfo&)> testTriggered;

        /**
         * @brief The function to call when the event is triggered.
         *
         */
        std::function<void(EventInfo)> onTriggerCallback;
    };
    static std::unordered_map<std::string, EventDescription> Bindings;

    /**
     * @brief Add a new event to the InputManager.
     *
     * @param name The name of the event.
     * @param description The description of the event.
     */

    static void RegisterBinding(const std::string& name, InputManager::EventDescription description);

    /**
     * @brief Remove an event from the InputManager.
     *
     * @param name The name of the event to remove.
     */

    static void RemoveEvent(const std::string& name);

    /**
     * @brief Check if a binding is triggered. and execute his function if its the case
     *
     * @return none
     */

    void check_event_binding();

    bool isDown(const std::string& name);

    bool isDown(const int key_code);

    /**
     * @brief Get the EventInfo structure of an event.
     *
     * @param name The name of the event.
     * @return EventInfo The EventInfo structure of the event.
     */

    std::shared_ptr<InputManager::EventInfo> GetEventInfo(const std::string& name);

    /**
     * @brief Get PolledEvent.
     *
     * @return a std::vector<std::shared_ptr<EventInfo>>.
     */

    std::vector<std::shared_ptr<EventInfo>> GetPolledEvents();

    /**
     * @brief Check if the event have been realesed.
     *
     * @param name The name of the event.
     * @return Bool set to true if the key is up after being pressed.
     */

    bool isReleased(const std::string& name);

    bool isReleased(const int key_code);

    /**
     * @brief Get the number of press an event take.
     *
     * @param name The name of the event.
     * @return the number of time a key have been pressed.
     */

    int GetCountPressing(const std::string& name);

    /**
     * @brief Get the PollEvent.
     *
     * @param
     * @return none.
     */

    void PollEvents();
    /**
     * @brief Get the name of a key code.
     *
     * @param keyCode
     * @return none.
     */
    std::string KeyCodeTOName(int keyCode);
    /**
     * @brief Get the value of a key code.
     *
     * @param
     * @return none.
     */
    char KeyCodeTOChar(int keyCode);
    /**
     * @brief Get the last char pressed, take into account if caps lock was enabled.
     *
     * @param
     * @return none.
     */
    char GetLastCharPressed();

    /**
     * @brief Get mouse position.
     *
     * @param none.
     * @return none.
     */
    Vector2 MousePosition();

    /**
     * @brief Get mouse delta between frame.
     *
     * @param none.
     * @return none.
     */
    Vector2 MouseDelta();

    /**
     * @brief Get wheel movement.
     *
     * @param none.
     * @return none.
     */
    Vector2 MouseWheelMove();

    /**
     * @brief Get mouse button pressed.
     *
     * @param none.
     * @return none.
     */
    bool MouseButtonPressed(int button);

    /**
     * @brief Get mouse button released.
     *
     * @param none.
     * @return none.
     */

    bool MouseButtonReleased(int button);

private:
    std::vector<std::shared_ptr<EventInfo>> m_polledEvents;

    const std::vector<char> m_defaultKeys = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
        'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
        'u', 'v', 'w', 'x', 'y', 'z',
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        ' ', '\n', '\t', '\b', '\r', '\f', '\v', '\a', '\\',
        ' ', ',', ';', ':', '.', '!', '?', '/', '"', '\'',
        '(', ')', '[', ']', '{', '}', '<', '>', '|', '-',
        '_', '=', '+', '*', '&', '^', '%', '$', '#', '@' };
    std::function<bool(char, EventInfo&)> m_defaultCharPressedTest = [&](char c, EventInfo& info) {
        if (IsKeyPressed(c)) {
            info.name = std::string(1, c);
            info.infoChar.push_back(c);
            return true;
        }
        return false;
    };
};

// Enum 03: KeyboardKey (110 values)
//   Name: KeyboardKey
//   Description: Keyboard keys (US keyboard layout)
//   Value[KEY_NULL]: 0
//   Value[KEY_APOSTROPHE]: 39
//   Value[KEY_COMMA]: 44
//   Value[KEY_MINUS]: 45
//   Value[KEY_PERIOD]: 46
//   Value[KEY_SLASH]: 47
//   Value[KEY_ZERO]: 48
//   Value[KEY_ONE]: 49
//   Value[KEY_TWO]: 50
//   Value[KEY_THREE]: 51
//   Value[KEY_FOUR]: 52
//   Value[KEY_FIVE]: 53
//   Value[KEY_SIX]: 54
//   Value[KEY_SEVEN]: 55
//   Value[KEY_EIGHT]: 56
//   Value[KEY_NINE]: 57
//   Value[KEY_SEMICOLON]: 59
//   Value[KEY_EQUAL]: 61
//   Value[KEY_A]: 65
//   Value[KEY_B]: 66
//   Value[KEY_C]: 67
//   Value[KEY_D]: 68
//   Value[KEY_E]: 69
//   Value[KEY_F]: 70
//   Value[KEY_G]: 71
//   Value[KEY_H]: 72
//   Value[KEY_I]: 73
//   Value[KEY_J]: 74
//   Value[KEY_K]: 75
//   Value[KEY_L]: 76
//   Value[KEY_M]: 77
//   Value[KEY_N]: 78
//   Value[KEY_O]: 79
//   Value[KEY_P]: 80
//   Value[KEY_Q]: 81
//   Value[KEY_R]: 82
//   Value[KEY_S]: 83
//   Value[KEY_T]: 84
//   Value[KEY_U]: 85
//   Value[KEY_V]: 86
//   Value[KEY_W]: 87
//   Value[KEY_X]: 88
//   Value[KEY_Y]: 89
//   Value[KEY_Z]: 90
//   Value[KEY_LEFT_BRACKET]: 91
//   Value[KEY_BACKSLASH]: 92
//   Value[KEY_RIGHT_BRACKET]: 93
//   Value[KEY_GRAVE]: 96
//   Value[KEY_SPACE]: 32
//   Value[KEY_ESCAPE]: 256
//   Value[KEY_ENTER]: 257
//   Value[KEY_TAB]: 258
//   Value[KEY_BACKSPACE]: 259
//   Value[KEY_INSERT]: 260
//   Value[KEY_DELETE]: 261
//   Value[KEY_RIGHT]: 262
//   Value[KEY_LEFT]: 263
//   Value[KEY_DOWN]: 264
//   Value[KEY_UP]: 265
//   Value[KEY_PAGE_UP]: 266
//   Value[KEY_PAGE_DOWN]: 267
//   Value[KEY_HOME]: 268
//   Value[KEY_END]: 269
//   Value[KEY_CAPS_LOCK]: 280
//   Value[KEY_SCROLL_LOCK]: 281
//   Value[KEY_NUM_LOCK]: 282
//   Value[KEY_PRINT_SCREEN]: 283
//   Value[KEY_PAUSE]: 284
//   Value[KEY_F1]: 290
//   Value[KEY_F2]: 291
//   Value[KEY_F3]: 292
//   Value[KEY_F4]: 293
//   Value[KEY_F5]: 294
//   Value[KEY_F6]: 295
//   Value[KEY_F7]: 296
//   Value[KEY_F8]: 297
//   Value[KEY_F9]: 298
//   Value[KEY_F10]: 299
//   Value[KEY_F11]: 300
//   Value[KEY_F12]: 301
//   Value[KEY_LEFT_SHIFT]: 340
//   Value[KEY_LEFT_CONTROL]: 341
//   Value[KEY_LEFT_ALT]: 342
//   Value[KEY_LEFT_SUPER]: 343
//   Value[KEY_RIGHT_SHIFT]: 344
//   Value[KEY_RIGHT_CONTROL]: 345
//   Value[KEY_RIGHT_ALT]: 346
//   Value[KEY_RIGHT_SUPER]: 347
//   Value[KEY_KB_MENU]: 348
//   Value[KEY_KP_0]: 320
//   Value[KEY_KP_1]: 321
//   Value[KEY_KP_2]: 322
//   Value[KEY_KP_3]: 323
//   Value[KEY_KP_4]: 324
//   Value[KEY_KP_5]: 325
//   Value[KEY_KP_6]: 326
//   Value[KEY_KP_7]: 327
//   Value[KEY_KP_8]: 328
//   Value[KEY_KP_9]: 329
//   Value[KEY_KP_DECIMAL]: 330
//   Value[KEY_KP_DIVIDE]: 331
//   Value[KEY_KP_MULTIPLY]: 332
//   Value[KEY_KP_SUBTRACT]: 333
//   Value[KEY_KP_ADD]: 334
//   Value[KEY_KP_ENTER]: 335
//   Value[KEY_KP_EQUAL]: 336
//   Value[KEY_BACK]: 4
//   Value[KEY_MENU]: 82
//   Value[KEY_VOLUME_UP]: 24
//   Value[KEY_VOLUME_DOWN]: 25
// Enum 04: MouseButton (7 values)
//   Name: MouseButton
//   Description: Mouse buttons
//   Value[MOUSE_BUTTON_LEFT]: 0
//   Value[MOUSE_BUTTON_RIGHT]: 1
//   Value[MOUSE_BUTTON_MIDDLE]: 2
//   Value[MOUSE_BUTTON_SIDE]: 3
//   Value[MOUSE_BUTTON_EXTRA]: 4
//   Value[MOUSE_BUTTON_FORWARD]: 5
//   Value[MOUSE_BUTTON_BACK]: 6
// Enum 05: MouseCursor (11 values)
//   Name: MouseCursor
//   Description: Mouse cursor
//   Value[MOUSE_CURSOR_DEFAULT]: 0
//   Value[MOUSE_CURSOR_ARROW]: 1
//   Value[MOUSE_CURSOR_IBEAM]: 2
//   Value[MOUSE_CURSOR_CROSSHAIR]: 3
//   Value[MOUSE_CURSOR_POINTING_HAND]: 4
//   Value[MOUSE_CURSOR_RESIZE_EW]: 5
//   Value[MOUSE_CURSOR_RESIZE_NS]: 6
//   Value[MOUSE_CURSOR_RESIZE_NWSE]: 7
//   Value[MOUSE_CURSOR_RESIZE_NESW]: 8
//   Value[MOUSE_CURSOR_RESIZE_ALL]: 9
//   Value[MOUSE_CURSOR_NOT_ALLOWED]: 10
// Enum 06: GamepadButton (18 values)
//   Name: GamepadButton
//   Description: Gamepad buttons
//   Value[GAMEPAD_BUTTON_UNKNOWN]: 0
//   Value[GAMEPAD_BUTTON_LEFT_FACE_UP]: 1
//   Value[GAMEPAD_BUTTON_LEFT_FACE_RIGHT]: 2
//   Value[GAMEPAD_BUTTON_LEFT_FACE_DOWN]: 3
//   Value[GAMEPAD_BUTTON_LEFT_FACE_LEFT]: 4
//   Value[GAMEPAD_BUTTON_RIGHT_FACE_UP]: 5
//   Value[GAMEPAD_BUTTON_RIGHT_FACE_RIGHT]: 6
//   Value[GAMEPAD_BUTTON_RIGHT_FACE_DOWN]: 7
//   Value[GAMEPAD_BUTTON_RIGHT_FACE_LEFT]: 8
//   Value[GAMEPAD_BUTTON_LEFT_TRIGGER_1]: 9
//   Value[GAMEPAD_BUTTON_LEFT_TRIGGER_2]: 10
//   Value[GAMEPAD_BUTTON_RIGHT_TRIGGER_1]: 11
//   Value[GAMEPAD_BUTTON_RIGHT_TRIGGER_2]: 12
//   Value[GAMEPAD_BUTTON_MIDDLE_LEFT]: 13
//   Value[GAMEPAD_BUTTON_MIDDLE]: 14
//   Value[GAMEPAD_BUTTON_MIDDLE_RIGHT]: 15
//   Value[GAMEPAD_BUTTON_LEFT_THUMB]: 16
//   Value[GAMEPAD_BUTTON_RIGHT_THUMB]: 17
// Enum 07: GamepadAxis (6 values)
//   Name: GamepadAxis
//   Description: Gamepad axis
//   Value[GAMEPAD_AXIS_LEFT_X]: 0
//   Value[GAMEPAD_AXIS_LEFT_Y]: 1
//   Value[GAMEPAD_AXIS_RIGHT_X]: 2
//   Value[GAMEPAD_AXIS_RIGHT_Y]: 3
//   Value[GAMEPAD_AXIS_LEFT_TRIGGER]: 4
//   Value[GAMEPAD_AXIS_RIGHT_TRIGGER]: 5

#endif /* B93CCB91_6BD6_4169_B10A_5BE69A806B2A */
