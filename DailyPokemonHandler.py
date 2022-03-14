import datetime
from turtle import update

from Poke import Pokemon, get_random_pokemon

class DailyPokemonHandler:
	def __init__(self, lang: str) -> None:
		self.lang = lang
		self.update()

	def update(self):
		self.pokemon = get_random_pokemon(self.lang)
		self.hide_pokemon_name_in_description()
		now = datetime.datetime.now()
		# Update time is 4 am, not the time when the app is launched
		self.last_update = datetime.datetime(year=now.year, month=now.month, day=now.day, hour=4, minute=0, second=0, microsecond=0)

	def is_update_needed(self):
		actual_time = datetime.datetime.now()
		delta_time = actual_time - self.last_update
		return delta_time > datetime.timedelta(1)

	def update_if_needed(self):
		if self.is_update_needed():
			self.update()

	def hide_pokemon_name_in_description(self):
		self.pokemon.description.replace(self.pokemon.name, '******')

