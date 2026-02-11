#include "Implementations/SqliteDatabase.h"

#include <format>

#include <sqlite3.h>

#include "Exceptions/DatabaseException.h"

namespace database
{
	Database* SqliteDatabase::createDatabase(std::string_view databaseName)
	{
		return new SqliteDatabase(databaseName);
	}

	SqliteDatabase::SqliteDatabase(std::string_view databaseName) :
		Database(databaseName, implementation::sqlite),
		connection(nullptr)
	{
		if (databaseName.find(":memory:") == std::string_view::npos) // in memory
		{
			std::string fileName = std::format("{}.{}", this->getDatabaseName(), SqliteDatabase::fileExtension);

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

	sqlite3* SqliteDatabase::operator *() const
	{
		return connection;
	}

	std::string_view SqliteDatabase::getDatabaseFileName() const
	{
		sqlite3_filename result = sqlite3_db_filename(connection, this->getDatabaseName().data());

		if (result)
		{
			return std::string_view(result);
		}

		return "";
	}

	SqliteDatabase::~SqliteDatabase()
	{
		if (connection)
		{
			sqlite3_close(connection);
		}
	}
}
