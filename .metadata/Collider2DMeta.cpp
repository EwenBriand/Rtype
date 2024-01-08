/*
* CAUTION: This file was generated automatically.
* Do not modify it, as your changes will be lost.
*/

#include <sstream>
#include "ToBytes.hpp"
#include <iostream>
#include "metadata.hpp"
#include "Collider2D.hpp"

void Collider2D::Load(const std::string &filepath)
{
	bytes::ToBytes bytes;
	bytes.Load(filepath, _tag, _layer, _vertices);
}

void Collider2D::Save(const std::string &filepath)
{
	bytes::ToBytes bytes;
	bytes.Save(filepath, _tag, _layer, _vertices);
}

std::map<std::string, metadata_t> Collider2D::GetMetadata()
{
	std::map<std::string, metadata_t> metadata;
	metadata["_tag"] = {"std::string", "_tag", sizeof(std::string), [&](const std::string &str) {_tag = fromString<decltype(_tag)>(str);}, [&]() { return &_tag; }, [&]() { std::ostringstream oss;oss << _tag;return oss.str();}};
	metadata["_layer"] = {"std::string", "_layer", sizeof(std::string), [&](const std::string &str) {_layer = fromString<decltype(_layer)>(str);}, [&]() { return &_layer; }, [&]() { std::ostringstream oss;oss << _layer;return oss.str();}};
	metadata["_vertices"] = {"std::vector<float>", "_vertices", sizeof(std::vector<float>), [&](const std::string &str) {std::stringstream _vertices_ss(str);std::string _vertices_value;while (getline(_vertices_ss, _vertices_value, ',')) {_vertices.push_back(fromString<decltype(_vertices)::value_type>(_vertices_value));}}, [&]() { return &_vertices; }, [&]() { std::ostringstream oss;for (auto &value : _vertices) {oss << value << ",";}return oss.str();}};
	return metadata;
}
