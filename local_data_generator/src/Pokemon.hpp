#pragma once

#include <map>
#include <string>

#include <nlohmann/json.hpp>

#include <Language.hpp>
#include <Type.hpp>
#include <units/lenght/Meter.hpp>
#include <units/mass/Kilogram.hpp>

namespace pokemon
{

struct Pokemon
{
	std::size_t id;
	std::map<Language, std::string> names;
	std::map<Language, std::vector<std::string>> descriptions;
	Meter height;
	Kilogram weight;
	std::map<Language, std::vector<Type>> types;
	std::string image_url;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Pokemon, id, names, descriptions, height, weight, types, image_url)

} // namespace pokemon


