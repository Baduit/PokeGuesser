#include <nlohmann/json.hpp>

#include <PokeAPI.hpp>
#include <network.hpp>
#include <Path.hpp>

const Path POKE_API_PATH = Path{ .str = "https://pokeapi.co/api/v2" };

std::string get_type_from_url(const std::string& url, std::string_view lang)
{
	httplib::Client cli(url.c_str());
	auto json_res = nlohmann::json::parse(cli.Get("")->body);
	for (const auto& name: json_res.at("names"))
	{
		if (name.at("language").at("name").get<std::string>() == lang)
		{
			return name.at("name").get<std::string>();
		}
	}
	return "Type name not found in this langage";
}

std::string get_name_from_json(const nlohmann::json& species, std::string_view lang)
{
	for (const auto& name: species.at("names"))
	{
		if (name.at("language").at("name").get<std::string>() == lang)
		{
			return name.at("name").get<std::string>();
		}
	}
	return "Name not found in this langage";
}

std::string get_description_from_json(const nlohmann::json& species, std::string_view lang)
{
	for (const auto& name: species.at("flavor_text_entries"))
	{
		if (name.at("language").at("name").get<std::string>() == lang)
		{
			return name.at("flavor_text").get<std::string>();
		}
	}
	return "Description not found in this langage";
}

std::pair<std::string, std::string> get_name_and_description_from_url(const std::string& url, std::string_view lang)
{
	httplib::Client cli(url.c_str());;
	auto species = nlohmann::json::parse(cli.Get("")->body);
	return { get_name_from_json(species, lang), get_description_from_json(species, lang) };
}

Pokemon get_random_pokemon(std::string_view lang)
{
	return {};
}