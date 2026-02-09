#pragma once

#include "Table.h"

#include "Queries/IQuery.h"
#include "DatabaseUtility.h"

typedef struct sqlite3 sqlite3;

namespace database
{
	class SqliteTable : public Table
	{
	public:
		static inline constexpr std::string_view implementationName = implementation::sqlite;
		static inline constexpr bool supportsTables = true;

	public:
		static Table* createTable(std::string_view tableName, const IQuery& query, Database* database);

	private:
		sqlite3* getConnection() const;

	public:
		SqliteTable(std::string_view tableName, const IQuery& query, Database* database);

		SqlResult execute(const IQuery& query, const std::vector<SqlValue>& values) override;

		~SqliteTable() = default;
	};
}
