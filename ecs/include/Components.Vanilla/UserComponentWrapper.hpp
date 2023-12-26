/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** Script.hpp
*/

#pragma once

#include "../Component.hpp"
#include "metadata.hpp"
#include <iostream>
#include <memory>

/**
 * @brief This class brings back classical inheritance to the ECS to allow
 * runtime polymorphism in the editor, so that the user can
 * write his / her own scripts.
 *
 *
 */
class AUserComponent {
public:
    /**
     * @brief This function is called every frame. It is the main function
     * of the script.
     *
     * @warning This function is not generated automatically and must be
     * implemented by the user.
     * @param entityID
     */
    virtual void Update(int entityID) = 0;

    /**
     * @brief This function is called once before the first frame of the game.
     * It does not get called if the game has already started.
     *
     * @warning This function is not generated automatically and must be
     * implemented by the user.
     */
    virtual void Start() = 0;

    /**
     * @brief This function is called once, the first time that this class will
     * be loaded into the engine.
     *
     * @warning This function is not generated automatically and must be
     * implemented by the user.
     */
    virtual void OnLoad() {};

    /**
     * @brief This function is called everytime a component is added to an entity.
     *
     * @warning This function is not generated automatically and must be
     * implemented by the user.
     * @param entityID
     */
    virtual void OnAddComponent(int entityID) {};

    ///////////////////////////////////////////////////////////////////////
    // The following functions are generated automatically by the parser //
    ///////////////////////////////////////////////////////////////////////

    /**
     * @brief This function loads the data from the file at the given path.
     *
     * @param path
     */
    virtual void Load(const std::string& path) = 0;

    /**
     * @brief This function saves the data to the file at the given path.
     *
     * @param path
     */
    virtual void Save(const std::string& path) = 0;

    /**
     * @brief Returns a string representing the name of the class in a
     * human-readable format.
     *
     * @return std::string
     */
    virtual std::string GetClassName() const = 0;

    /**
     * @brief Returns the metadata of the class. The metadata contains
     * information about the members of the class. (see metadata.h for more
     * information)
     *
     * @return std::map<std::string, metadata_t>
     */
    virtual std::map<std::string, metadata_t> GetMetadata() = 0;
};

class UserComponentWrapper : public ecs::Component<UserComponentWrapper> {
public:
    /**
     * @brief Calls the update method of the internal component if it exists.
     *
     * @param entityID
     */
    void Update(int entityID);

    /**
     * @brief Calls the start method of the internal component if it exists.
     *
     */
    void Start();

    /**
     * @brief Calls the OnAddComponent method of the internal component if it exists.
     *
     */
    void OnAddComponent(int entityID);

    /**
     * @brief Calls the load method of the internal component if it exists.
     *
     * @param path
     */
    void Load(const std::string& path);

    /**
     * @brief This function does not save the object if the resource ID is empty.
     * This prevents unwanted saving of uninitialized objects.
     *
     * @param path
     */
    void Save(const std::string& path);

    /**
     * @brief Calls the OnLoad method of the internal component if it exists.
     *
     */
    void OnLoad();

    /**
     * @brief Returns the metadata of the internal component if it exists.
     *
     * @return std::map<std::string, metadata_t>
     */
    std::map<std::string, metadata_t> GetMetadata();

    /**
     * @brief Returns the name of the class instanciated by the internal component.
     *
     * @return std::string
     */
    std::string GetClassName() const;

    /**
     * @brief Sets the Internal Component object
     *
     * @param component
     */
    void SetInternalComponent(std::shared_ptr<AUserComponent>);

    /**
     * @brief Gets the Internal Component object
     *
     * @return AUserComponent*
     */
    std::shared_ptr<AUserComponent> GetInternalComponent() const;

    /**
     * @brief Returns the Internal component object much like GetInternalComponent,
     * but directly casted to the given type.
     *
     */
    template <typename T>
    inline std::shared_ptr<T> Get() const
    {
        return std::dynamic_pointer_cast<T>(m_internalComponent);
    }

    /**
     * @brief Sets the Resource ID object
     *
     * @param resourceID
     */
    void SetResourceID(const std::string& resourceID);

    /**
     * @brief Gets the Resource ID object
     *
     * @return std::string
     */
    std::string GetResourceID() const;

private:
    std::string resourceID = "";

    std::shared_ptr<AUserComponent> m_internalComponent = nullptr;
};
