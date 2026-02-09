#pragma once

#include "Database.h"
#include "Queries/CreateTableQuery.h"
#include "Queries/RawQuery.h"

namespace database
{
	std::shared_ptr<Database> createDatabase(std::string_view implementationName, std::string_view databaseName);

	Database* createRawDatabase(std::string_view implementationName, std::string_view databaseName);

	const std::unique_ptr<Table>& createTable(std::string_view implementationName, std::string_view tableName, const CreateTableQuery& query, std::shared_ptr<Database> database);

	Table* createRawTable(std::string_view implementationName, std::string_view tableName, const CreateTableQuery& query, Database* database);

	template<std::derived_from<Database> T>
	std::shared_ptr<Database> createDatabase(std::string_view databaseName);

	template<std::derived_from<Table> T, std::derived_from<CreateTableQuery> QueryT, typename... Args>
	const std::unique_ptr<Table>& createTable(std::string_view tableName, std::shared_ptr<Database> database, Args&&... args);
}

namespace database
{
	template<std::derived_from<Database> T>
	std::shared_ptr<Database> createDatabase(std::string_view databaseName)
	{
		return std::shared_ptr<Database>(T::createDatabase(databaseName));
	}

	template<std::derived_from<Table> T, std::derived_from<CreateTableQuery> QueryT, typename... Args>
	const std::unique_ptr<Table>& createTable(std::string_view tableName, std::shared_ptr<Database> database, Args&&... args)
	{
		if (database->contains(tableName))
		{
			return database->get(tableName);
		}

		if constexpr (T::supportsTables)
		{
			QueryT query(std::forward<Args>(args)...);

			return database->addTable(T::createTable(tableName, query, database.get()));
		}

		return database->addTable(T::createTable(tableName, RawQuery(""), database.get()));
	}
}
