#pragma once

#include "Table.h"

#include "Queries/IQuery.h"
#include "DatabaseUtility.h"

namespace database
{
	class RedisTable : public Table
	{
	public:
		static inline constexpr std::string_view implementationName = implementation::redis;
		static inline constexpr bool supportsTables = false;

	public:
		static Table* createTable(std::string_view tableName, const IQuery& query, Database* database);

	public:
		RedisTable(std::string_view tableName, const IQuery& query, Database* database);

		SqlResult execute(const IQuery& query, const std::vector<SqlValue>& values) override;

		~RedisTable() = default;
	};
}
