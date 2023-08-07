#pragma once

#include <expected>
#include <string>
#include <filesystem>

namespace pokemon
{

// Temporary
enum class ReadFileError
{
	CantOpenFile,
	ErrorDeterminingFileSize,
	CouldNotReadWholeFile,
};

// Don't be stupid and use it on enourmous file
auto readFileAtOnce(const std::filesystem::path& filepath) -> std::expected<std::string, ReadFileError>;

}