#include "SQLiteDatabase.h"

namespace database
{
	IDatabase* SQLiteDatabase::createDatabase(std::string_view databaseName)
	{
		return new SQLiteDatabase(databaseName);
	}

	SQLiteDatabase::SQLiteDatabase(std::string_view databaseName) :
		IDatabase(databaseName)
	{

	}
}
