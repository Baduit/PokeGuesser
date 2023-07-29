#pragma once

#include <string>

#include <nlohmann/json.hpp>

#include <strong_type/strong_type.hpp>
#include <strong_type/regular.hpp>
#include <strong_type/ordered.hpp>

namespace pokemon
{

using Language = strong::type<std::string, struct Language_, strong::regular, strong::ordered>;

void to_json(nlohmann::json& j, const Language& l) {
	j = l.value_of();
}

void from_json(const nlohmann::json& j, Language& l) {
	j.get_to(l.value_of());
}

} // namespace pokemon
