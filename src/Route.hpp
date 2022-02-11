#pragma once

#include <string>

template <typename Req, typename Rep>
struct Route
{
	using Request = Req;
	using Reply = Rep;

	std::string path;
};
