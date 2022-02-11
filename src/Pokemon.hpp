#pragma once

#include <string>
#include <vector>

#include <nlohmann/json.hpp>

struct Pokemon
{
	std::string name;
	std::string description;
	std::vector<std::string> types;
	int height = 0;
	int weight = 0;
	std::string image_url;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Pokemon, name, description, types, height, weight, image_url)