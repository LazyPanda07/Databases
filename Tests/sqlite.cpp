#include "gtest/gtest.h"

#include "DatabaseFactory.h"
#include "Implementations/SQLiteDatabase.h"
#include "Implementations/SQLiteTable.h"
#include "Queries/CreateTableQuery.h"

static std::shared_ptr<database::Database> db;

TEST(SQLite, CreateDatabase)
{
	ASSERT_NO_THROW(db = database::createDatabase<database::SQLiteDatabase>("test"));
}

TEST(SQLite, CreateTable)
{
	ASSERT_NO_THROW
	(
		(database::createTable<database::SQLiteTable, database::CreateTableQuery>(
			"test_table",
			db,
			"CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY AUTOINCREMENT, userName TEXT NOT NULL UNIQUE, uuid TEXT NOT NULL UNIQUE, online BOOLEAN NOT NULL)"
		))
	);
}
