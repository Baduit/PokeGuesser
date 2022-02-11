#pragma once

#include <string_view>
#include <algorithm>
#include <random>

#include <network.hpp>

#include <Pokemon.hpp>

template<std::size_t N>
struct Path
{
    char p[N]{};
 
    constexpr Path (char const(&pp)[N])
    {
        std::ranges::copy(pp, p);
    };


	template <std::size_t S>
	constexpr Path<N + S> operator/(const Path<S>& other) const
	{
		char concatenation[N + S];
		std::ranges::copy(p, concatenation);
		concatenation[N - 1] = '/'; // transform the \0 into a slash because it is a path
		std::ranges::copy(other.p, concatenation + N);
		return Path<N + S>(concatenation);
	}

	template <std::size_t S>
	constexpr Path<N + S> operator/(char const(&other)[S]) const
	{
		char concatenation[N + S];
		std::ranges::copy(p, concatenation);
		concatenation[N - 1] = '/'; // transform the \0 into a slash because it is a path
		std::ranges::copy(other, concatenation + N);
		return Path<N + S>(concatenation);
	}

	template <std::size_t S>
	constexpr bool operator==(const Path<S>& other) const
	{
		return std::ranges::equal(p, other.p);
	}

	// Make it simpler to use with cpp-httplib, but when I will make the mise en cache of request, this overload will probably be useless
	constexpr operator const char*() const
	{
		return p;
	}
};

// I should put this in its own namespace
// Commented, it fucks up my autocompletion 
/* template<Path P>
constexpr auto operator"" _p()
{
    return P;
} */

int get_random_number(int max_included)
{
	static std::random_device r;
	static std::default_random_engine e1(r());
    std::uniform_int_distribution<int> uniform_dist(0, max_included);
	return uniform_dist(e1);
}

// todo: cache the request so I never make the same request twice
Pokemon get_random_pokemon_from_api(const std::string& lang_code)
{
	namespace fs = std::filesystem;

	Pokemon p;
	p.name = "Pikachu";

	constexpr auto base_path = Path("/api/v2");
	std::string base_path_str = "/api/v2";

	httplib::Client cli("https://pokeapi.co");
	auto res_number_of_pokemon = cli.Get(base_path / "pokemon-species");

	int number_of_pokemon = nlohmann::json::parse(res_number_of_pokemon->body).at("count").get<int>();
	int random_pokemon_id = get_random_number(number_of_pokemon);
	std::cout << random_pokemon_id << std::endl;

	std::string get_pokemon_url = base_path_str + "/" + "pokemon" + "/" + std::to_string(random_pokemon_id); 
	std::cout << get_pokemon_url << std::endl;
	auto res_pokemon = cli.Get(get_pokemon_url.c_str());
	//std::cout << nlohmann::json::parse(res_pokemon->body).dump(4) << std::endl;
	
	auto pokemon_json = nlohmann::json::parse(res_pokemon->body);
	p.weight = pokemon_json.at("weight").get<int>();
	p.height = pokemon_json.at("height").get<int>();
	p.image_url = pokemon_json.at("sprites").at("other").at("official-artwork").at("front_default").get<std::string>();

	// todo (url are just example url, not real url to use) : 
	// - get random pokemon : https://pokeapi.co/api/v2/pokemon/1/
	// - get height and width
	// - get an image url (official artwork if it is always here, to investigate)
	// - retrieve species : https://pokeapi.co/api/v2/pokemon-species/1/
	// - from the specie, get the name in the right langage
	// - from the specie, get all favors of the right langage, then 

	return p;
}