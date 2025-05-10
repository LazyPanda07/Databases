#include "SQLiteDatabase.h"

#include <format>

#include "sqlite3.h"

#include "Exceptions/DatabaseException.h"

namespace database
{
	IDatabase* SQLiteDatabase::createDatabase(std::string_view databaseName)
	{
		return new SQLiteDatabase(databaseName);
	}

	SQLiteDatabase::SQLiteDatabase(std::string_view databaseName) :
		IDatabase(databaseName),
		connection(nullptr)
	{
		if (sqlite3_open(this->getDatabaseFileName().data(), &connection) != SQLITE_OK)
		{
			throw exception::DatabaseException(format("Can't open {}", this->getDatabaseFileName()));
		}
	}

	std::string SQLiteDatabase::getDatabaseFileName() const
	{
		return std::format("{}.{}", this->getDatabaseName(), SQLiteDatabase::fileExtension);
	}

	SQLiteDatabase::~SQLiteDatabase()
	{
		if (connection)
		{
			sqlite3_close(connection);
		}
	}
}
