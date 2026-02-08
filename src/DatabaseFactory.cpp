#include "DatabaseFactory.h"

#include <stdexcept>
#include <format>
#include <array>

#include "Implementations/SQLiteDatabase.h"
#include "Implementations/RedisDatabase.h"

#include "Implementations/SQLiteTable.h"
#include "Implementations/RedisTable.h"

static constexpr std::array<std::pair<std::string_view, database::Database* (*)(std::string_view)>, 2> createDatabaseFunctions =
{
	std::make_pair(database::SQLiteDatabase::implementationName, &database::SQLiteDatabase::createDatabase),
	std::make_pair(database::RedisDatabase::implementationName, &database::RedisDatabase::createDatabase)
};

static constexpr std::array<std::pair<std::string_view, database::Table* (*)(std::string_view, const database::CreateTableQuery&, database::Database* database)>, 2> createTableFunctions =
{
	std::make_pair(database::SQLiteTable::implementationName, &database::SQLiteTable::createTable),
	std::make_pair(database::RedisTable::implementationName, &database::RedisTable::createTable)
};

namespace database
{
	std::shared_ptr<Database> createDatabase(std::string_view implementationName, std::string_view databaseName)
	{
		return std::shared_ptr<Database>(createRawDatabase(implementationName, databaseName));
	}

	Database* createRawDatabase(std::string_view implementationName, std::string_view databaseName)
	{
		auto it = std::ranges::find_if(createDatabaseFunctions, [implementationName](const auto& value) { return value.first == implementationName; });

		if (it == createDatabaseFunctions.end())
		{
			throw std::runtime_error(std::format("Wrong implementation name: {}", implementationName));
		}

		return it->second(databaseName);
	}

	const std::unique_ptr<Table>& createTable(std::string_view implementationName, std::string_view tableName, const CreateTableQuery& query, std::shared_ptr<Database> database)
	{
		if (database->contains(tableName))
		{
			return database->get(tableName);
		}

		auto it = std::ranges::find_if(createTableFunctions, [implementationName](const auto& value) { return value.first == implementationName; });

		if (it == createTableFunctions.end())
		{
			throw std::runtime_error(std::format("Wrong implementation name: {}", implementationName));
		}

		return database->addTable(it->second(tableName, query, database.get()));
	}

	Table* createRawTable(std::string_view implementationName, std::string_view tableName, const CreateTableQuery& query, Database* database)
	{
		if (Table* temp = nullptr; database->contains(tableName, &temp))
		{
			return temp;
		}

		auto it = std::ranges::find_if(createTableFunctions, [implementationName](const auto& value) { return value.first == implementationName; });

		if (it == createTableFunctions.end())
		{
			throw std::runtime_error(std::format("Wrong implementation name: {}", implementationName));
		}

		return database->addTable(it->second(tableName, query, database)).get();
	}
}
