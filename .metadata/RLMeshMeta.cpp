/*
* CAUTION: This file was generated automatically.
* Do not modify it, as your changes will be lost.
*/

#include <sstream>
#include "ToBytes.hpp"
#include <iostream>
#include "metadata.hpp"
#include "RLMesh.hpp"

void RLMesh::Load(const std::string &filepath)
{
	bytes::ToBytes bytes;
	bytes.Load(filepath, m_path, m_texturePath);
}

void RLMesh::Save(const std::string &filepath)
{
	bytes::ToBytes bytes;
	bytes.Save(filepath, m_path, m_texturePath);
}

std::map<std::string, metadata_t> RLMesh::GetMetadata()
{
	std::map<std::string, metadata_t> metadata;
	metadata["m_path"] = {"std::string", "m_path", sizeof(std::string), [&](const std::string &str) {m_path = fromString<decltype(m_path)>(str);}, [&]() { return &m_path; }, [&]() { std::ostringstream oss;oss << m_path;return oss.str();}};
	metadata["m_texturePath"] = {"std::string", "m_texturePath", sizeof(std::string), [&](const std::string &str) {m_texturePath = fromString<decltype(m_texturePath)>(str);}, [&]() { return &m_texturePath; }, [&]() { std::ostringstream oss;oss << m_texturePath;return oss.str();}};
	return metadata;
}
