/*
** EPITECH PROJECT, 2023
** safe
** File description:
** RLCube.hpp
*/

#include "Components.Vanilla/UserComponentWrapper.hpp"
#include "IGraphicalModule.hpp"
#include "metadata.hpp"
#include <raylib.h>

/**
 * @brief A Component that draws a 3D cube on the screen.
 *
 */
serialize class RLCube : public AUserComponent {
public:
    GENERATE_METADATA(RLCube)

    ~RLCube();

    void Update(int e) override;
    void OnAddComponent(int e) override;
    void Start() override;
    void OnLoad() override;

private:
    void updateMeshFromTransform();
    void loadTexture();
    BoundingBox _boundingBox;
    Model _mesh;
    serialize std::string _texturePath;
    bool _textureIsLoaded = false;
    Texture2D _texture;
};