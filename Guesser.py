import random
import datetime


from fastapi import FastAPI
from fastapi.staticfiles import StaticFiles
from fastapi.middleware.cors import CORSMiddleware

from DailyPokemonHandler import DailyPokemonHandler
from Poke import Pokemon, POKEMON_NAMES


MAX_NB_PLAYER = 100000
MAX_PLAYER_ID = MAX_NB_PLAYER * 10000
MIN_PLAYER_ID = 1

app = FastAPI()
app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)


dailyPokemonHandler = DailyPokemonHandler()

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
async def start_game():
	dailyPokemonHandler.update_if_needed()
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
async def guess_pokemon(id: int, pokemon_name: str):
	dailyPokemonHandler.update_if_needed()
	return players[id].try_guess(dailyPokemonHandler.pokemon, pokemon_name)

@app.get("/names")
async def get_names(lang: str):
	return {
		'names': POKEMON_NAMES[lang]
	}

app.mount("/", StaticFiles(directory="public", html=True), name="public")
