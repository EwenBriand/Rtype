/*
* CAUTION: This file was generated automatically.
* Do not modify it, as your changes will be lost.
*/

#include <sstream>
#include "ToBytes.hpp"
#include <iostream>
#include "metadata.hpp"
#include "UIButton.hpp"

void UIButton::Load(const std::string &filepath)
{
	bytes::ToBytes bytes;
	bytes.Load(filepath, m_description, m_text, m_clickCallbackName);
}

void UIButton::Save(const std::string &filepath)
{
	bytes::ToBytes bytes;
	bytes.Save(filepath, m_description, m_text, m_clickCallbackName);
}

std::map<std::string, metadata_t> UIButton::GetMetadata()
{
	std::map<std::string, metadata_t> metadata;
	metadata["m_description"] = {"std::string", "m_description", sizeof(std::string), [&](const std::string &str) {m_description = fromString<decltype(m_description)>(str);}, [&]() { return &m_description; }, [&]() { std::ostringstream oss;oss << m_description;return oss.str();}};
	metadata["m_text"] = {"std::string", "m_text", sizeof(std::string), [&](const std::string &str) {m_text = fromString<decltype(m_text)>(str);}, [&]() { return &m_text; }, [&]() { std::ostringstream oss;oss << m_text;return oss.str();}};
	metadata["m_clickCallbackName"] = {"std::string", "m_clickCallbackName", sizeof(std::string), [&](const std::string &str) {m_clickCallbackName = fromString<decltype(m_clickCallbackName)>(str);}, [&]() { return &m_clickCallbackName; }, [&]() { std::ostringstream oss;oss << m_clickCallbackName;return oss.str();}};
	return metadata;
}
