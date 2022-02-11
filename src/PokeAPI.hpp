#pragma once

#include <string_view>

#include <Pokemon.hpp>

// Todo : change the lang as string into an enum
Pokemon get_random_pokemon(std::string_view lang);
