#pragma once

#include "IDatabase.h"
#include "IDatabaseImplementation.h"

namespace database
{
	std::shared_ptr<IDatabase> createDatabase(std::string_view implementationName, std::string_view databaseName);

	IDatabase* createRawDatabase(std::string_view implementationName, std::string_view databaseName);

	template<std::derived_from<IDatabaseImplementation> T>
	std::shared_ptr<IDatabase> createDatabase(std::string_view databaseName);
}

namespace database
{
	template<std::derived_from<IDatabaseImplementation> T>
	std::shared_ptr<IDatabase> createDatabase(std::string_view databaseName)
	{
		return std::shared_ptr<IDatabase>(T::createDatabase(databaseName));
	}
}
