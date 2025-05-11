#pragma once

#include "Table.h"

namespace database
{
	class SQLiteTable : public Table
	{
	public:
		static inline constexpr std::string_view implementationName = "sqlite";

	public:
		static Table* createTable(std::string_view databaseName);

	public:
		SQLiteTable(std::string_view tableName);

		~SQLiteTable() = default;
	};
}
