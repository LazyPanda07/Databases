#pragma once

#include <string>

namespace database
{
	class Query
	{
	public:
		Query() = default;

		virtual std::string_view getQuery() const = 0;

		virtual ~Query() = default;
	};
}
