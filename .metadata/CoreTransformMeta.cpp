/*
* CAUTION: This file was generated automatically.
* Do not modify it, as your changes will be lost.
*/

#include <sstream>
#include "ToBytes.hpp"
#include <iostream>
#include "metadata.hpp"
#include "CoreTransform.hpp"

void CoreTransform::Load(const std::string &filepath)
{
	bytes::ToBytes bytes;
	bytes.Load(filepath, x, y, z, rotationX, rotationY, rotationZ, scaleX, scaleY, scaleZ);
}

void CoreTransform::Save(const std::string &filepath)
{
	bytes::ToBytes bytes;
	bytes.Save(filepath, x, y, z, rotationX, rotationY, rotationZ, scaleX, scaleY, scaleZ);
}

std::map<std::string, metadata_t> CoreTransform::GetMetadata()
{
	std::map<std::string, metadata_t> metadata;
	metadata["x"] = {"float", "x", sizeof(float), [&](const std::string &str) {x = fromString<decltype(x)>(str);}, [&]() { return &x; }, [&]() { std::ostringstream oss;oss << x;return oss.str();}};
	metadata["y"] = {"float", "y", sizeof(float), [&](const std::string &str) {y = fromString<decltype(y)>(str);}, [&]() { return &y; }, [&]() { std::ostringstream oss;oss << y;return oss.str();}};
	metadata["z"] = {"float", "z", sizeof(float), [&](const std::string &str) {z = fromString<decltype(z)>(str);}, [&]() { return &z; }, [&]() { std::ostringstream oss;oss << z;return oss.str();}};
	metadata["rotationX"] = {"float", "rotationX", sizeof(float), [&](const std::string &str) {rotationX = fromString<decltype(rotationX)>(str);}, [&]() { return &rotationX; }, [&]() { std::ostringstream oss;oss << rotationX;return oss.str();}};
	metadata["rotationY"] = {"float", "rotationY", sizeof(float), [&](const std::string &str) {rotationY = fromString<decltype(rotationY)>(str);}, [&]() { return &rotationY; }, [&]() { std::ostringstream oss;oss << rotationY;return oss.str();}};
	metadata["rotationZ"] = {"float", "rotationZ", sizeof(float), [&](const std::string &str) {rotationZ = fromString<decltype(rotationZ)>(str);}, [&]() { return &rotationZ; }, [&]() { std::ostringstream oss;oss << rotationZ;return oss.str();}};
	metadata["scaleX"] = {"float", "scaleX", sizeof(float), [&](const std::string &str) {scaleX = fromString<decltype(scaleX)>(str);}, [&]() { return &scaleX; }, [&]() { std::ostringstream oss;oss << scaleX;return oss.str();}};
	metadata["scaleY"] = {"float", "scaleY", sizeof(float), [&](const std::string &str) {scaleY = fromString<decltype(scaleY)>(str);}, [&]() { return &scaleY; }, [&]() { std::ostringstream oss;oss << scaleY;return oss.str();}};
	metadata["scaleZ"] = {"float", "scaleZ", sizeof(float), [&](const std::string &str) {scaleZ = fromString<decltype(scaleZ)>(str);}, [&]() { return &scaleZ; }, [&]() { std::ostringstream oss;oss << scaleZ;return oss.str();}};
	return metadata;
}
