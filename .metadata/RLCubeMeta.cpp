/*
* CAUTION: This file was generated automatically.
* Do not modify it, as your changes will be lost.
*/

#include <sstream>
#include "ToBytes.hpp"
#include <iostream>
#include "metadata.hpp"
#include "RLCube.hpp"

void RLCube::Load(const std::string &filepath)
{
	bytes::ToBytes bytes;
	bytes.Load(filepath, _texturePath);
}

void RLCube::Save(const std::string &filepath)
{
	bytes::ToBytes bytes;
	bytes.Save(filepath, _texturePath);
}

std::map<std::string, metadata_t> RLCube::GetMetadata()
{
	std::map<std::string, metadata_t> metadata;
	metadata["_texturePath"] = {"std::string", "_texturePath", sizeof(std::string), [&](const std::string &str) {_texturePath = fromString<decltype(_texturePath)>(str);}, [&]() { return &_texturePath; }, [&]() { std::ostringstream oss;oss << _texturePath;return oss.str();}};
	return metadata;
}
