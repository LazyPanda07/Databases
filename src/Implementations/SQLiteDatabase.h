#pragma once

#include "IDatabaseImplementation.h"

typedef struct sqlite3 sqlite3;

namespace database
{
	class SQLiteDatabase final : 
		public IDatabase,
		public IDatabaseImplementation
	{
	public:
		static inline constexpr std::string_view implementationName = "sqlite";
		static inline constexpr std::string_view fileExtension = "sqlite";

	public:
		static IDatabase* createDatabase(std::string_view databaseName);

	private:
		sqlite3* connection;

	public:
		SQLiteDatabase(std::string_view databaseName);

		std::string getDatabaseFileName() const override;

		~SQLiteDatabase();
	};
}
