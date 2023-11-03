/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** EntityExplorer.hpp
*/

#pragma once
#include <vector>
#include <memory>
#include "../Entity.hpp"
#include "../Types.hpp"
#include "TextField.hpp"
#include "metadata.hpp"

/**
 * @brief This Component is used to inspect and modify the components of an entity.
 * It can also add a new entity to the scene.
 * It should only be used in the editor.
 *
 */
class EntityExplorer : public ecs::Component<EntityExplorer> {
    public:
        void Update(int entityID);
        std::string GetClassName() {return "EntityExplorer";};

    private:
        class EntityWatcher {
            public:
                EntityWatcher(Entity entity);
                int entityID = 0;
                void Update();
            private:
                /**
                 * @brief Counts the number of components on the entity.
                 *
                 */
                int countCpt() const;

                /**
                 * @brief Updates the value of the text field if the
                 * value of the component has changed.
                 *
                 */
                void updateTextFieldsValues();

                int cptCount = 0;
                std::vector<std::tuple<UIDiv, std::shared_ptr<TextField>>> fieldsAccessors;
                std::vector<CptMetadata> metadata;
                std::vector<std::function<void()>> onEnterCallbacks;
        };

        std::shared_ptr<EntityWatcher> m_watcher = nullptr;
};
