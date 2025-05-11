#pragma once

#include "Table.h"

namespace database
{
	class SQLiteTable : public Table
	{
	public:
		static inline constexpr std::string_view implementationName = "sqlite";

	public:
		static Table* createTable(std::string_view databaseName, Database* database);

	public:
		SQLiteTable(std::string_view tableName, Database* database);

		void execute(const std::vector<SQLValue>& values) override;

		~SQLiteTable() = default;
	};
}
