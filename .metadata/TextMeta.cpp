/*
* CAUTION: This file was generated automatically.
* Do not modify it, as your changes will be lost.
*/

#include <sstream>
#include "ToBytes.hpp"
#include <iostream>
#include "metadata.hpp"
#include "Text.hpp"

void Text::Load(const std::string &filepath)
{
	bytes::ToBytes bytes;
	bytes.Load(filepath, m_text);
}

void Text::Save(const std::string &filepath)
{
	bytes::ToBytes bytes;
	bytes.Save(filepath, m_text);
}

std::map<std::string, metadata_t> Text::GetMetadata()
{
	std::map<std::string, metadata_t> metadata;
	metadata["m_text"] = {"std::string", "m_text", sizeof(std::string), [&](const std::string &str) {m_text = fromString<decltype(m_text)>(str);}, [&]() { return &m_text; }, [&]() { std::ostringstream oss;oss << m_text;return oss.str();}};
	return metadata;
}
