#pragma once

#include <variant>
#include <string>

namespace database
{
	class SQLValue
	{
	public:
		using ValueType = std::variant<int64_t, double, std::string, bool, std::nullptr_t>;

	private:
		ValueType value;

	public:
		SQLValue(const ValueType& value);

		SQLValue() = default;
	};
}
