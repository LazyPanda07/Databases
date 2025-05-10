#pragma once

#include "IDatabaseImplementation.h"

namespace database
{
	class SQLiteDatabase final : 
		public IDatabase,
		public IDatabaseImplementation
	{
	public:
		static inline constexpr std::string_view implementationName = "sqlite";

	public:
		static IDatabase* createDatabase(std::string_view databaseName);

	public:
		SQLiteDatabase(std::string_view databaseName);

		~SQLiteDatabase() = default;
	};
}
