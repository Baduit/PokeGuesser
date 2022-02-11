#include <array>
#include <string_view>
#include <charconv>

#include <nlohmann/json.hpp>

#include <App.hpp>
#include <PokeAPI.hpp>
#include <StringviewUtils.hpp>
#include <Random.hpp>

App::App():
	_daily_pokemon(get_random_pokemon("fr"))
{
	_srv.Get("/start",
		[&](const httplib::Request& req, httplib::Response& res)
		{
			nlohmann::json reply;

			reply["id"] = add_player();
			reply["description"] = _daily_pokemon.description;

			res.body = reply.dump();
			res.set_header("Access-Control-Allow-Origin", "*");
		});

	_srv.Get("/start",
		[&](const httplib::Request& req, httplib::Response& res)
		{
			nlohmann::json reply;

			const auto [success, finished, nb_tries] = guess(get_integer_param("id", req.params), get_string_param("pokemon_name", req.params));
			reply["success"] = success;
			// todo replace thoses string for game_status
			if (finished)
			{

			}
			else if (success)
			{
				reply["game_status"] = "finished";
				reply["pokemon"] = _daily_pokemon;
			}
			else
			{
				if (nb_tries <= 0)
					throw std::runtime_error("Nb tries is weird because too low");
				
				if (nb_tries == 1)
				{
					reply["game_status"] = "running";
					reply["field_name"] = "types";
					reply["field_value"] = _daily_pokemon.types;
				}
				else if (nb_tries == 2)
				{
					reply["game_status"] = "running";
					reply["field_name"] = "height";
					reply["field_value"] = _daily_pokemon.height;
				}
				else if (nb_tries == 3)
				{
					reply["game_status"] = "running";
					reply["field_name"] = "weight";
					reply["field_value"] = _daily_pokemon.weight;
				}
				else if (nb_tries == 4)
				{
					reply["game_status"] = "running";
					reply["field_name"] = "image_url";
					reply["field_value"] = _daily_pokemon.image_url;
				}
				else if (nb_tries == 5)
				{
					reply["game_status"] = "finished";
					reply["field_name"] = "name";
					reply["field_value"] = _daily_pokemon.name;
				}
				else
				{
					throw std::runtime_error("Nb tries is weird because too high");
				}
			}

			res.body = reply.dump();
			res.set_header("Access-Control-Allow-Origin", "*");
		});
}

void App::run(const char* host, int port)
{
	_srv.listen(host, port);
}

int App::get_integer_param(const char* param_name, const httplib::Params& params)
{
	const auto it = params.find(param_name);
	if (it == params.cend())
		throw std::runtime_error("Param : '" + std::string(param_name) + "' not found");

	int param_value = 0;
	const auto& param_string_value = it->second;
	const auto [_, ec] = std::from_chars(param_string_value.data(), param_string_value.data() + param_string_value.size(), param_value);

	if (ec != std::errc())
		throw std::runtime_error("Param : '" + std::string(param_name) + "' has an invalid format, expected int, got : " + param_string_value);

	return param_value;
}

std::string_view App::get_string_param(const char* param_name, const httplib::Params& params)
{
	const auto it = params.find(param_name);
	if (it == params.cend())
		throw std::runtime_error("Param : '" + std::string(param_name) + "' not found");
	return it->second;
}

PlayerId App::add_player()
{
	std::lock_guard lock(_mutex);
	if (_players.size() > 1000000)
		throw std::runtime_error("Too many players.");

	// Loop to avoid collision
	PlayerId player_id = 0;
	bool inserted = false;
	do
	{
		player_id = get_random_number(0, 1000000);
		inserted = _players.insert(std::make_pair(player_id, PlayerStatus{})).second;
	}
	while (!inserted);

	return player_id;
}

GuessResult App::guess(PlayerId player_id, std::string_view pokemon_name)
{
	std::lock_guard lock(_mutex);
	auto& player = _players.at(player_id);

	if (player.finished)
	{
		return GuessResult{ .success = false, .finished = player.finished, .nb_tries = player.nb_try };
	}
	
	if (compare_non_case_senstive(pokemon_name, _daily_pokemon.name))
	{
		player.finished = true;
		return GuessResult{ .success = true, .finished = player.finished, .nb_tries = player.nb_try };
	}
	else
	{
		++(player.nb_try)
		if (player.nb_try >= 5)
			player.finished = true;
		return GuessResult{ .success = false, .finished = player.finished, .nb_tries = player.nb_try };
	}
}