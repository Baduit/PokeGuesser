#include <fstream>

#include <ioUtils.hpp>

namespace
{

std::size_t getIfstreamSize(std::ifstream& stream)
{
	stream.seekg(0, std::ios::end);
	std::size_t file_size = static_cast<std::size_t>(stream.tellg());
    stream.seekg(0, std::ios::beg);
	return file_size;
}

}

namespace pokemon
{

auto readFileAtOnce(const std::filesystem::path& filepath) -> std::expected<std::string, ReadFileError>
{
	std::ifstream stream(filepath);
	if (!stream)
		return std::unexpected(ReadFileError::CantOpenFile);
	
	std::size_t file_size = getIfstreamSize(stream);
	if (!stream)
		return std::unexpected(ReadFileError::ErrorDeterminingFileSize);

	std::string contents;
	contents.resize(file_size);		
    stream.read(&contents[0], static_cast<std::streamsize>(contents.size()));
	if (stream.gcount() != file_size)
		return std::unexpected(ReadFileError::CouldNotReadWholeFile);
		
    return contents;
}

}