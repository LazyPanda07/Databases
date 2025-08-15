#pragma once

#include "Database.h"

#include "DatabaseUtility.h"

typedef struct sqlite3 sqlite3;

namespace database
{
	class SQLiteDatabase final : public Database
	{
	public:
		static inline constexpr std::string_view implementationName = implementation::sqlite;
		static inline constexpr std::string_view fileExtension = implementation::sqlite;

	public:
		static Database* createDatabase(std::string_view databaseName);

	private:
		sqlite3* connection;

	public:
		SQLiteDatabase(std::string_view databaseName);

		sqlite3* operator *() const;

		std::string_view getDatabaseFileName() const override;

		~SQLiteDatabase();
	};
}
