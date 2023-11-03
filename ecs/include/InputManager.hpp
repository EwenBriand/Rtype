/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** InputManager.hpp
*/

#ifndef B93CCB91_6BD6_4169_B10A_5BE69A806B2A
#define B93CCB91_6BD6_4169_B10A_5BE69A806B2A
#include <unordered_map>
#include <vector>
#include <memory>
#include <functional>
#include <string>
#include <iostream>


class InputManager {
    public:

        struct EventInfo {
            std::string name;
            std::vector<char> infoChar;
            std::vector<int> infoInt;
            std::vector<float> infoFloat;
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
            std::function<bool(EventInfo &)> testTriggered;

            /**
             * @brief The function to call when the event is triggered.
             *
             */
            std::function<void(EventInfo)> onTriggerCallback;
        };
        static std::unordered_map<std::string, EventDescription> Bindings;
        static std::unordered_map<std::string, bool> Flags;

        /**
         * @brief Registers a binding to the input manager. This function needs
         * to know the name corresponding to the event, as well as the function
         * to call when the event is triggered. It also needs a function to call
         * to check if the event is triggered.
         *
         * @param key
         * @param description
         */
        static void RegisterBinding(const std::string &key, EventDescription description);

        /**
         * @brief Raises a flag.
         *
         * @param flag
         */
        static void RaiseFlag(const std::string &flag);

        /**
         * @brief Lowers a flag.
         *
         * @param flag
         */
        static void LowerFlag(const std::string &flag);

        /**
         * @brief Returns true if the flag is up, false if it is down or not set.
         *
         * @param flag
         */
        static bool IsFlagUp(const std::string &flag);

        /**
         * @brief Polls the events and calls the callbacks if the key is pressed.
         *
         * @param entityID
         */
        void PollEvents();

        /**
         * @brief Returns a vector with the string id of all events polled during the frame.
         * Events are not duplicated.
         *
         */
        std::vector<EventInfo> GetEvents();

        /**
         * @brief returns true if the char was pressed during the frame.
         *
         */
        bool WasCharPressed(char c);

        /**
         * @brief Sets up the default inputs (letters for exemple)
         *
         */
        void SetupDefaults();

        /**
         * @brief returns a reference to the polled events
         *
         */
        std::vector<EventInfo> &GetPolled();

        /**
         * @brief removes an input from the list of polled inputs.
         *
         */
        void Consume(const std::string &name);

    private:
        std::vector<EventInfo> m_polledEvents;
        std::vector<int> m_charPressed;

        const std::vector<char> m_defaultKeys = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
                                           'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
                                           'u', 'v', 'w', 'x', 'y', 'z',
                                           '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                                           ' ', '\n', '\t', '\b', '\r', '\f', '\v', '\a', '\\',
                                           ' ', ',', ';', ':', '.', '!', '?', '/', '"', '\'',
                                           '(', ')', '[', ']', '{', '}', '<', '>', '|', '-',
                                           '_', '=', '+', '*', '&', '^', '%', '$', '#', '@'};
        std::function<bool(char, EventInfo &)> m_defaultCharPressedTest = [&](char c, EventInfo &info) {
            if (WasCharPressed(c)) {
                info.name = std::string(1, c);
                info.infoChar.push_back(c);
                return true;
            }
            return false;
        };
};

#endif /* B93CCB91_6BD6_4169_B10A_5BE69A806B2A */
