/*
* CAUTION: This file was generated automatically.
* Do not modify it, as your changes will be lost.
*/

#include <sstream>
#include "ToBytes.hpp"
#include <iostream>
#include "metadata.hpp"
#include "UIMenu.hpp"

void UIMenu::Load(const std::string &filepath)
{
	bytes::ToBytes bytes;
	bytes.Load(filepath, m_x, m_y, m_alwaysVisible);
}

void UIMenu::Save(const std::string &filepath)
{
	bytes::ToBytes bytes;
	bytes.Save(filepath, m_x, m_y, m_alwaysVisible);
}

std::map<std::string, metadata_t> UIMenu::GetMetadata()
{
	std::map<std::string, metadata_t> metadata;
	metadata["m_x"] = {"float", "m_x", sizeof(float), [&](const std::string &str) {m_x = fromString<decltype(m_x)>(str);}, [&]() { return &m_x; }, [&]() { std::ostringstream oss;oss << m_x;return oss.str();}};
	metadata["m_y"] = {"float", "m_y", sizeof(float), [&](const std::string &str) {m_y = fromString<decltype(m_y)>(str);}, [&]() { return &m_y; }, [&]() { std::ostringstream oss;oss << m_y;return oss.str();}};
	metadata["m_alwaysVisible"] = {"bool", "m_alwaysVisible", sizeof(bool), [&](const std::string &str) {m_alwaysVisible = fromString<decltype(m_alwaysVisible)>(str);}, [&]() { return &m_alwaysVisible; }, [&]() { std::ostringstream oss;oss << m_alwaysVisible;return oss.str();}};
	return metadata;
}
