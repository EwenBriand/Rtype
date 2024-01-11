/*
* CAUTION: This file was generated automatically.
* Do not modify it, as your changes will be lost.
*/

#include <sstream>
#include "ToBytes.hpp"
#include <iostream>
#include "metadata.hpp"
#include "Tag.hpp"

void Tag::Load(const std::string &filepath)
{
	bytes::ToBytes bytes;
	bytes.Load(filepath, name);
}

void Tag::Save(const std::string &filepath)
{
	bytes::ToBytes bytes;
	bytes.Save(filepath, name);
}

std::map<std::string, metadata_t> Tag::GetMetadata()
{
	std::map<std::string, metadata_t> metadata;
	metadata["name"] = {"std::string", "name", sizeof(std::string), [&](const std::string &str) {name = fromString<decltype(name)>(str);}, [&]() { return &name; }, [&]() { std::ostringstream oss;oss << name;return oss.str();}};
	return metadata;
}
