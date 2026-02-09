#include "SqlResult.h"

namespace database
{
	void SqlResult::addRow(std::unordered_map<std::string, SqlValue>&& row)
	{
		rows.emplace_back(std::move(row));
	}

	std::unordered_map<std::string, SqlValue>& SqlResult::at(size_t index)
	{
		return rows.at(index);
	}

	const std::unordered_map<std::string, SqlValue>& SqlResult::at(size_t index) const
	{
		return rows.at(index);
	}

	size_t SqlResult::size() const
	{
		return rows.size();
	}

	SqlResult::ValueType::iterator SqlResult::begin()
	{
		return rows.begin();
	}

	SqlResult::ValueType::iterator SqlResult::end()
	{
		return rows.end();
	}

	SqlResult::operator ValueType& ()
	{
		return rows;
	}

	SqlResult::operator const ValueType& () const
	{
		return rows;
	}

	std::unordered_map<std::string, SqlValue>& SqlResult::operator [](size_t index)
	{
		return rows[index];
	}

	const std::unordered_map<std::string, SqlValue>& SqlResult::operator [](size_t index) const
	{
		return rows[index];
	}
}
