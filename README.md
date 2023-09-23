# PokeGuesser
Your daily dose of Pokemon, every day you have one pokémon to guess. You have only 5 tries, but to help you, you have the informations from the pokedex. First the description, then the types, the weight, the height and finally an image. 

You can play it here : https://pokeguesser.baduit.eu/

## Technical stack
- The data are extracted from the poke api using a C++ program : [poke_data](https://github.com/Baduit/poke_data)
- The back is done in Python with [FastAPI](https://fastapi.tiangolo.com/)
- The front is made with react
- It is deployed on my raspberry pi with a docker image (built automatically when something is pushed on main)

## Todos
* Finish translation to have a multi-langage front
* Make a terminal client maybe, that would be fun
