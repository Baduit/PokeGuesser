#pragma once

#include <map>
#include <string>

#include <Language.hpp>
#include <Type.hpp>

namespace pokemon
{

struct Pokemon
{
	std::map<Language, std::string> names;
	std::map<Language, std::string> descriptions;
	std::size_t height;
	std::size_t weight;
	std::size_t generation;
	std::map<Language, Type> types;
};



} // namespace pokemon


