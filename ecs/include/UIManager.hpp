/*
** EPITECH PROJECT, 2023
** tmp2
** File description:
** UIManager.hpp
*/

#pragma once
#include "GenericStorageSystem.hpp"
#include <functional>
#include <raylib.h>

/**
 * @brief private define to register a ui element in and its system in the storage.
 * All system should be named system<type>.
 *
 */
#define REG_UI(type) _storage.RegisterComponent<type>([&](type* ui) { _system##type(ui); })

namespace ui {
    /**
     * @brief A class to manage the ui. It uses its own types to represent
     * the UI elements. Do not mix them with the ecs components such as UIButton, TextField, etc.
     * All available elements are subclasses of the UIElement class.
     *
     */
    class UIManager {
    public:
        static const int DEFAULT_TEXT_LAYER = 10;

        struct Text {
            std::string text;
            Vector2 position;
            int fontSize;
            Color color = WHITE;
            int layer = DEFAULT_TEXT_LAYER;
        };

        struct TextField {
            std::string text;
            std::string placeholder = "...";
            Vector2 position;
            int fontSize;
            Color color = BLACK;
            std::size_t length = 20;
            std::function<void(std::string)> callback;
            int layer = DEFAULT_TEXT_LAYER;
        };

        // ----------------------------------------------------------------------------
        // public methods
        // ----------------------------------------------------------------------------
        /**
         * @brief Getter to implement the singleton pattern.
         *
         * @return UIManager&
         */
        static UIManager& Get();

        /**
         * @brief Construct a new UIManager object.
         * UIManager is a singleton, don't use unless you know what you are
         * doing!
         *
         */
        UIManager();

        /**
         * @brief Draws the UI.
         *
         */
        void Draw();

        /**
         * @brief Adds a group of UIElements to the ui and returns a handle to
         * the group. This handle can be used to remove the group from the ui using
         * the RemoveGroup method.
         * The Added Elements must be copy constructible.
         *
         * @return std::size_t
         */
        template <typename... Args>
        std::size_t AddGroup(Args... args)
        {
            std::vector<std::tuple<std::string, std::size_t>> handles;
            (handles.push_back(std::make_tuple<std::string, std::size_t>(
                 std::string(typeid(Args).name()),
                 _storage.AddComponent(new Args(args)))),
                ...);
            std::size_t handle = 0;
            while (_groups.find(handle) != _groups.end())
                handle++;
            std::cout << "keeping handle: " << handle << std::endl;
            _groups.emplace(handle, handles);
            return handle;
        }

        /**
         * @brief Removes a group of UIElements from the ui.
         *
         * @param handle
         */
        void RemoveGroup(std::size_t handle)
        {
            std::cout << "removing group: " << handle << std::endl;
            if (_groups.find(handle) == _groups.end())
                throw std::runtime_error("Invalid handle");
            std::cout << "no error here" << std::endl;
            std::vector<std::tuple<std::string, std::size_t>> group = _groups[handle];
            std::for_each(group.begin(), group.end(), [&](auto& elem) {
                _storage.RemoveAt(std::get<1>(elem), std::get<0>(elem));
            });
            _groups.erase(handle);
        }

        // ----------------------------------------------------------------------------
        // Text utils
        // ----------------------------------------------------------------------------
        std::size_t AddText(const struct Text&);

    private:
        // ----------------------------------------------------------------------------
        // Systems for ui elements
        // ----------------------------------------------------------------------------
        void registerSystems();

        std::function<void(Text*)> _systemText;

        // ----------------------------------------------------------------------------
        // Private methods
        // ----------------------------------------------------------------------------
        void registerEditorHandles();

        // ----------------------------------------------------------------------------
        // Private attributes
        // ----------------------------------------------------------------------------

        ecs::GenericStorageSystem _storage;
        std::map<std::size_t, std::vector<std::tuple<std::string, std::size_t>>> _groups;
        std::vector<std::tuple<int, std::function<void()>>> _drawBuffer;

        TextField* _focusedTextField = nullptr;
    };
} // namespace ui
