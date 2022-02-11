#pragma once

#include <string>

struct Path
{
	std::string str;

	// Not std::string_view because there is no operator + between std::string and std::string_view
	Path operator/(const std::string& right) const
	{
		return Path{ .str = str + '/' + right };
	}

	Path operator/(int right) const
	{
		return Path{ .str = str + '/' + std::to_string(right) };
	}

	operator const char*() const
	{
		return str.c_str();
	}
};