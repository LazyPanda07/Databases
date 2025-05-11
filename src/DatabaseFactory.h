#pragma once

#include "Database.h"

namespace database
{
	std::shared_ptr<Database> createDatabase(std::string_view implementationName, std::string_view databaseName);

	Database* createRawDatabase(std::string_view implementationName, std::string_view databaseName);

	const std::unique_ptr<Table>& createTable(std::string_view implementationName, std::string_view tableName, std::shared_ptr<Database> database);

	Table* createRawTable(std::string_view implementationName, std::string_view tableName, Database* database);

	template<std::derived_from<Database> T>
	std::shared_ptr<Database> createDatabase(std::string_view databaseName);

	template<std::derived_from<Table> T>
	const std::unique_ptr<Table>& createTable(std::string_view tableName, std::shared_ptr<Database> database);
}

namespace database
{
	template<std::derived_from<Database> T>
	std::shared_ptr<Database> createDatabase(std::string_view databaseName)
	{
		return std::shared_ptr<Database>(T::createDatabase(databaseName));
	}

	template<std::derived_from<Table> T>
	const std::unique_ptr<Table>& createTable(std::string_view tableName, std::shared_ptr<Database> database)
	{
		if (database->contains(tableName))
		{
			return database->get(tableName);
		}

		return database->addTable(T::createTable(tableName, database.get()));
	}
}
