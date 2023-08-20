#include <iostream>

#include <PokemonGenerator.hpp>

int main(int, char**)
{
	std::cout << "Hello, World!" << std::endl;
	for (auto&& pokemon: pokemon::PokemonGenerator::generatePokemon("C:/Users/Athlena/Documents/GitHub/api-data/data"))
	{
		std::cout << nlohmann::json(pokemon).dump(4) << std::endl;
	}
}