#include "SQLiteTable.h"

namespace database
{
	Table* SQLiteTable::createTable(std::string_view databaseName)
	{
		return new SQLiteTable(databaseName);
	}

	SQLiteTable::SQLiteTable(std::string_view tableName) :
		Table(tableName)
	{

	}
}
