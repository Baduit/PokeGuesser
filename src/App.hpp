#pragma once

#include <charconv>
#include <utility>

#include <nlohmann/json.hpp>

#include <network.hpp>

class App
{
	public:
		App()
		{
			_srv.Get("/start",
				[&](const httplib::Request& req, httplib::Response& res)
				{
					nlohmann::json reply;
					res.body = reply.dump();
					res.set_header("Access-Control-Allow-Origin", "*");
				});

			_srv.Get("/start",
				[&](const httplib::Request& req, httplib::Response& res)
				{
					nlohmann::json reply;
					res.body = reply.dump();
					res.set_header("Access-Control-Allow-Origin", "*");
				});
		}

		void run(const char* host, int port)
		{
			_srv.listen(host, port);
		}

	private:
		int get_integer_param(const char* param_name, const httplib::Params& params)
		{
			auto it = params.find(param_name);
			if (it == params.cend())
				throw std::runtime_error("Param : '" + std::string(param_name) + "' not found");
			int param_value = 0;
			auto& param_string_value = it->second;
			auto [_, ec] = std::from_chars(param_string_value.data(), param_string_value.data() + param_string_value.size(), param_value);
			if (ec != std::errc())
				throw std::runtime_error("Param : '" + std::string(param_name) + "' has an invalid format, expected int, got : " + param_string_value);
			return param_value;
		}

	private:
		httplib::Server _srv;
};