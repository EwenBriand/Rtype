/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** EntityExplorer.cpp
*/

#include "EntityExplorer.hpp"
#include "../ECSImpl.hpp"

EntityExplorer::EntityWatcher::EntityWatcher(Entity e)
    : entityID(e)
{
    // std::vector<std::string> types = SYS.GetComponentsTypes();
    int delta = 0;
    int cptIndex = 0;

    cptCount = countCpt();
    SYS.ForEachComponent(e, [&](ecs::ECSImpl::AnyCpt& cpt) {
        std::visit([&](auto&& arg) {
            metadata.push_back(arg.GetMetadata());
            std::map<std::string, metadata_t>& m = metadata.back();

            for (auto& [key, value] : m) {
                fieldsAccessors.emplace_back();
                auto& [div, text] = fieldsAccessors[delta];
                div.SetParentID(0);
                div.SetPosition({ 32, 50.0f + delta * 20 });
                div.SetSize({ 200, 20 });
                div.SetColor({ 33, 33, 33, 255 });

                text = std::make_shared<TextField>();
                text->SetParent(div);
                text->SetLabel(key);
                text->SetPlaceholder(value.type + " : " + value.tostring());
                text->SetupCallbacks();
                text->RegisterOnEnterCallback([this, delta, key, cptIndex]() {
                    auto textfield = std::get<1>(fieldsAccessors[delta]);
                    auto& valueMeta = metadata[cptIndex][key];
                    valueMeta.set(textfield->GetText());
                    textfield->SetText("");
                    textfield->SetPlaceholder(valueMeta.type + " : " + valueMeta.tostring());
                    textfield->UnFocus();
                });
                ++delta;
            }
            ++cptIndex;
        },
            cpt);
    });
}

int EntityExplorer::EntityWatcher::countCpt() const
{
    int count = 0;
    std::vector<std::string> types = SYS.GetComponentsTypes();

    for (auto type : types) {
        std::vector<ecs::ECSImpl::AnyCpt> cpts = SYS.GetComponentsByName(entityID, type);
        count += cpts.size();
    }
    return count;
}

void EntityExplorer::EntityWatcher::updateTextFieldsValues()
{
    int i = 0;
    int fieldIdx = 0;
    SYS.ForEachComponent(entityID, [&](ecs::ECSImpl::AnyCpt& cpt) {
        std::visit([&](auto&& arg) {
            std::map<std::string, metadata_t>& m = metadata[i];

            for (auto& [key, value] : m) {
                auto& textfield = std::get<1>(fieldsAccessors[fieldIdx++]);
                if (textfield->IsFocused())
                    continue;
                textfield->SetPlaceholder(value.type + " : " + value.tostring());
            }
            ++i;
        },
            cpt);
    });
}

void EntityExplorer::EntityWatcher::Update()
{
    auto validEntities = SYS.GetEntities();
    if (std::find(validEntities.begin(), validEntities.end(), entityID) == validEntities.end()) {
        throw std::runtime_error("EntityWatcher::Update: entityID is not valid");
    }
    int newCount = countCpt();

    if (newCount != cptCount)
        throw std::runtime_error("EntityWatcher::Update: cptCount != newCount");

    updateTextFieldsValues();
    for (auto& field : fieldsAccessors) {
        std::get<0>(field).SetParentID(0);
        std::get<1>(field)->SetParent(std::get<0>(field));
        std::get<0>(field).Update(entityID);
        std::get<1>(field)->Update(entityID);
    }
}

void EntityExplorer::Update(int entityID)
{
    Entity context = SYS.GetEditorEntityContext();

    if (context <= 0)
        return;

    if (m_watcher == nullptr) {
        m_watcher = std::make_shared<EntityWatcher>(context);
    } else if (m_watcher->entityID != context) {
        m_watcher = std::make_shared<EntityWatcher>(context);
    }
    try {
        m_watcher->Update();
    } catch (std::exception& e) {
        m_watcher = std::make_shared<EntityWatcher>(context);
    }
}
