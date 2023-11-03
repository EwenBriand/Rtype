/*
** EPITECH PROJECT, 2023
** graph
** File description:
** Shader.cpp
*/

#include <filesystem>
#include <fstream>
#include <algorithm>
#include "Shader.hpp"

namespace lg {
    Shader::Shader(const std::string &pathToSrc, int priority, GLenum type, bool fromFile) :
        _priority(priority),
        _type(type)
    {
        if (fromFile)
            _source = loadFile(pathToSrc);
        else
            _source = pathToSrc;
        compile();
    }

    Shader::~Shader()
    {
        glDeleteShader(_id);
    }

    std::string Shader::loadFile(const std::string &path)
    {
        if (path.size() > 0 && path[0] != '/')
            return loadFile(std::filesystem::current_path().string() + "/" + path);
        if (!std::filesystem::exists(path))
            throw std::runtime_error("Shader file not found: " + path);
        std::ifstream file(path);
        std::string line;
        std::string source;

        while (std::getline(file, line))
            source += line + "\n";
        return source;
    }

    void Shader::compile()
    {
        const char *src = _source.c_str();
        _id = glCreateShader(_type);
        glShaderSource(_id, 1, &src, nullptr);
        glCompileShader(_id);
        int success;
        char infoLog[512];
        glGetShaderiv(_id, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(_id, 512, nullptr, infoLog);
            throw std::runtime_error("Shader compilation failed: " + std::string(infoLog));
        }
        std::cout << "Successfully compiled shader " << _id << std::endl;
    }

    ShaderProgram::ShaderProgram(const std::string &name) :
        _name(name)
    {
        _id = glCreateProgram();
    }

    ShaderProgram::~ShaderProgram()
    {
        glDeleteProgram(_id);
    }

    void ShaderProgram::AddShader(const std::string &pathToSrc, int priority, GLenum type, bool fromFile)
    {
        _shaders.emplace_back(priority, std::make_shared<Shader>(pathToSrc, priority, type, fromFile));
    }

    void ShaderProgram::AddShader(std::shared_ptr<Shader> shader, int priority)
    {
        _shaders.emplace_back(priority, shader);
    }

    void ShaderProgram::Link(bool showLog)
    {
        std::sort(_shaders.begin(), _shaders.end(), [](auto &a, auto &b) {
            return std::get<0>(a) > std::get<0>(b);
        });

        int i = 0;
        for (auto &shader : _shaders) {
            std::cout << "Attaching shader " << std::get<1>(shader)->getId() << " (" << ++i << "/" << _shaders.size() << ")" << std::endl;
            glAttachShader(_id, std::get<1>(shader)->getId());
        }
        std::cout << "Linking program " << _id << "..." << std::endl;
        glLinkProgram(_id);

        int success;
        char infoLog[512] = {0};
        glGetProgramiv(_id, GL_LINK_STATUS, &success);

        if (!success) {
            if (showLog)
                glGetProgramInfoLog(_id, 512, nullptr, infoLog);
            throw std::runtime_error("Shader program linking failed: " + std::string(infoLog));
        }
        std::cout << "Successfully linked program " << _name << std::endl;
        for (auto &shader : _shaders)
            glDetachShader(_id, std::get<1>(shader)->getId());
    }

    void ShaderProgram::Use()
    {
        glUseProgram(_id);
    }

    void ShaderProgram::AssignVBO(
        VBO &vbo,
        int startIdx,
        int size,
        int stride,
        GLenum type,
        bool normalized,
        void *offset
    )
    {
        vbo.Bind();
        glVertexAttribPointer(startIdx, size, type, normalized, stride, offset);
        glEnableVertexAttribArray(0);
    }

    bool ShaderProgram::IsUsed() const
    {
        int id;
        glGetIntegerv(GL_CURRENT_PROGRAM, &id);
        return id == _id;
    }
}
