#include "DatabaseFactory.h"

#include <stdexcept>
#include <format>
#include <array>

#include "Implementations/SQLiteDatabase.h"

static constexpr std::array<std::pair<std::string_view, database::IDatabase* (*)(std::string_view)>, 1> createFunctions =
{
	std::make_pair(database::SQLiteDatabase::implementationName, &database::SQLiteDatabase::createDatabase)
};

namespace database
{
	std::shared_ptr<IDatabase> createDatabase(std::string_view implementationName, std::string_view databaseName)
	{
		auto it = std::ranges::find_if(createFunctions, [implementationName](const auto& value) { return value.first == implementationName; });

		if (it == createFunctions.end())
		{
			throw std::runtime_error(std::format("Wrong implementation name: {}", implementationName));
		}

		return std::shared_ptr<IDatabase>(it->second(databaseName));
	}

	IDatabase* createRawDatabase(std::string_view implementationName, std::string_view databaseName)
	{
		auto it = std::ranges::find_if(createFunctions, [implementationName](const auto& value) { return value.first == implementationName; });

		if (it == createFunctions.end())
		{
			throw std::runtime_error(std::format("Wrong implementation name: {}", implementationName));
		}

		return it->second(databaseName);
	}
}
