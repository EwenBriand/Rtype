/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** Component.hpp
*/

#ifndef F3F6E7A7_9238_42B1_AAD9_8165C61C8F4C
#define F3F6E7A7_9238_42B1_AAD9_8165C61C8F4C
#include <string>
#include "metadata.hpp"

namespace ecs {
    template <typename T>
    class Component {
        public:
            /**
             * @brief This is played every frame.
             *
             * @param entityID
             */
            void Update(int entityID) {}

            /**
             * @brief This is played once at the start of the game.
             *
             */
            void Start() {}

            /**
             * @brief This is played when the components is to be loaded from a file.
             *
             * @param path
             */
            void Load(const std::string &path) {}

            /**
             * @brief This is to be played when the component is to be saved to a file.
             *
             */
            void Save(const std::string &path) {}

            /**
             * @brief This is to be played when the component is loaded (not necessarily from a file).
             *
             */
            void OnLoad() {}

            /**
             * @brief This function is called once when the component is added to
             * an entity.
             *
             * @return std::map<std::string, metadata_t>
             */
            void OnAddComponent(int entityID) {}

            std::map<std::string, metadata_t> GetMetadata() {
                std::map<std::string, metadata_t> metadata;
                return metadata;
            }

            std::string GetClassName() const {
                return "VanillaComponent-NoMetadata";
            }
    };
} // namespace ecs

#endif /* F3F6E7A7_9238_42B1_AAD9_8165C61C8F4C */
