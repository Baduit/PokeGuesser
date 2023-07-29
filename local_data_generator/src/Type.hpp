#pragma once

#include <string>

#include <strong_type/strong_type.hpp>
#include <strong_type/equality.hpp>

namespace pokemon
{

using Type = strong::type<std::string, struct Type_, strong::equality>;

} // namespace pokemon
