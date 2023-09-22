import random
import datetime


from fastapi import FastAPI
from fastapi.staticfiles import StaticFiles
from fastapi.middleware.cors import CORSMiddleware
from fastapi_utils.tasks import repeat_every

from DailyPokemonHandler import DailyPokemonHandler
from Poke import Pokemon, NamesHandler

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


dailyPokemonHandlers = {
	'fr': DailyPokemonHandler('fr'),
	'en': DailyPokemonHandler('en'),
	'de': DailyPokemonHandler('de'),
	'it': DailyPokemonHandler('it'),
	'es': DailyPokemonHandler('es')
}

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

	def is_active(self):
		if (datetime.datetime.now() - self.last_activity) > datetime.timedelta(hours=6):
			return False
		elif self.finish and (datetime.datetime.now() - self.last_activity) > datetime.timedelta(hours=1):
			return False
		else:
			return True

	def try_guess(self, pokemon: Pokemon, pokemon_name: str, lang: str):
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
				"pokemon": dailyPokemonHandlers[lang].pokemon.__dict__
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
				"field_value": getattr(dailyPokemonHandlers[lang].pokemon, field_name)
			}

class PlayerManager:
	def __init__(self) -> None:
		self.players = {}
		self.clear_list()

	def clear_list(self):
		self.players.clear()
		now = datetime.datetime.now()
		# Update time is 4 am, not the time when the app is launched
		self.last_update = datetime.datetime(year=now.year, month=now.month, day=now.day, hour=4, minute=0, second=0, microsecond=0)

	def is_clear_list_needed(self):
		actual_time = datetime.datetime.now()
		delta_time = actual_time - self.last_update
		return delta_time > datetime.timedelta(1)

	# Clear list is needed when the pokemon is updated
	def clear_list_if_needed(self):
		if self.is_clear_list_needed():
			self.clear_list()

	def add_player(self):
		if len(self.players) > MAX_NB_PLAYER:
			return -1
		else:
			new_id = random.randint(MIN_PLAYER_ID, MAX_PLAYER_ID)
			# Handle collision, it can't infinite loop because MAX_PLAYER_ID - MIN_PLAYER_ID is whay higher than MAX_NB_PLAYER
			while new_id in self.players:
				new_id = random.randint(MIN_PLAYER_ID, MAX_PLAYER_ID)
			self.players[new_id] = PlayerStatus()
			return new_id

	def clear_inactive_players(self):
		self.players = {id:p for (id,p) in self.players.items() if p.is_active()}
	
player_manager = PlayerManager()
names_handler = NamesHandler()

def daily_updates():
	for poke_handler in dailyPokemonHandlers.values():
		poke_handler.update_if_needed()
	player_manager.clear_list_if_needed()

@app.on_event("startup") 
@repeat_every(seconds=60*60) 
async def call_clear_inactive_players():
	player_manager.clear_inactive_players()

@app.get("/start")
async def start_game(lang: str = 'fr'):
	daily_updates()
	new_id = player_manager.add_player()
	if new_id == -1:
		return {
			"error_message": f"There is more than {MAX_NB_PLAYER} players. That's too much"
		}
	else:
		return {
			"id": new_id,
			"description": dailyPokemonHandlers[lang].pokemon.description
		}


@app.get("/guess")
async def guess_pokemon(id: int, pokemon_name: str, lang: str = 'fr'):
	daily_updates()
	return player_manager.players[id].try_guess(dailyPokemonHandlers[lang].pokemon, pokemon_name, lang)

@app.get("/names")
async def get_names(lang: str):
	return {
		'names': names_handler.get_all_pokemon_names(lang)
	}

@app.get("/stats/nb_players")
async def get_number_of_players():
	return len(player_manager.players)

app.mount("/", StaticFiles(directory="front/build", html=True), name="Morgan<3")

