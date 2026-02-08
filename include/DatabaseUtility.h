#pragma once

#include <string_view>

namespace database
{
	namespace utility
	{
		constexpr std::string_view getVersion();
	}

	namespace implementation
	{
		constexpr std::string_view sqlite = "sqlite";
		constexpr std::string_view redis = "redis";
	}
}
