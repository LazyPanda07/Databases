#include "Implementations/SQLiteDatabase.h"

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
		if (databaseName.find(":memory:") == std::string_view::npos) // in memory
		{
			std::string fileName = std::format("{}.{}", this->getDatabaseName(), SQLiteDatabase::fileExtension);

			if (sqlite3_open(fileName.data(), &connection) != SQLITE_OK)
			{
				throw exception::DatabaseException(format("Can't open {}", fileName));
			}
		}
		else
		{
			if (sqlite3_open(this->getDatabaseName().data(), &connection) != SQLITE_OK)
			{
				throw exception::DatabaseException(format("Can't open {}", this->getDatabaseName()));
			}
		}
	}

	sqlite3* SQLiteDatabase::operator *() const
	{
		return connection;
	}

	std::string_view SQLiteDatabase::getDatabaseFileName() const
	{
		sqlite3_filename result = sqlite3_db_filename(connection, this->getDatabaseName().data());

		if (result)
		{
			return std::string_view(result);
		}

		return "";
	}

	SQLiteDatabase::~SQLiteDatabase()
	{
		if (connection)
		{
			sqlite3_close(connection);
		}
	}
}
