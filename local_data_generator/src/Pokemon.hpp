#pragma once

#include <map>
#include <string>

#include <nlohmann/json.hpp>

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

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Pokemon, names, descriptions, height, weight, generation, types)

} // namespace pokemon


