#pragma once

#include <string>

#include <nlohmann/json.hpp>

#include <strong_type/strong_type.hpp>
#include <strong_type/regular.hpp>
#include <strong_type/ordered.hpp>

namespace pokemon
{

using Type = strong::type<std::string, struct Type_, strong::regular, strong::ordered>;

void to_json(nlohmann::json& j, const Type& t) {
	j = t.value_of();
}

void from_json(const nlohmann::json& j, Type& t) {
	j.get_to(t.value_of());
}

} // namespace pokemon
