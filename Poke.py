import json
import requests
import random

from typing import Tuple

class Pokemon:
	def __init__(self) -> None:
		self.name = ""
		self.description = ""
		self.types = []
		self.height = 0
		self.weight = 0
		self.image_url = ""

poke_url = 'https://pokeapi.co/api/v2'

def get_type_from_url(url: str, lang: str) -> str:
	type_json = json.loads(requests.get(url).text)
	for name in type_json['names']:
		if name['language']['name'] == lang:
			return name['name']
	return 'type name not found in this langage'

def get_name_from_json(species_json, lang: str) -> str:
	for name in species_json['names']:
		if name['language']['name'] == lang:
			return name['name']
	return 'name not found in this langage'

def get_description_from_json(species_json, lang: str) -> str:
	for name in species_json['flavor_text_entries']:
		if name['language']['name'] == lang:
			# Todo gérer les \n dans la description
			return name['flavor_text']
	return 'name not found in this langage'

def get_name_and_description_from_url(url: str, lang: str) -> Tuple[str, str]:
	species_json = json.loads(requests.get(url).text)
	return (get_name_from_json(species_json, lang), get_description_from_json(species_json, lang))

def get_pokemon(pokemon_id: int, lang: str) -> Pokemon:
	pokemon = Pokemon()

	pokemon_json = json.loads(requests.get(f"{poke_url}/pokemon/{pokemon_id}").text)
	pokemon.height = pokemon_json['height']
	pokemon.weight = pokemon_json['weight']
	pokemon.image_url = pokemon_json['sprites']['other']['official-artwork']['front_default']

	pokemon.name, pokemon.description = get_name_and_description_from_url(pokemon_json['species']['url'], lang)
	for type_url in pokemon_json['types']:
		pokemon.types.append(get_type_from_url(type_url['type']['url'], lang))

	return pokemon

def get_number_of_pokemon() -> int:
	return json.loads(requests.get(f"{poke_url}/pokemon-species").text)['count']

def get_random_pokemon_id() -> int:
	nb_pokemons = get_number_of_pokemon()
	return random.randint(1, nb_pokemons)

def get_random_pokemon(lang: str) -> Pokemon:
	pokemon_id = get_random_pokemon_id()
	pokemon = get_pokemon(pokemon_id, lang)
	return pokemon

if __name__ == "__main__":
	print(get_random_pokemon('fr').__dict__)