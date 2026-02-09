#pragma once

#include <unordered_map>

#include "SqlValue.h"

namespace database
{
	class SqlResult
	{
	public:
		using ValueType = std::vector<std::unordered_map<std::string, SqlValue>>;

	private:
		ValueType rows;

	public:
		SqlResult() = default;

		void addRow(std::unordered_map<std::string, SqlValue>&& row);

		std::unordered_map<std::string, SqlValue>& at(size_t index);

		const std::unordered_map<std::string, SqlValue>& at(size_t index) const;

		size_t size() const;

		ValueType::iterator begin();

		ValueType::iterator end();

		operator ValueType& ();

		operator const ValueType& () const;

		std::unordered_map<std::string, SqlValue>& operator [](size_t index);

		const std::unordered_map<std::string, SqlValue>& operator [](size_t index) const;

		~SqlResult() = default;
	};
}
