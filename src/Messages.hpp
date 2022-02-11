#pragma once

#include <nlohmann/json.hpp>

namespace request
{

struct Start
{
	bool dummy;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Start, dummy)

struct Guess
{
	bool dummy;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Guess, dummy)

} // namespace request



namespace reply
{

struct Dummy
{
	bool dummy;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Dummy, dummy)

inline std::string to_string(const auto& message)
{
	return nlohmann::json(message).dump();
}

} // namespace reply