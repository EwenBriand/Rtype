/*
** EPITECH PROJECT, 2023
** graph
** File description:
** Sprite.cpp
*/

#include <unistd.h>
#include <limits.h>

#include "VAO.hpp"
#include "VBO.hpp"
#include "Uniform.hpp"
#include "Shader.hpp"
#include "Sprite.hpp"
#include "ShaderBuildSystem.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace lg
{
    // std::shared_ptr<ShaderProgram> Sprite::SpriteShaderProgram = nullptr;
    bool Sprite::ShaderIsSetup = false;
    std::shared_ptr<Uniform<glm::mat4>> Sprite::SpriteTransformUniform = nullptr;

    Sprite::Sprite(const std::string &pathToImage, Rect rect) :
        _rect(rect)
    {
        // creating a VAO to store the VBO and EBO for quick access
        _vao = std::make_shared<VAO>();

        // a vbo to store the positions of the vertices
        _vboPositions = std::make_shared<VBO>(_vertices, 4 * 3 * sizeof(float));
        _vboPositions->SetVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);

        // a vbo to store the texture coordinates
        _vboTexCoords = std::make_shared<VBO>(_texCoords, 4 * 2 * sizeof(float));
        _vboTexCoords->SetVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *) 0);

        // an ebo to store the indices of the vertices that make triangles
        _ebo = std::make_shared<EBO>(_triangleIndices, sizeof(_triangleIndices));

        if (not ShaderIsSetup) {
            ShaderBuilder &builder = ShaderBuildSystem::Get()["spriteShader"];
            auto &vertexShader = builder.GetVertexShader();
            auto &fragmentShader = builder.GetFragmentShader();
            builder.AddSharedElement(LayoutCommon(
                "position", "vec3", builder.GetNewUsableLocation()));
            builder.AddSharedElement(LayoutCommon(
                "texCoord", "vec2", builder.GetNewUsableLocation()));

            vertexShader.Add(OutVarCommon("TexCoords", "vec2"));
            vertexShader.Add(UniformCommon("transformationMatrix", "mat4"));
            vertexShader.AddToMain(
                "gl_Position = transformationMatrix * vec4(position, 1.0);\n");
            vertexShader.AddToMain("TexCoords = texCoord;\n");

            fragmentShader.Add(InVarCommon("TexCoords", "vec2"));
            fragmentShader.Add(OutVarCommon("FragColor", "vec4"));
            fragmentShader.Add(UniformCommon("textureSampler", "sampler2D"));
            fragmentShader.AddToMain(
                "FragColor = texture(textureSampler, TexCoords);\n");
            ShaderIsSetup = true;
        }
        // creating a texture, it will be bound to the texture unit 0 in the shader
        _texture = std::make_shared<Texture>(pathToImage);

        _vao->Unbind();
        _vboPositions->Unbind();
        _vboTexCoords->Unbind();
        _ebo->Unbind();

    }

    Sprite::~Sprite()
    {
    }

    void Sprite::Draw()
    {
        // no call to SpriteShaderProgram->Use() because it is already done by the
        // << operator of the uniform below
        SpriteTransformUniform = std::make_shared<Uniform<glm::mat4>>("transformationMatrix", _transformMatrix, *ShaderBuildSystem::Get()["spriteShader"].Get());
        *SpriteTransformUniform << _transformMatrix;
        _texture->Bind();
        _vao->Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        int err = glGetError();
        if (err != GL_NO_ERROR)
            throw std::runtime_error("Failed to draw sprite: " + std::to_string(err));
        _vao->Unbind();
        _texture->Unbind();
    }

    void Sprite::SetPosition(const vec3f &position)
    {
        _transformMatrix = glm::translate(_transformMatrix, position - _position);
        _position = position;
    }

    void Sprite::SetScale(const vec3f &scale)
    {
        _transformMatrix = glm::scale(_transformMatrix, scale / _scale);
        _scale = scale;
    }

    void Sprite::SetRotation(const vec3f &rotation)
    {
        _transformMatrix = glm::rotate(_transformMatrix, rotation.x - _rotation.x, vec3f(1, 0, 0));
        _transformMatrix = glm::rotate(_transformMatrix, rotation.y - _rotation.y, vec3f(0, 1, 0));
        _transformMatrix = glm::rotate(_transformMatrix, rotation.z - _rotation.z, vec3f(0, 0, 1));
        _rotation = rotation;
    }

    void Sprite::SetRect(const Rect &rect)
    {
        _rect = rect;
    }

    vec3f Sprite::GetPosition() const
    {
        return _position;
    }

    vec3f Sprite::GetScale() const
    {
        return _scale;
    }

    vec3f Sprite::GetRotation() const
    {
        return _rotation;
    }
} // namespace lg
