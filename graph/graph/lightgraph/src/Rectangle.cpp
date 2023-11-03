/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** Rectangle.cpp
*/

#include "Uniform.hpp"
#include "Rectangle.hpp"
#include "ShaderBuildSystem.hpp"

namespace lg {
    bool Rectangle::ShaderIsSetup = false;
    unsigned int Rectangle::Indices[6] = {
        0, 1, 3,
        1, 2, 3,
    };

    Rectangle::Rectangle(vec2f pos, vec2f size, vec4f color, float borderSize, vec4f borderColor) :
        _color(color)
    {
        if (!ShaderIsSetup) {
            std::cout << "Setting up rectangle shader" << std::endl;
            ShaderIsSetup = true;

            auto &rectangleShaderBuilder = ShaderBuildSystem::Get()["rectangleShader"];
            auto &vertex = rectangleShaderBuilder.GetVertexShader();
            auto &frag = rectangleShaderBuilder.GetFragmentShader();

            vertex.Add(UniformCommon("model", "mat4"));
            vertex.Add(LayoutCommon("aPos", "vec3", 0));
            vertex.AddToMain(
                "gl_Position = vec4(aPos, 1);\n"
            );
            vertex.AddToMain(
                "gl_Position = model * gl_Position;\n", 1000
            );

            frag.Add(UniformCommon("color", "vec4"));
            frag.Add(OutVarCommon("FragColor", "vec4"));
            frag.AddToMain(
                "FragColor = color;\n", 1001
            );
        }

        _vertices[0] = pos.x;
        _vertices[1] = pos.y;
        _vertices[2] = 0.0f;

        _vertices[3] = pos.x + size.x;
        _vertices[4] = pos.y;
        _vertices[5] = 0.0f;

        _vertices[6] = pos.x + size.x;
        _vertices[7] = pos.y + size.y;
        _vertices[8] = 0.0f;

        _vertices[9] = pos.x;
        _vertices[10] = pos.y + size.y;
        _vertices[11] = 0.0f;

        _vao = std::make_shared<VAO>();
        _vbo = std::make_shared<VBO>(_vertices, sizeof(_vertices));
        _ebo = std::make_shared<EBO>(Rectangle::Indices, sizeof(Rectangle::Indices));
    }

    void Rectangle::Draw()
    {
        auto rectangleShader = *ShaderBuildSystem::Get()["rectangleShader"].Get();
        rectangleShader.Use();
        _vao->Bind();
        rectangleShader.AssignVBO(*_vbo);
        _ebo->Bind();

        Uniform<glm::mat4> model("model", _rotation, rectangleShader);
        Uniform<glm::vec4> color("color", _color, rectangleShader);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        _vao->Unbind();
    }

    void Rectangle::SetRotation(vec3f rotation)
    {
        _rotation = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), glm::vec3(1, 0, 0));
        _rotation = glm::rotate(_rotation, glm::radians(rotation.y), glm::vec3(0, 1, 0));
        _rotation = glm::rotate(_rotation, glm::radians(rotation.z), glm::vec3(0, 0, 1));
    }

    void Rectangle::SetColor(vec4f color)
    {
        _color = color;
    }
}
