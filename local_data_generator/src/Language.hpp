#pragma once

#include <string>

#include <strong_type/strong_type.hpp>
#include <strong_type/regular.hpp>
#include <strong_type/ordered.hpp>

namespace pokemon
{

using Language = strong::type<std::string, struct Language_, strong::regular, strong::ordered>;


} // namespace pokemon
