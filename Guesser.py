import random
import datetime
import schedule
import time

from fastapi import FastAPI
from fastapi.staticfiles import StaticFiles

from Poke import Pokemon, get_random_pokemon

MAX_NB_PLAYER = 100000
MAX_PLAYER_ID = MAX_NB_PLAYER * 10000
MIN_PLAYER_ID = 1

app = FastAPI()

class DailyPokemonHandler:
	def __init__(self) -> None:
		self.update()

	def update(self):
		self.pokemon = get_random_pokemon('fr')
		
dailyPokemonHandler = DailyPokemonHandler()
schedule.every().day.at("04:00").do(dailyPokemonHandler.update)

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
		elif pokemon.name.casefold() == pokemon_name.casefold():
			return {
				"success": True,
				"game_status": "finished",
				"pokemon": dailyPokemonHandler.pokemon.__dict__
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
				"field_value": getattr(dailyPokemonHandler.pokemon, field_name)
			}

	
players = {}

@app.get("/start")
async def root():
	if len(players) > MAX_NB_PLAYER:
		return {
			"error_message": f"There is more than {MAX_NB_PLAYER} players. That's too much"
		}
	else:
		new_id = random.randint(MIN_PLAYER_ID, MAX_PLAYER_ID)
		# Handle collision, it can't infinite loop because MAX_PLAYER_ID - MIN_PLAYER_ID is whay higher than MAX_NB_PLAYER
		while new_id in players:
			new_id = random.randint(MIN_PLAYER_ID, MAX_PLAYER_ID)
		players[new_id] = PlayerStatus()
		return {
			"id": new_id,
			"description": dailyPokemonHandler.pokemon.description
		}


@app.get("/guess")
async def root(id: int, pokemon_name: str):
	return players[id].try_guess(dailyPokemonHandler.pokemon, pokemon_name)

app.mount("/", StaticFiles(directory="public", html=True), name="public")
