#pragma once

#include "Table.h"

typedef struct sqlite3 sqlite3;

namespace database
{
	class SQLiteTable : public Table
	{
	public:
		static inline constexpr std::string_view implementationName = "sqlite";

	public:
		static Table* createTable(std::string_view databaseName, Database* database);

	private:
		sqlite3* getConnection() const;

	public:
		SQLiteTable(std::string_view tableName, Database* database);

		SQLResult execute(const std::unique_ptr<IQuery>& query, const std::vector<SQLValue>& values, bool insertTableNameAsFirstArgument) override;

		~SQLiteTable() = default;
	};
}
