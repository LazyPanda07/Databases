#pragma once

#include <string_view>
#include <vector>

#include "SqlValue.h"

namespace database
{
	namespace utility
	{
		constexpr std::string_view getVersion();

		template<typename... Args>
		std::vector<SqlValue> makeSqlValues(Args&&... args);
	}

	namespace implementation
	{
		constexpr std::string_view sqlite = "sqlite";
		constexpr std::string_view redis = "redis";
	}
}

namespace database::utility
{
	template<typename... Args>
	std::vector<SqlValue> makeSqlValues(Args&&... args)
	{
		std::vector<SqlValue> result({ std::forward<Args>(args)... });

		return result;
	}
}
