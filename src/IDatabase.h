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

		virtual std::string getDatabaseFileName() const = 0;

		virtual ~IDatabase() = default;
	};
}
