#include "gtest/gtest.h"

#include "DatabaseFactory.h"
#include "Implementations/SQLiteDatabase.h"
#include "Implementations/SQLiteTable.h"
#include "Queries/CreateTableQuery.h"
#include "Exceptions/DatabaseException.h"

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
			"CREATE TABLE IF NOT EXISTS test_table (id INTEGER PRIMARY KEY AUTOINCREMENT, doubleValue DOUBLE NOT NULL, textValue TEXT NOT NULL UNIQUE, boolValue BOOLEAN NOT NULL, data BLOB)"
		))
	);
}

TEST(SQLite, Insert)
{
	const std::unique_ptr<database::Table>& table = db->get("test_table");
	std::vector<database::SQLValue> values;
	std::vector<uint8_t> bytes;

	values.emplace_back(1.1);
	values.emplace_back("first");
	values.emplace_back(true);
	values.emplace_back(nullptr);

	for (size_t i = 0; i < 128; i++)
	{
		bytes.push_back(rand() % 256);
	}

	table->execute(values, "INSERT INTO test_table (doubleValue, textValue, boolValue, data) VALUES(?, ?, ?, ?)");

	values[0] = 2.2;
	values[1] = "second";
	values[2] = false;
	values[3] = bytes;

	table->execute(values, "INSERT INTO test_table (doubleValue, textValue, boolValue, data) VALUES(?, ?, ?, ?)");

	ASSERT_THROW(table->execute(values, "INSERT INTO test_table (doubleValue, textValue, boolValue, data) VALUES(?, ?, ?, ?)"), database::exception::DatabaseException);
}

TEST(SQLite, Select)
{
	const std::unique_ptr<database::Table>& table = db->get("test_table");
	std::vector<database::SQLValue> values;

	database::SQLResult result = table->execute({}, "SELECT * FROM test_table");

	ASSERT_EQ(result.size(), 2);

	for (const auto& row : result)
	{
		ASSERT_TRUE(std::holds_alternative<int64_t>(*row.at("id")));
		ASSERT_TRUE(std::holds_alternative<double>(*row.at("doubleValue")));
		ASSERT_TRUE(std::holds_alternative<std::string>(*row.at("textValue")));
		ASSERT_TRUE(std::holds_alternative<bool>(*row.at("boolValue")) || std::holds_alternative<int64_t>(*row.at("boolValue")));
		ASSERT_TRUE(std::holds_alternative<nullptr_t>(*row.at("data")) || std::holds_alternative<std::vector<uint8_t>>(*row.at("data")));
	}

	result = table->execute(values, "SELECT id FROM test_table WHERE data IS NOT NULL");

	ASSERT_EQ(result.size(), 1);
	ASSERT_EQ(result[0].at("id").get<int64_t>(), 2);
}

TEST(SQLite, Update)
{
	const std::unique_ptr<database::Table>& table = db->get("test_table");
	std::vector<database::SQLValue> values;

	values.emplace_back(5.5);
	values.emplace_back("modified");
	values.emplace_back(1);

	table->execute(values, "UPDATE test_table SET doubleValue = ?, textValue = ? WHERE id = ?");

	database::SQLResult result = table->execute({ database::SQLValue(1) }, "SELECT doubleValue, textValue FROM test_table WHERE id = ?");

	ASSERT_EQ(result.size(), 1);

	const auto& row = result.at(0);

	ASSERT_FLOAT_EQ(row.at("doubleValue").get<double>(), 5.5);
	ASSERT_EQ(row.at("textValue").get<std::string>(), "modified");
}

TEST(SQLite, Delete)
{
	const std::unique_ptr<database::Table>& table = db->get("test_table");

	table->execute({ database::SQLValue(2.2) }, "DELETE FROM test_table WHERE doubleValue = ?");

	database::SQLResult result = table->execute({}, "SELECT * FROM test_table");

	ASSERT_EQ(result.size(), 1);
}

TEST(SQLite, Contains)
{
	database::Table* table = nullptr;

	ASSERT_TRUE(db->contains("test_table", &table));
	ASSERT_NE(table, nullptr);
}
