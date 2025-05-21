#include "Database.h"

#include <format>

#include "Exceptions/DatabaseException.h"

namespace database
{
	size_t Database::StringViewHash::operator()(std::string_view value) const
	{
		return std::hash<std::string_view>()(value);
	}

	bool Database::StringViewEqual::operator ()(std::string_view left, std::string_view right) const
	{
		return left == right;
	}

	const std::unique_ptr<Table>& Database::addTable(Table* table)
	{
		std::unique_lock<std::mutex> lock(tablesMutex);

		return tables.try_emplace(table->getTableName(), std::unique_ptr<Table>(table)).first->second;
	}

	bool Database::contains(std::string_view tableName, Table* outTable) const
	{
		std::unique_lock<std::mutex> lock(tablesMutex);
		auto it = tables.find(tableName);
		bool result = it != tables.end();

		if (outTable && result)
		{
			outTable = it->second.get();
		}

		return result;
	}

	const std::unique_ptr<Table>& Database::get(std::string_view tableName) const
	{
		std::unique_lock<std::mutex> lock(tablesMutex);
		auto it = tables.find(tableName);

		if (it == tables.end())
		{
			throw exception::DatabaseException(std::format("Can't find {} table in {}", tableName, this->getDatabaseName()));
		}

		return it->second;
	}

	Database::Database(std::string_view databaseName) :
		databaseName(databaseName)
	{

	}

	const std::string& Database::getDatabaseName() const
	{
		return databaseName;
	}
}
