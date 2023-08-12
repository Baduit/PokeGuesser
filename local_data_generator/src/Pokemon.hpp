#pragma once

#include <map>
#include <string>

#include <nlohmann/json.hpp>

#include <Language.hpp>
#include <Type.hpp>
#include <units/lenght/Meter.hpp>
#include <units/lenght/Decimeter.hpp>
#include <units/mass/Kilogram.hpp>
#include <units/mass/Hectogram.hpp>

namespace pokemon
{

struct Pokemon
{
	std::map<Language, std::string> names;
	std::map<Language, std::string> descriptions;
	Meter height;
	Kilogram weight;
	std::size_t generation;
	std::map<Language, Type> types;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Pokemon, names, descriptions, height, weight, generation, types)

} // namespace pokemon


