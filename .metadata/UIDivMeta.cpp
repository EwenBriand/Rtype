/*
* CAUTION: This file was generated automatically.
* Do not modify it, as your changes will be lost.
*/

#include <sstream>
#include "ToBytes.hpp"
#include <iostream>
#include "metadata.hpp"
#include "UIDiv.hpp"

void UIDiv::Load(const std::string &filepath)
{
	bytes::ToBytes bytes;
	bytes.Load(filepath, m_x, m_y, m_width, m_height, m_r, m_g, m_b, m_a, m_border_r, m_border_g, m_border_b, m_border_a, m_border_thickness, m_visible);
}

void UIDiv::Save(const std::string &filepath)
{
	bytes::ToBytes bytes;
	bytes.Save(filepath, m_x, m_y, m_width, m_height, m_r, m_g, m_b, m_a, m_border_r, m_border_g, m_border_b, m_border_a, m_border_thickness, m_visible);
}

std::map<std::string, metadata_t> UIDiv::GetMetadata()
{
	std::map<std::string, metadata_t> metadata;
	metadata["m_x"] = {"float", "m_x", sizeof(float), [&](const std::string &str) {m_x = fromString<decltype(m_x)>(str);}, [&]() { return &m_x; }, [&]() { std::ostringstream oss;oss << m_x;return oss.str();}};
	metadata["m_y"] = {"float", "m_y", sizeof(float), [&](const std::string &str) {m_y = fromString<decltype(m_y)>(str);}, [&]() { return &m_y; }, [&]() { std::ostringstream oss;oss << m_y;return oss.str();}};
	metadata["m_width"] = {"float", "m_width", sizeof(float), [&](const std::string &str) {m_width = fromString<decltype(m_width)>(str);}, [&]() { return &m_width; }, [&]() { std::ostringstream oss;oss << m_width;return oss.str();}};
	metadata["m_height"] = {"float", "m_height", sizeof(float), [&](const std::string &str) {m_height = fromString<decltype(m_height)>(str);}, [&]() { return &m_height; }, [&]() { std::ostringstream oss;oss << m_height;return oss.str();}};
	metadata["m_r"] = {"int", "m_r", sizeof(int), [&](const std::string &str) {m_r = fromString<decltype(m_r)>(str);}, [&]() { return &m_r; }, [&]() { std::ostringstream oss;oss << m_r;return oss.str();}};
	metadata["m_g"] = {"int", "m_g", sizeof(int), [&](const std::string &str) {m_g = fromString<decltype(m_g)>(str);}, [&]() { return &m_g; }, [&]() { std::ostringstream oss;oss << m_g;return oss.str();}};
	metadata["m_b"] = {"int", "m_b", sizeof(int), [&](const std::string &str) {m_b = fromString<decltype(m_b)>(str);}, [&]() { return &m_b; }, [&]() { std::ostringstream oss;oss << m_b;return oss.str();}};
	metadata["m_a"] = {"int", "m_a", sizeof(int), [&](const std::string &str) {m_a = fromString<decltype(m_a)>(str);}, [&]() { return &m_a; }, [&]() { std::ostringstream oss;oss << m_a;return oss.str();}};
	metadata["m_border_r"] = {"int", "m_border_r", sizeof(int), [&](const std::string &str) {m_border_r = fromString<decltype(m_border_r)>(str);}, [&]() { return &m_border_r; }, [&]() { std::ostringstream oss;oss << m_border_r;return oss.str();}};
	metadata["m_border_g"] = {"int", "m_border_g", sizeof(int), [&](const std::string &str) {m_border_g = fromString<decltype(m_border_g)>(str);}, [&]() { return &m_border_g; }, [&]() { std::ostringstream oss;oss << m_border_g;return oss.str();}};
	metadata["m_border_b"] = {"int", "m_border_b", sizeof(int), [&](const std::string &str) {m_border_b = fromString<decltype(m_border_b)>(str);}, [&]() { return &m_border_b; }, [&]() { std::ostringstream oss;oss << m_border_b;return oss.str();}};
	metadata["m_border_a"] = {"int", "m_border_a", sizeof(int), [&](const std::string &str) {m_border_a = fromString<decltype(m_border_a)>(str);}, [&]() { return &m_border_a; }, [&]() { std::ostringstream oss;oss << m_border_a;return oss.str();}};
	metadata["m_border_thickness"] = {"int", "m_border_thickness", sizeof(int), [&](const std::string &str) {m_border_thickness = fromString<decltype(m_border_thickness)>(str);}, [&]() { return &m_border_thickness; }, [&]() { std::ostringstream oss;oss << m_border_thickness;return oss.str();}};
	metadata["m_visible"] = {"bool", "m_visible", sizeof(bool), [&](const std::string &str) {m_visible = fromString<decltype(m_visible)>(str);}, [&]() { return &m_visible; }, [&]() { std::ostringstream oss;oss << m_visible;return oss.str();}};
	return metadata;
}
