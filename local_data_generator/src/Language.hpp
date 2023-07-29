#pragma once

#include <string>

#include <strong_type/strong_type.hpp>
#include <strong_type/equality.hpp>

namespace pokemon
{

using Language = strong::type<std::string, struct Language_, strong::equality>;

} // namespace pokemon
