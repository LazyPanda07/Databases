#pragma once

#include "Table.h"

#include "Queries/CreateTableQuery.h"
#include "DatabaseUtility.h"

typedef struct sqlite3 sqlite3;

namespace database
{
	class SQLiteTable : public Table
	{
	public:
		static inline constexpr std::string_view implementationName = implementation::sqlite;

	public:
		static Table* createTable(std::string_view databaseName, const CreateTableQuery& query, Database* database);

	private:
		sqlite3* getConnection() const;

	public:
		SQLiteTable(std::string_view tableName, const CreateTableQuery& query, Database* database);

		SQLResult execute(const IQuery& query, const std::vector<SQLValue>& values) override;

		~SQLiteTable() = default;
	};
}
