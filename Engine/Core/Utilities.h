#pragma once

#include <string>

namespace nh
{
	std::string ToLower(const std::string& string);
	bool ICompare(const std::string& string1, const std::string& string2);
	std::string UniqueString(const std::string& string);
}