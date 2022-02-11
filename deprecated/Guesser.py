import random
import datetime

from fastapi import FastAPI
from fastapi.staticfiles import StaticFiles

from Poke import Pokemon, get_random_pokemon

app = FastAPI()

daily_pokemon = get_random_pokemon('fr')

info_order = [
	"description",
	"types",
	"height",
	"weight",
	"image_url",
	"name"
]

class PlayerStatus:
	def __init__(self) -> None:
		self.creation_date = datetime.datetime.now()
		self.last_activity = datetime.datetime.now()
		self.nb_try = 0
		self.finish = False

	def try_guess(self, pokemon: Pokemon, pokemon_name: str):
		if self.finish == True:
			return {
				"success": False,
				"game_status": "finished",
				"error_message": "You played enough for today !"
			}
		elif pokemon.name == pokemon_name:
			return {
				"success": True,
				"game_status": "finished",
				"pokemon": daily_pokemon.__dict__
			}
		else:
			self.nb_try += 1
			field_name = info_order[self.nb_try]
			game_status = "running"
			if self.nb_try == len(info_order) - 1:
				self.finish = True
				game_status = "finished"
			return {
				"success": False,
				"game_status": game_status,
				"field_name": field_name,
				"field_value": getattr(daily_pokemon, field_name)
			}

	
players = {}

@app.get("/start")
async def root():
	new_id = random.randint(1, 1000000000)
	print(f"Add new player with id: {new_id}")
	players[new_id] = PlayerStatus()
	return {
		"id": new_id,
		"description": daily_pokemon.description
	}


@app.get("/guess")
async def root(id: int, pokemon_name: str):
	print(f"Received request from : {id} with the pokemon name: {pokemon_name}")
	return players[id].try_guess(daily_pokemon, pokemon_name)

app.mount("/", StaticFiles(directory="public", html=True), name="public")
