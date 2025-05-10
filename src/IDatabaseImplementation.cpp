#include "IDatabaseImplementation.h"

#include <stdexcept>

namespace database
{
	IDatabase* IDatabaseImplementation::createDatabase(std::string_view databaseName)
	{
		throw std::runtime_error("No implementation");

		return nullptr;
	}
}
