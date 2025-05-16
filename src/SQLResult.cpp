#include "SQLResult.h"

namespace database
{
	void SQLResult::addRow(std::unordered_map<std::string, SQLValue>&& row)
	{
		rows.emplace_back(std::move(row));
	}

	std::unordered_map<std::string, SQLValue>& SQLResult::at(size_t index)
	{
		return rows.at(index);
	}

	const std::unordered_map<std::string, SQLValue>& SQLResult::at(size_t index) const
	{
		return rows.at(index);
	}

	size_t SQLResult::size() const
	{
		return rows.size();
	}

	SQLResult::ValueType::iterator SQLResult::begin()
	{
		return rows.begin();
	}

	SQLResult::ValueType::iterator SQLResult::end()
	{
		return rows.end();
	}

	SQLResult::operator ValueType& ()
	{
		return rows;
	}

	SQLResult::operator const ValueType& () const
	{
		return rows;
	}

	std::unordered_map<std::string, SQLValue>& SQLResult::operator[](size_t index)
	{
		return rows[index];
	}

	const std::unordered_map<std::string, SQLValue>& SQLResult::operator[](size_t index) const
	{
		return rows[index];
	}
}
