#include "SQLiteTable.h"

namespace database
{
	Table* SQLiteTable::createTable(std::string_view databaseName, Database* database)
	{
		return new SQLiteTable(databaseName, database);
	}

	SQLiteTable::SQLiteTable(std::string_view tableName, Database* database) :
		Table(tableName, database)
	{

	}

	void SQLiteTable::execute(const std::vector<SQLValue>& values)
	{

	}
}
