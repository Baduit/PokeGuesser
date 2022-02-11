#pragma once

#include <string_view>
#include <algorithm>
#include <cctype>

inline bool compare_non_case_senstive(std::string_view left, std::string_view right)
{
	auto comparator =
		[](unsigned char a, unsigned char b)
		{
			return std::tolower(a) == std::tolower(b);
		};
	return std::ranges::equal(left, right, comparator);
}