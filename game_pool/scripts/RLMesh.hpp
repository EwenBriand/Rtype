/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** RLMesh3D.hpp
*/

#pragma once

#include "Component.hpp"
#include "Components.Vanilla/UserComponentWrapper.hpp"
#include "IGraphicalModule.hpp"
#include "metadata.hpp"
#include <atomic>
#include <future>
#include <iostream>
#include <raylib.h>
#include <string>

/**
 * A class to encapsulate raylib meshes. Loads a .obj file.
 */
serialize class RLMesh : public AUserComponent {
public:
    GENERATE_METADATA(RLMesh)

    ~RLMesh();

    void Update(int e) override;
    void OnAddComponent(int e) override;
    void Start() override;
    void OnLoad() override;

private:
    /**
     * @brief Attempts to load the .obj file at m_path
     *
     */
    void internalLoadObject();

    /**
     * @brief Loads the model asynchronously
     *
     */
    void loadModelAsync();
    bool doLoad();

    /**@brief
     *
     *
     * @return true
     * @return false
     */
    bool internalLoadTexture(const std::string& assetRoot);

    /**
     * @brief Updates the position of the model according to the transform component
     * of the entity if it has one.
     *
     */
    void updatePosition(int entityID);

    /**
     * @brief Path to the object file.
     *
     */
    serialize std::string m_path = "";
    serialize std::string m_texturePath = "";

    // bool m_modelIsLoaded = false;
    std::atomic_bool m_modelIsLoaded = false;
    bool m_textureIsLoaded = false;
    bool m_loadingStarted = false;
    Model m_model;
    std::future<void> m_modelLoader;
    BoundingBox m_boundingBox;
    Texture2D m_texture;
};
