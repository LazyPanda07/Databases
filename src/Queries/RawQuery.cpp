#include "RawQuery.h"

namespace database
{
	RawQuery::RawQuery(std::string_view query) :
		query(query)
	{

	}
	std::string_view RawQuery::getQuery() const
	{
		return query;
	}
}
