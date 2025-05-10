#pragma once

#include <memory>

#include "ITable.h"

namespace database
{
	class IDatabase
	{
	protected:
		std::string databaseName;

	public:
		IDatabase(std::string_view databaseName);

		const std::string& getDatabaseName() const;

		virtual ~IDatabase() = default;
	};
}
