/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** metadataGenerator.hpp
*/

#pragma once

#include <map>
#include <string>
#include <vector>

namespace meta {
    class MetadataGenerator {
    public:
        // std::map<std::string, std::string> typeConversionTable = {
        //     { "int", "libconfig::Setting::TypeInt" },
        //     { "float", "libconfig::Setting::TypeFloat" },
        //     { "float", "libconfig::Setting::TypeFloat" },
        //     { "double", "libconfig::Setting::TypeFloat" },
        //     { "std::string", "libconfig::Setting::TypeString" },
        //     { "char", "libconfig::Setting::TypeInt" },
        //     { "bool", "libconfig::Setting::TypeBoolean" },
        //     { "std::vector<int>", "libconfig::Setting::TypeArray" },
        //     { "std::vector<float>", "libconfig::Setting::TypeArray" },
        //     { "std::vector<double>", "libconfig::Setting::TypeArray" },
        //     { "std::vector<std::string>", "libconfig::Setting::TypeArray" },
        //     { "std::vector<char>", "libconfig::Setting::TypeArray" },
        //     { "std::vector<bool>", "libconfig::Setting::TypeArray" },
        // };
        MetadataGenerator();
        ~MetadataGenerator();
        void generateMetadata(const std::string& path, const std::string& outputDirectory = "./metabuild");
        void buildCMake();

    private:
        void generateMetadataForFile(const std::string& filepath, const std::string& filename);
        void generateMetadataForClass(
            const std::vector<std::string>& words,
            size_t& i,
            const std::string& filename);
        std::vector<std::string> tokenize(std::ifstream& file);
        void initFunctions(std::string& loadFunction, std::string& saveFunction, std::string& getMetadataFunction, const std::string& className);
        void updateMetadataFunctions(const std::vector<std::string>& words, size_t& i, std::string& loadFunction, std::string& saveFunction, std::string& getMetadataFunction);
        void writeDisclaimer(std::ofstream& file);
        void initCMake();
        void cmakeAppendShared(const std::string& filename, const std::string& filepath, std::vector<std::string>& words);
        void saveCMake();
        void detectDependencies(std::vector<std::string>& words, const std::string& targetname);
        bool wordBeginsWith(const std::string& word, const std::string& beginning);
        void updateMetadataVector(const std::vector<std::string>& words, size_t& i,
            std::string& loadFunction, std::string& saveFunction, std::string& getMetadataFunction);
        void updateMetadataMap(const std::vector<std::string>& words, size_t& i,
            std::string& loadFunction, std::string& saveFunction, std::string& getMetadataFunction);

        std::string folder = "";
        std::string _className = "";
        std::string _filename = "";
        std::string _cmakefile = "";
        std::string _outputDirectory = "./metabuild";
    };
}
