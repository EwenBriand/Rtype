/*
* CAUTION: This file was generated automatically.
* Do not modify it, as your changes will be lost.
*/

#include <sstream>
#include "ToBytes.hpp"
#include <iostream>
#include "metadata.hpp"
#include "SphereCollider3D.hpp"

void SphereCollider3D::Load(const std::string &filepath)
{
	bytes::ToBytes bytes;
	bytes.Load(filepath, _radius, _isTrigger, _tag);
}

void SphereCollider3D::Save(const std::string &filepath)
{
	bytes::ToBytes bytes;
	bytes.Save(filepath, _radius, _isTrigger, _tag);
}

std::map<std::string, metadata_t> SphereCollider3D::GetMetadata()
{
	std::map<std::string, metadata_t> metadata;
	metadata["_radius"] = {"float", "_radius", sizeof(float), [&](const std::string &str) {_radius = fromString<decltype(_radius)>(str);}, [&]() { return &_radius; }, [&]() { std::ostringstream oss;oss << _radius;return oss.str();}};
	metadata["_isTrigger"] = {"bool", "_isTrigger", sizeof(bool), [&](const std::string &str) {_isTrigger = fromString<decltype(_isTrigger)>(str);}, [&]() { return &_isTrigger; }, [&]() { std::ostringstream oss;oss << _isTrigger;return oss.str();}};
	metadata["_tag"] = {"std::string", "_tag", sizeof(std::string), [&](const std::string &str) {_tag = fromString<decltype(_tag)>(str);}, [&]() { return &_tag; }, [&]() { std::ostringstream oss;oss << _tag;return oss.str();}};
	return metadata;
}
