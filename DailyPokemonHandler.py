import schedule

from Poke import Pokemon, get_random_pokemon

class DailyPokemonHandler:
	def __init__(self) -> None:
		self.update()
		schedule.every().day.at("04:00").do(self.update)

	def update(self):
		self.pokemon = get_random_pokemon('fr')
