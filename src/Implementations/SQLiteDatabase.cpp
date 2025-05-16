#include "SQLiteDatabase.h"

#include <format>

#include "sqlite3.h"

#include "Exceptions/DatabaseException.h"

namespace database
{
	Database* SQLiteDatabase::createDatabase(std::string_view databaseName)
	{
		return new SQLiteDatabase(databaseName);
	}

	SQLiteDatabase::SQLiteDatabase(std::string_view databaseName) :
		Database(databaseName),
		connection(nullptr)
	{
		if (sqlite3_open(this->getDatabaseFileName().data(), &connection) != SQLITE_OK)
		{
			throw exception::DatabaseException(format("Can't open {}", this->getDatabaseFileName()));
		}
	}

	sqlite3* SQLiteDatabase::operator *() const
	{
		return connection;
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
