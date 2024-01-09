/*
* CAUTION: This file was generated automatically.
* Do not modify it, as your changes will be lost.
*/

#include <sstream>
#include "ToBytes.hpp"
#include <iostream>
#include "metadata.hpp"
#include "LuaScript.hpp"

void LuaScript::Load(const std::string &filepath)
{
	bytes::ToBytes bytes;
	bytes.Load(filepath, _scriptPath);
}

void LuaScript::Save(const std::string &filepath)
{
	bytes::ToBytes bytes;
	bytes.Save(filepath, _scriptPath);
}

std::map<std::string, metadata_t> LuaScript::GetMetadata()
{
	std::map<std::string, metadata_t> metadata;
	metadata["_scriptPath"] = {"std::string", "_scriptPath", sizeof(std::string), [&](const std::string &str) {_scriptPath = fromString<decltype(_scriptPath)>(str);}, [&]() { return &_scriptPath; }, [&]() { std::ostringstream oss;oss << _scriptPath;return oss.str();}};
	return metadata;
}
