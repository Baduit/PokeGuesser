#pragma once

#include <mutex>

#include <Messages.hpp>

class AppLogic
{
	public:

	auto operator()(request::Start) -> reply::Dummy
	{
		return {};
	}

	auto operator()(request::Guess) -> reply::Dummy
	{
		return {};
	}

	private:

};