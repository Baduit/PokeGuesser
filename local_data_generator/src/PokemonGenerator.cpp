#include <ranges>
#include <expected>
#include <fstream>

#include <PokemonGenerator.hpp>


namespace
{

enum class ToIntError
{
	InvalidArgument, // std::errc::invalid_argument
	ResultOutOfRange // std::errc::result_out_of_range 
};

std::expected<std::size_t, ToIntError> to_int(std::string_view s) 
{
	std::size_t value;
	auto result = std::from_chars(s.data(), s.data() + s.size(), value);
	if (result.ec == std::errc{})
		return value;
	else if (result.ec == std::errc::invalid_argument)
		return std::unexpected(ToIntError::InvalidArgument);
	else if (result.ec == std::errc::result_out_of_range)
		return std::unexpected(ToIntError::ResultOutOfRange);
	else
		std::unreachable();
};

bool is_dir(const std::filesystem::directory_entry& entry)
{
	return entry.is_directory();
}

auto get_file_info(const std::filesystem::directory_entry& entry)
{
	return
		std::make_pair(
			to_int(entry.path().filename().string()),
			entry.path() / "index.json"
		);
};

} // namespace

namespace pokemon
{

// Implementation
tl::generator<Pokemon> PokemonGenerator::generatePokemon(std::filesystem::path data_location)
{
	std::filesystem::path pokemon_directory = data_location;
	pokemon_directory += "/api/v2/pokemon";

	for (auto const& [id, pokemon_filename] : std::filesystem::directory_iterator{ pokemon_directory }
		| std::views::filter(is_dir)
		// | std::views::filter(is_not_special_form)
		| std::views::transform(get_file_info))
	{
		Pokemon pokemon{};
		pokemon.id = *id;

		auto pokemon_info = getPokemonInfo(pokemon_filename);
		pokemon.height = pokemon_info.height;
		pokemon.weight = pokemon_info.weight;
		pokemon.image_url = std::move(pokemon_info.image_url);

		auto spec_location = data_location / pokemon_info.specie;
		auto pokemon_specie_info = getPokemonSpeciesInfo(data_location / pokemon_info.specie / "index.json");
		pokemon.names = std::move(pokemon_specie_info.names);
		pokemon.descriptions = std::move(pokemon_specie_info.descriptions);

		for (const auto& type_url : pokemon_info.types)
		{
			auto type_info = getTypeInfo(data_location / type_url / "index.json");
			for (auto&& [language, type]: type_info)
			{
				pokemon.types[std::move(language)].push_back(std::move(type));
			}
		}
		
		co_yield pokemon;
	}

}

PokemonGenerator::PokemonInfo PokemonGenerator::getPokemonInfo(const std::filesystem::path& pokemon_filename)
{
	std::ifstream stream(pokemon_filename);
	nlohmann::json data = nlohmann::json::parse(stream);

	// todo: error handling
	auto height = data.at("height").get<Decimeter>();
	auto weight = data.at("weight").get<Hectogram>();
	auto image_url = data.at("sprites").at("other").at("official-artwork").at("front_default").get<std::string>();
	auto specie = data.at("species").at("url").get<std::string_view>().substr(1);

	auto type_obj_to_url = [](auto&& type_obj){ return type_obj.at("type").at("url"); };

	std::vector<std::filesystem::path> types;
	for (auto&& t: data.at("types")
		| std::views::transform(type_obj_to_url))
	{
		types.push_back(t.get<std::string_view>().substr(1));
	}
	
	return
		PokemonInfo{
			.height = height,
			.weight = weight,
			.image_url = std::move(image_url),
			.specie = std::move(specie),
			.types = std::move(types)
		};
}



PokemonGenerator::PokemonSpecieInfo PokemonGenerator::getPokemonSpeciesInfo(const std::filesystem::path& pokemon_specie_filename)
{
	std::ifstream stream(pokemon_specie_filename);
	nlohmann::json data = nlohmann::json::parse(stream);
	
	std::map<Language, std::string> names;
	for (const auto& name_object: data.at("names"))
	{
		auto language  = name_object.at("language").at("name").get<Language>();
		auto name = name_object.at("name").get<std::string>();
		names.insert(std::make_pair(std::move(language), std::move(name)));
	}

	std::map<Language, std::vector<std::string>> descriptions;
	for (const auto& description_object: data.at("flavor_text_entries"))
	{
		auto language  = description_object.at("language").at("name").get<Language>();
		auto description = description_object.at("flavor_text").get<std::string>();
		descriptions[std::move(language)].push_back(std::move(description));
	}
	
	return
		PokemonSpecieInfo{
			.names = std::move(names),
			.descriptions = std::move(descriptions)
		};
}



std::vector<PokemonGenerator::TypeInfo> PokemonGenerator::getTypeInfo(const std::filesystem::path& type_filename)
{
	std::ifstream stream(type_filename);
	nlohmann::json data = nlohmann::json::parse(stream);
	
	std::vector<PokemonGenerator::TypeInfo> types;

	for (const auto& type_obj: data.at("names"))
	{
		auto language = type_obj.at("language").at("name").get<Language>();
		auto type = type_obj.at("name").get<Type>();
		types.push_back(
			TypeInfo{
				.language = language,
				.type = type
			}
		);
	}

	return types;
}
	
} // namespace pokemon
