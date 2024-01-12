/*
* CAUTION: This file was generated automatically.
* Do not modify it, as your changes will be lost.
*/

#include <sstream>
#include "ToBytes.hpp"
#include <iostream>
#include "metadata.hpp"
#include "InputField.hpp"

void InputField::Load(const std::string &filepath)
{
	bytes::ToBytes bytes;
	bytes.Load(filepath, m_label, m_placeholder, m_x, m_y);
}

void InputField::Save(const std::string &filepath)
{
	bytes::ToBytes bytes;
	bytes.Save(filepath, m_label, m_placeholder, m_x, m_y);
}

std::map<std::string, metadata_t> InputField::GetMetadata()
{
	std::map<std::string, metadata_t> metadata;
	metadata["m_label"] = {"std::string", "m_label", sizeof(std::string), [&](const std::string &str) {m_label = fromString<decltype(m_label)>(str);}, [&]() { return &m_label; }, [&]() { std::ostringstream oss;oss << m_label;return oss.str();}};
	metadata["m_placeholder"] = {"std::string", "m_placeholder", sizeof(std::string), [&](const std::string &str) {m_placeholder = fromString<decltype(m_placeholder)>(str);}, [&]() { return &m_placeholder; }, [&]() { std::ostringstream oss;oss << m_placeholder;return oss.str();}};
	metadata["m_x"] = {"float", "m_x", sizeof(float), [&](const std::string &str) {m_x = fromString<decltype(m_x)>(str);}, [&]() { return &m_x; }, [&]() { std::ostringstream oss;oss << m_x;return oss.str();}};
	metadata["m_y"] = {"float", "m_y", sizeof(float), [&](const std::string &str) {m_y = fromString<decltype(m_y)>(str);}, [&]() { return &m_y; }, [&]() { std::ostringstream oss;oss << m_y;return oss.str();}};
	return metadata;
}
