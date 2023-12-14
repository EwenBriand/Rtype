/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** metadataGenerator.cpp
*/

#include "metadataGenerator.hpp"
#include "metadata.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <unistd.h>
#include <vector>

namespace meta {

    MetadataGenerator::MetadataGenerator()
    {
    }

    MetadataGenerator::~MetadataGenerator()
    {
    }

    std::string MetadataGenerator::MakePath(const std::vector<std::string>& pathElements, bool throwIfNotExist)
    {
        std::filesystem::path p;
        for (const auto& element : pathElements) {
            p /= element;
        }
        p = std::filesystem::canonical(p);
        if (throwIfNotExist && !std::filesystem::exists(p)) {
            throw std::runtime_error("Path " + p.string() + " does not exist");
        }
        return p.string();
    }

    void MetadataGenerator::generateMetadata(const std::string& path, const std::string& outputDirectory, const std::string& buildRoot)
    {
        _buildRoot = buildRoot;
        _outputDirectory = outputDirectory;
        if (!std::filesystem::exists("./.metadata"))
            std::filesystem::create_directory("./.metadata");
        char buff[1024] = { 0 };
        getcwd(buff, 1024);
        std::cout << "-- Working from directory " << buff << std::endl;

        if (folder == "")
            folder = path;
        initCMake();
        for (const auto& entry : std::filesystem::directory_iterator(path)) {
            if (entry.is_directory() && entry.path().filename() != ".metadata")
                generateMetadata(entry.path().string());
            else {
                _filename = entry.path().filename().string();
                if (entry.path().extension() == ".hpp")
                    generateMetadataForFile(entry.path().string(), entry.path().filename().string());
            }
        }
        saveCMake();
    }

    std::vector<std::string> MetadataGenerator::tokenize(std::ifstream& file)
    {
        std::vector<std::string> words;
        std::string line;

        while (std::getline(file, line)) {
            std::string word;
            for (size_t i = 0; i < line.size(); ++i) {
                if (line[i] == ' ' || line[i] == '\t' || line[i] == '\n') {
                    if (word.size() > 0)
                        words.push_back(word);
                    word.clear();
                } else
                    word += line[i];
            }
            if (word.size() > 0)
                words.push_back(word);
        }
        for (size_t i = 0; i < words.size(); ++i) {
            if (words[i].size() > 0 && words[i][words[i].size() - 1] == ';')
                words[i] = words[i].substr(0, words[i].size() - 1);
        }
        return words;
    }

    void MetadataGenerator::generateMetadataForFile(const std::string& filepath,
        const std::string& filename)
    {
        std::ifstream file(filepath);
        std::vector<std::string> words = tokenize(file);
        bool serializeed = false;
        std::cout << "-- Generating metadata for " << filename << std::endl;

        for (size_t i = 0; i < words.size(); ++i) {
            if (words[i] == "class" && i > 0 && words[i - 1] == "serialize") {
                serializeed = true;
                std::string filenameNoExtension = filename.substr(0, filename.find_last_of("."));
                generateMetadataForClass(words, i, "./.metadata/" + filenameNoExtension + "Meta" + ".cpp");
            }
        }
        if (serializeed)
            cmakeAppendShared(filename, filepath, words);
        file.close();
    }

    void MetadataGenerator::generateMetadataForClass(
        const std::vector<std::string>& words,
        size_t& i,
        const std::string& filename)
    {
        std::ofstream metadataFile(filename, std::ios::out);
        if (!metadataFile.is_open()) {
            std::cerr << "Error: could not open file " << filename << std::endl;
            return;
        }
        bool nonTrivialEnd = false;
        int openBrackets = 0;
        std::string loadFunction = "";
        std::string saveFunction = "";
        std::string getMetadataFunction = "";
        std::string className = words[++i];
        _className = className;
        bool needsInit = true;

        for (; i < words.size() && (!nonTrivialEnd || openBrackets > 0); ++i) {
            if (words[i] == "{" && needsInit) {
                ++openBrackets;
                nonTrivialEnd = true;
                needsInit = false;
                initFunctions(loadFunction, saveFunction, getMetadataFunction, className);

            } else if (words[i] == "}")
                --openBrackets;
            if (words[i] == "serialize") {
                updateMetadataFunctions(words, i, loadFunction, saveFunction, getMetadataFunction);
            }
        }
        loadFunction += ");\n}\n";
        saveFunction += ");\n}\n";
        getMetadataFunction += "\treturn metadata;\n";
        getMetadataFunction += "}\n";
        writeDisclaimer(metadataFile);
        metadataFile << "#include <iostream>\n";
        metadataFile << "#include \"metadata.hpp\"\n";
        metadataFile << "#include \"" + _filename + "\"\n";
        metadataFile << "\n";
        metadataFile << loadFunction;
        metadataFile << "\n";
        metadataFile << saveFunction;
        metadataFile << "\n";
        metadataFile << getMetadataFunction;
        metadataFile.close();
    }

    void MetadataGenerator::writeDisclaimer(std::ofstream& file)
    {
        file << "/*\n";
        file << "* CAUTION: This file was generated automatically.\n";
        file << "* Do not modify it, as your changes will be lost.\n";
        file << "*/\n\n";
        file << "#include <sstream>\n";
        file << "#include \"ToBytes.hpp\"\n";
    }

    void MetadataGenerator::initFunctions(std::string& loadFunction,
        std::string& saveFunction,
        std::string& getMetadataFunction,
        const std::string& className)
    {

        loadFunction = "void " + className + "::Load(const std::string &filepath)\n{\n";
        loadFunction += "\tbytes::ToBytes bytes;\n";
        loadFunction += "\tbytes.Load(filepath";

        saveFunction = "void " + className + "::Save(const std::string &filepath)\n{\n";
        saveFunction += "\tbytes::ToBytes bytes;\n";
        saveFunction += "\tbytes.Save(filepath";

        getMetadataFunction = "std::map<std::string, metadata_t> " + className + "::GetMetadata()\n{\n";
        getMetadataFunction += "\tstd::map<std::string, metadata_t> metadata;\n";
    }

    bool MetadataGenerator::wordBeginsWith(const std::string& word, const std::string& beginning)
    {
        if (word.length() < beginning.length())
            return false;
        if (word.substr(0, beginning.length()) == beginning)
            return true;
        return false;
    }

    void MetadataGenerator::updateMetadataVector(const std::vector<std::string>& words,
        size_t& i,
        std::string& loadFunction,
        std::string& saveFunction,
        std::string& getMetadataFunction)
    {
        std::string type;
        std::string name;
        std::string offset;
        std::string size;
        std::string contained_type;

        type = words[i];
        contained_type = words[i].substr(std::string("std::vector<").length(), words[i].find_last_of(">") - std::string("std::vector<").length());
        name = words[++i];
        size = "sizeof(" + type + ")";

        loadFunction += ", " + name;
        saveFunction += ", " + name;
        getMetadataFunction += "\tmetadata[\"" + name + "\"] = {\"" + type + "\", \"" + name + "\", " + size + ", " + TEMPLATE_GENERATE_SETTER_VECTOR(name) + ", " + TEMPLATE_GENERATE_GETTER_VECTOR(name) + ", " + TEMPLATE_GENERATE_TO_STRING_VECTOR(name) + "};\n";
    }

    void MetadataGenerator::updateMetadataMap(
        const std::vector<std::string>& words,
        size_t& i,
        std::string& loadFunction,
        std::string& saveFunction,
        std::string& getMetadataFunction)
    {
        std::string type;
        std::string name;
        std::string offset;
        std::string size;
        type = words[i];
        name = words[++i];
        size = "sizeof(" + type + ")";

        loadFunction += ", " + name;
        saveFunction += ", " + name;
        getMetadataFunction += "\tmetadata[\"" + name + "\"] = {\"" + type + "\", \"" + name + "\", " + size + ", " + TEMPLATE_GENERATE_SETTER_MAP(name) + ", " + TEMPLATE_GENERATE_GETTER_MAP(name) + ", " + TEMPLATE_GENERATE_TO_STRING_MAP(name) + "};\n";
    }

    void MetadataGenerator::updateMetadataFunctions(
        const std::vector<std::string>& words,
        size_t& i,
        std::string& loadFunction,
        std::string& saveFunction,
        std::string& getMetadataFunction)
    {
        std::string type;
        std::string name;
        std::string offset;
        std::string size;

        ++i;
        if (wordBeginsWith(words[i], "std::vector")) {
            updateMetadataVector(words, i, loadFunction, saveFunction, getMetadataFunction);
            return;
        }
        if (wordBeginsWith(words[i], "std::map")) {
            updateMetadataMap(words, i, loadFunction, saveFunction, getMetadataFunction);
            return;
        }
        type = words[i];
        name = words[++i];
        size = "sizeof(" + type + ")";
        loadFunction += ", " + name;
        saveFunction += ", " + name;

        getMetadataFunction += "\tmetadata[\"" + name + "\"] = {\"" + type + "\", \"" + name + "\", " + size + ", " + GENERATE_SETTER(name) + ", " + GENERATE_GETTER(name) + ", " + GENERATE_TO_STRING(name) + "};\n";
    }

    void MetadataGenerator::initCMake()
    {
        _cmakefile += "cmake_minimum_required(VERSION 3.5)\n";
        _cmakefile += "project(autogen)\n";
        _cmakefile += "\n";
        _cmakefile += "set(CMAKE_CXX_STANDARD 20)\n";
        _cmakefile += "include_directories(" + folder + " " + folder + " " + _buildRoot + "/metadata/include/ " + _buildRoot + "/ecs/include/ " + _buildRoot + "/ecs/raylib/src/ " + _buildRoot + "/ecs/raylib/examples/shapes/ )\n";
        _cmakefile += "set(CMAKE_RUNTIME_OUTPUT_DIRECTORY " + folder + ")\n";
        _cmakefile += "set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH}  ${CMAKE_CURRENT_SOURCE_DIR}/../build/)\n";
        _cmakefile += "link_directories(\n";
        _cmakefile += ")\n";
    }

    void MetadataGenerator::cmakeAppendShared(const std::string& filename, const std::string& filepath,
        std::vector<std::string>& words)
    {
        std::string filenameNoExtension = filename.substr(0, filename.find_last_of("."));
        std::string metadataFilename = "./.metadata/" + filenameNoExtension + "Meta" + ".cpp";
        _cmakefile += "add_library(" + filenameNoExtension + " SHARED " + filepath + " ../" + metadataFilename;
        std::string cppFilePath = filepath.substr(0, filepath.find_last_of(".")) + ".cpp";
        if (std::filesystem::exists(cppFilePath)) {
            _cmakefile += " " + folder + "/" + filenameNoExtension + ".cpp";
        }
        _cmakefile += ")\n";
        detectDependencies(words, filenameNoExtension);
    }

    void MetadataGenerator::saveCMake()
    {
        remove(_outputDirectory.c_str());
        std::filesystem::create_directory(_outputDirectory);

        _cmakefile += "add_compile_options(-g3)";
        std::ofstream cmakefile("./" + _outputDirectory + "/CMakeLists.txt", std::ios::out);

        if (!cmakefile.is_open()) {
            std::cerr << "Error: could not open file CMakeLists.txt" << std::endl;
            return;
        }
        cmakefile << _cmakefile;
        cmakefile.close();
    }

    void MetadataGenerator::buildCMake()
    {
        std::string command = "cd " + _outputDirectory + " && cmake . && make -j 8";
        int out = system(command.c_str());
        if (out != 0) {
            std::cerr << "Error: could not build metadata" << std::endl;
            throw std::exception();
        }
    }

    void MetadataGenerator::detectDependencies(std::vector<std::string>& words,
        const std::string& targetName)
    {
        std::string dependencies = "";
        for (size_t i = 0; i < words.size(); ++i) {
            if (words[i].substr(0, 8) == "needs") {
                std::string libname = words[i].substr(9, words[i].size() - 10);
                ++i;
                dependencies += libname + " ";
            }
        }
        if (dependencies != "") {
            _cmakefile += "target_link_libraries(" + targetName + " " + dependencies + ")\n";
        }
    }
}
