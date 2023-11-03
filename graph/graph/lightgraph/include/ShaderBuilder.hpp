/*
** EPITECH PROJECT, 2023
** graph
** File description:
** ShaderBuilderPartial.hpp
*/

#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <tuple>
#include "Shader.hpp"

namespace lg {
    struct LayoutCommon {
        const std::string Name;
        const std::string Type;
        const unsigned int Location;

        LayoutCommon(std::string name, std::string type, unsigned int location = 0)
            : Name(name), Type(type), Location(location)
        {
        }

        std::string toStr()
        {
            return "layout(location = " + std::to_string(Location) + ") in " + Type + " " + Name + ";\n";
        }

        template <typename Stream>
        Stream &operator>>(Stream &stream)
        {
            stream << toStr();
            return stream;
        }
    };

    struct UniformCommon {
        const std::string Name;
        const std::string Type;

        UniformCommon(std::string name, std::string type)
            : Name(name), Type(type)
        {
        }

        std::string ToStr() {
            return "uniform " + Type + " " + Name + ";\n";
        }

        template <typename Stream>
        Stream &operator>>(Stream &stream)
        {
            stream << ToStr();
            return stream;
        }
    };

    struct InVarCommon {
        const std::string Name;
        const std::string Type;

        InVarCommon(std::string name, std::string type)
            : Name(name), Type(type)
        {
        }

        std::string ToStr() {
            return "in " + Type + " " + Name + ";\n";
        }

        template <typename Stream>
        Stream &operator>>(Stream &stream)
        {
            stream << ToStr();
            return stream;
        }
    };

    struct OutVarCommon {
        const std::string Name;
        const std::string Type;

        OutVarCommon(std::string name, std::string type)
            : Name(name), Type(type)
        {
        }

        std::string ToStr() {
            return "out " + Type + " " + Name + ";\n";
        }

        template <typename Stream>
        Stream &operator>>(Stream &stream)
        {
            stream << ToStr();
            return stream;
        }
    };

    struct FunctionCommon {
        const std::string Body;
        const std::string Name;
        const std::string ReturnType;

        FunctionCommon(std::string Body, std::string Name, std::string ReturnType)
            : Body(Body), Name(Name), ReturnType(ReturnType)
        {
        }

        std::string ToStr() {
            return Body + "\n";
        }

        template <typename Stream>
        Stream &operator>>(Stream &stream)
        {
            stream << ToStr();
            return stream;
        }

        const std::string &GetName() const
        {
            return Name;
        }

        const std::string &GetReturnType() const
        {
            return ReturnType;
        }

        const std::string &GetBody() const
        {
            return Body;
        }
    };
    class ShaderBuilderPartial {
        public:

            void SetVersion(unsigned int version);

            template <typename CommonType>
            void Add(const CommonType &common)
            {
                throw std::runtime_error("Unknown common type: " + std::string(typeid(CommonType).name()) + "");
            }

            void AddToMain(const std::string &str, int priority = 0);

            std::string GetSrc();
        private:
            int _version = 420;
            std::vector<LayoutCommon> _layouts;
            std::vector<UniformCommon> _uniforms;
            std::vector<InVarCommon> _inVars;
            std::vector<OutVarCommon> _outVars;

            std::vector<FunctionCommon> _functions;
            std::vector<std::tuple<int, std::string>> _mainBody;

            std::shared_ptr<ShaderProgram> _shaderProgram = nullptr;
            std::string _src = "";
    };

    class ShaderBuilder {
        public:
            unsigned int CurrentLocation = 0;

            unsigned int GetNewUsableLocation();
            ShaderBuilderPartial &GetVertexShader();
            ShaderBuilderPartial &GetFragmentShader();
            std::shared_ptr<ShaderProgram> Build();
            void Use() const;
            std::shared_ptr<ShaderProgram> Get() const;

            template <typename CommonType>
            void AddSharedElement(const CommonType &common)
            {
                _vertexShader.Add(common);
                _fragmentShader.Add(common);
            }

        private:
            unsigned int NInstances = 0;
            ShaderBuilderPartial _vertexShader;
            ShaderBuilderPartial _fragmentShader;
            std::shared_ptr<ShaderProgram> _shaderProgram = nullptr;
    };
}
