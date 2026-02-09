#pragma once

#include <variant>
#include <string>
#include <vector>
#include <format>
#include <cstdint>

namespace database
{
	template<typename T>
	concept OneOf = std::is_same_v<T, int64_t> || std::is_same_v<T, double> || std::is_same_v<T, std::string> || std::is_same_v<T, bool> || std::is_same_v<T, std::nullptr_t> || std::is_same_v<T, std::vector<uint8_t>>;

	class SqlValue
	{
	public:
		using ValueType = std::variant<int64_t, double, std::string, bool, std::nullptr_t, std::vector<uint8_t>>;

	private:
		ValueType value;

	public:
		template<typename... Args>
		SqlValue(Args&&... args) requires std::constructible_from<ValueType, Args...>;

		SqlValue(size_t value);

		const ValueType& operator *() const;

		const ValueType* operator ->() const;

		template<OneOf T>
		T get() const;

		template<OneOf T>
		const T& get() const requires(std::same_as<T, std::string> || std::same_as<T, std::vector<uint8_t>>);

		SqlValue() = default;
	};

}

namespace database
{
	template<typename... Args>
	SqlValue::SqlValue(Args&&... args) requires std::constructible_from<ValueType, Args...> : value(std::forward<Args>(args)...)
	{

	}

	template<OneOf T>
	T SqlValue::get() const
	{
		if constexpr (std::same_as<bool, T>)
		{
			if (std::holds_alternative<int64_t>(value))
			{
				return std::get<int64_t>(value) == 1;
			}
		}

		return std::get<T>(value);
	}

	template<OneOf T>
	const T& SqlValue::get() const requires(std::same_as<T, std::string> || std::same_as<T, std::vector<uint8_t>>)
	{
		return std::get<T>(value);
	}
}
