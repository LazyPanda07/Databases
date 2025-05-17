#include "CreateTableQuery.h"

#include <algorithm>
#include <iterator>
#include <format>

#include "Exceptions/DatabaseException.h"

namespace database
{
	CreateTableQuery::CreateTableQuery(std::string_view query)
	{
		std::string temp;

		std::transform(query.begin(), query.end(), std::back_inserter(temp), [](char c) { return std::toupper(c); });

		if (temp.find("CREATE TABLE") == std::string::npos)
		{
			throw exception::DatabaseException(std::format("Can't find create table query: {}", query));
		}

		this->query = query;
	}

	std::string_view CreateTableQuery::getQuery() const
	{
		return query;
	}
}
