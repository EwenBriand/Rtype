/*
** EPITECH PROJECT, 2023
** graph
** File description:
** ShaderBuilderPartial.cpp
*/

#include <sstream>
#include <algorithm>
#include "glm/fwd.hpp"
#include "../include/ShaderBuilder.hpp"

namespace lg {
    void ShaderBuilderPartial::SetVersion(unsigned int version)
    {
        _version = version;
    }

    template <> void ShaderBuilderPartial::Add<LayoutCommon>(const LayoutCommon &layout)
    {
        _layouts.push_back(layout);
    }

    template <> void ShaderBuilderPartial::Add<UniformCommon>(const UniformCommon &uniform)
    {
        _uniforms.push_back(uniform);
    }

    template <> void ShaderBuilderPartial::Add<InVarCommon>(const InVarCommon &in)
    {
        _inVars.push_back(in);
    }

    template <> void ShaderBuilderPartial::Add<OutVarCommon>(const OutVarCommon &out)
    {
        _outVars.push_back(out);
    }

    template <> void ShaderBuilderPartial::Add<FunctionCommon>(const FunctionCommon &function)
    {
        _functions.push_back(function);
    }

    void ShaderBuilderPartial::AddToMain(const std::string &str, int priority)
    {
        _mainBody.emplace_back(priority, "\t" + str);
    }

    std::string ShaderBuilderPartial::GetSrc()
    {
        if (_src != "")
            return _src;
        std::stringstream stream;
        stream << "#version " << _version << " core\n";

        for (auto &layout : _layouts)
            layout >> stream;

        for (auto &uniform : _uniforms)
            uniform >> stream;

        for (auto &inVar : _inVars)
            inVar >> stream;

        for (auto &outVar : _outVars)
            outVar >> stream;

        for (auto &function : _functions)
            function >> stream;

        std::sort(_mainBody.begin(), _mainBody.end(), [](auto &a, auto &b) {
            return std::get<0>(a) < std::get<0>(b);
        });

        stream << "void main()\n{\n";

        for (auto &main : _mainBody)
            stream << std::get<1>(main);
        stream << "}\n";

        _src = stream.str();
        return _src;
    }

    ShaderBuilderPartial &ShaderBuilder::GetVertexShader()
    {
        return _vertexShader;
    }

    ShaderBuilderPartial &ShaderBuilder::GetFragmentShader()
    {
        return _fragmentShader;
    }

    std::shared_ptr<ShaderProgram> ShaderBuilder::Build()
    {
        if (_shaderProgram != nullptr)
            _shaderProgram = nullptr;
        _shaderProgram = std::make_shared<ShaderProgram>("ShaderBuilder_" + std::to_string(NInstances++));
        try {
            _shaderProgram->AddShader(_vertexShader.GetSrc(), 0, GL_VERTEX_SHADER, false);
            _shaderProgram->AddShader(_fragmentShader.GetSrc(), 1, GL_FRAGMENT_SHADER, false);
            _shaderProgram->Link();
        } catch (std::exception &e) {
            throw std::runtime_error("ShaderBuilder Error: \n\t" + std::string(e.what())
                + "\n\t" + _vertexShader.GetSrc() + "\n\t" + _fragmentShader.GetSrc());
        }
        return _shaderProgram;
    }

    void ShaderBuilder::Use() const
    {
        if (_shaderProgram == nullptr)
            throw std::runtime_error("ShaderBuilder Error: \n\tShader not built");
        _shaderProgram->Use();
    }

    std::shared_ptr<ShaderProgram> ShaderBuilder::Get() const
    {
        if (_shaderProgram == nullptr)
            throw std::runtime_error("ShaderBuilder Error: \n\tShader not built");
        return _shaderProgram;
    }

    unsigned int ShaderBuilder::GetNewUsableLocation()
    {
        return CurrentLocation++;
    }
}
