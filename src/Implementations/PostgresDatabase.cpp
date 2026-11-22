#include "Implementations/PostgresDatabase.h"

#include <format>

#include <libpq-fe.h>

#include "Exceptions/DatabaseException.h"
#include "Implementations/PostgresTable.h"

namespace database
{
	Database* PostgresDatabase::createDatabase(std::string_view databaseName)
	{
		return new PostgresDatabase(databaseName);
	}

	PostgresDatabase::PostgresDatabase(std::string_view databaseName) :
		Database(databaseName, implementation::postgres),
		connection(nullptr)
	{
		connection = PQconnectdb(databaseName.data());

		if (PQstatus(connection) != CONNECTION_OK)
		{
			throw exception::DatabaseException(std::format("Connection failed: {}", PQerrorMessage(connection)));

			PQfinish(connection);
		}
	}

	PGconn* PostgresDatabase::operator *() const
	{
		return connection;
	}

	std::string_view PostgresDatabase::getDatabaseFileName() const
	{
		return "";
	}

	bool PostgresDatabase::getSupportsTables() const
	{
		return PostgresTable::supportsTables;
	}

	PostgresDatabase::~PostgresDatabase()
	{
		if (connection)
		{
			PQfinish(connection);
		}
	}
}
