#pragma once

#include <utility>
#include <map>
#include <mutex>

#include <network.hpp>
#include <PlayerStatus.hpp>
#include <Pokemon.hpp>

using PlayerId = int;

struct GuessResult
{
	bool success = false;
	bool finished = false;
	int nb_tries = 0;
};

class App
{
	public:
		App();

		void run(const char* host, int port);

	private:
		int get_integer_param(const char* param_name, const httplib::Params& params);
		std::string_view get_string_param(const char* param_name, const httplib::Params& params);
		PlayerId add_player();
		GuessResult guess(PlayerId player_id, std::string_view pokemon_name);

	private:
		Pokemon _daily_pokemon;
		httplib::Server _srv;
		std::map<PlayerId, PlayerStatus> _players;
		std::mutex _mutex;
};