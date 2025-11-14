#pragma once

#include <string>

namespace database
{
	class IQuery
	{
	public:
		IQuery() = default;

		virtual std::string_view getQuery() const = 0;

		virtual ~IQuery() = default;
	};
}
