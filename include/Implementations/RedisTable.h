#pragma once

#include "Table.h"

#include "Queries/CreateTableQuery.h"
#include "DatabaseUtility.h"

namespace database
{
	class RedisTable : public Table
	{
	public:
		static inline constexpr std::string_view implementationName = implementation::redis;

	public:
		static Table* createTable(std::string_view tableName, const CreateTableQuery& query, Database* database);

	public:
		RedisTable(std::string_view tableName, const CreateTableQuery& query, Database* database);

		SQLResult execute(const IQuery& query, const std::vector<SQLValue>& values) override;

		~RedisTable() = default;
	};
}
