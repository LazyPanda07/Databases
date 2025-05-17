#include "Table.h"

#include "Database.h"

namespace database
{
	Table::Table(std::string_view tableName, Database* database) :
		tableName(tableName),
		database(database)
	{
		
	}

	const std::string& Table::getTableName() const
	{
		return tableName;
	}
}
