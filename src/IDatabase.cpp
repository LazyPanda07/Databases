#include "IDatabase.h"

namespace database
{
	IDatabase::IDatabase(std::string_view databaseName) :
		databaseName(databaseName)
	{

	}

	const std::string& IDatabase::getDatabaseName() const
	{
		return databaseName;
	}
}
