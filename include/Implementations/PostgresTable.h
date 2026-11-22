#pragma once

#include "Table.h"

#include "Queries/IQuery.h"
#include "DatabaseUtility.h"

typedef struct pg_conn PGconn;

namespace database
{
	class PostgresTable : public Table
	{
	public:
		static inline constexpr std::string_view implementationName = implementation::postgres;
		static inline constexpr bool supportsTables = true;

	public:
		static Table* createTable(std::string_view tableName, const IQuery& query, Database* database);

	private:
		PGconn* getConnection() const;

	public:
		PostgresTable(std::string_view tableName, const IQuery& query, Database* database);

		SqlResult execute(const IQuery& query, const std::vector<SqlValue>& values) override;

		~PostgresTable() = default;
	};
}
