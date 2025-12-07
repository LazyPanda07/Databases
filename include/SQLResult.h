#pragma once

#include <unordered_map>

#include "SQLValue.h"

namespace database
{
	class SQLResult
	{
	public:
		using ValueType = std::vector<std::unordered_map<std::string, SQLValue>>;

	private:
		ValueType rows;

	public:
		SQLResult() = default;

		void addRow(std::unordered_map<std::string, SQLValue>&& row);

		std::unordered_map<std::string, SQLValue>& at(size_t index);

		const std::unordered_map<std::string, SQLValue>& at(size_t index) const;

		size_t size() const;

		ValueType::iterator begin();

		ValueType::iterator end();

		operator ValueType& ();

		operator const ValueType& () const;

		std::unordered_map<std::string, SQLValue>& operator [](size_t index);

		const std::unordered_map<std::string, SQLValue>& operator [](size_t index) const;

		~SQLResult() = default;
	};
}
