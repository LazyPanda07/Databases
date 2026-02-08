#pragma once

#include <variant>
#include <string>
#include <vector>
#include <cstdint>

namespace database
{
	template<typename T>
	concept OneOf = std::is_same_v<T, int64_t> || std::is_same_v<T, double> || std::is_same_v<T, std::string> || std::is_same_v<T, bool> || std::is_same_v<T, std::nullptr_t> || std::is_same_v<T, std::vector<uint8_t>>;

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

		const ValueType* operator ->() const;

		template<OneOf T>
		const T& get() const;

		SQLValue() = default;
	};

	template<OneOf T>
	const T& SQLValue::get() const
	{
		return std::get<T>(value);
	}
}
