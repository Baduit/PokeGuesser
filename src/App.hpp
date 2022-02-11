#pragma once

#include <tuple>
#include <type_traits>
#include <charconv>
#include <utility>

#include <network.hpp>

#include <AppLogic.hpp>
#include <Route.hpp>
#include <Messages.hpp>


template <typename RequestType>
RequestType from_string(const std::string& str)
{
	if constexpr (std::is_empty_v<RequestType>)
	{
		return RequestType{};
	}
	else
	{
		return RequestType(nlohmann::json::parse(str));
	}
}

template <typename Tuple, typename Cb>
void foreach(Tuple&& t, Cb&& cb)
{
	std::apply(
		[&]<typename... T>(T&&... element)
		{
			(cb(std::forward<T>(element)), ...);
		}, std::forward<Tuple>(t));
}

class App
{
	private:

		template <typename R>
		void add_route(const R& route)
		{
			_srv.Post(route.path,
				[&](const httplib::Request& req, httplib::Response& res)
				{
					using RequestType = typename R::Request;
					res.body = reply::to_string(_app_logic(from_string<RequestType>(req.body)));
					res.set_header("Access-Control-Allow-Origin", "*");
				});
		}

	public:
		App()
		{
			const auto routes = std::tuple
				(
					Route<request::Start, reply::Dummy> { "/start" },
					Route<request::Guess, reply::Dummy> { "/guess" }
				);

			foreach(routes,
				[&](const auto& r)
				{
					add_route(r);
				});
				_srv.set_mount_point("/", "./public");
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
		AppLogic _app_logic;
		httplib::Server _srv;
};