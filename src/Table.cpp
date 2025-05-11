#include "Table.h"

#include "Database.h"

namespace database
{
	Table::Table(std::string_view tableName) :
		tableName(tableName)
	{

	}

	const std::string& Table::getTableName() const
	{
		return tableName;
	}
}
