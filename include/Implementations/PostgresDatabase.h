#pragma once

#include "Database.h"

#include "DatabaseUtility.h"

typedef struct pg_conn PGconn;

namespace database
{
	class PostgresDatabase final : public Database
	{
	public:
		static inline constexpr std::string_view implementationName = implementation::postgres;
		static inline constexpr std::string_view fileExtension = implementation::postgres;

	public:
		static Database* createDatabase(std::string_view databaseName);

	private:
		PGconn* connection;

	public:
		PostgresDatabase(std::string_view databaseName);

		PGconn* operator *() const;

		std::string_view getDatabaseFileName() const override;

		bool getSupportsTables() const override;

		~PostgresDatabase();
	};
}
