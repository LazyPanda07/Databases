#pragma once

#include <variant>
#include <string>
#include <vector>

namespace database
{
	class SQLValue
	{
	public:
		using ValueType = std::variant<int64_t, double, std::string, bool, std::nullptr_t, std::vector<uint8_t>>;

	private:
		ValueType value;

	public:
		SQLValue(const ValueType& value);

		SQLValue& operator = (const ValueType& value);

		SQLValue& operator = (ValueType&& value) noexcept;

		const ValueType& operator *() const;

		SQLValue() = default;
	};
}
