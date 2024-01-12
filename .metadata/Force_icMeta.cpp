/*
* CAUTION: This file was generated automatically.
* Do not modify it, as your changes will be lost.
*/

#include <sstream>
#include "ToBytes.hpp"
#include <iostream>
#include "metadata.hpp"
#include "Force_ic.hpp"

void Force_ic::Load(const std::string &filepath)
{
	bytes::ToBytes bytes;
	bytes.Load(filepath, _speed);
}

void Force_ic::Save(const std::string &filepath)
{
	bytes::ToBytes bytes;
	bytes.Save(filepath, _speed);
}

std::map<std::string, metadata_t> Force_ic::GetMetadata()
{
	std::map<std::string, metadata_t> metadata;
	metadata["_speed"] = {"float", "_speed", sizeof(float), [&](const std::string &str) {_speed = fromString<decltype(_speed)>(str);}, [&]() { return &_speed; }, [&]() { std::ostringstream oss;oss << _speed;return oss.str();}};
	return metadata;
}
