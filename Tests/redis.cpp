#include <gtest/gtest.h>

#include <DatabaseFactory.h>
#include <Implementations/RedisDatabase.h>
#include <Implementations/RedisTable.h>
#include <Queries/CreateTableQuery.h>
#include <Exceptions/DatabaseException.h>

static std::shared_ptr<database::Database> db;

#ifndef _WIN32
TEST(Redis, CreateDatabase)
{
	ASSERT_NO_THROW(db = database::createDatabase<database::RedisDatabase>("127.0.0.1:8080:password"));
}

TEST(Redis, CreateTable)
{
	ASSERT_NO_THROW
	(
		(database::createTable<database::RedisTable>("", db, ""))
	);
}

TEST(Redis, Set)
{
	const std::unique_ptr<database::Table>& table = db->get("");

	std::vector<std::pair<std::string, database::SqlValue>> values;
	std::vector<uint8_t> bytes;

	for (size_t i = 0; i < 128; i++)
	{
		bytes.push_back(rand() % 256);
	}

	{
		auto& [key, value] = values.emplace_back();

		key = "integer";
		value = 1;
	}

	{
		auto& [key, value] = values.emplace_back();

		key = "double";
		value = 2.2;
	}

	{
		auto& [key, value] = values.emplace_back();

		key = "string";
		value = "first";
	}

	{
		auto& [key, value] = values.emplace_back();

		key = "bool";
		value = true;
	}

	{
		auto& [key, value] = values.emplace_back();

		key = "blob";
		value = bytes;
	}

	for (const auto& [key, value] : values)
	{
		std::vector<database::SqlValue> temp = database::utility::makeSqlValues(key, value);

		ASSERT_NO_THROW(table->execute(temp, "SET"));
	}
}

TEST(Redis, Mset)
{
	const std::unique_ptr<database::Table>& table = db->get("");
	std::vector<database::SqlValue> values = database::utility::makeSqlValues
	(
		"multi_integer",
		10,
		"multi_double",
		20.2,
		"multi_string",
		"second",
		"multi_bool",
		false
	);

	ASSERT_NO_THROW(table->execute(values, "MSET"));
}

TEST(Redis, Get)
{
	const std::unique_ptr<database::Table>& table = db->get("");

	std::vector<database::SqlValue> names = database::utility::makeSqlValues
	(
		"integer",
		"double",
		"string",
		"bool",
		"blob"
	);
	std::vector<database::SqlValue> result;

	for (const database::SqlValue& name : names)
	{
		result.emplace_back(table->execute({ name }, "GET")[0].begin()->second);
	}

	ASSERT_EQ(result[0].get<int64_t>(), 1);
	ASSERT_EQ(result[1].get<double>(), 2.2);
	ASSERT_EQ(result[2].get<std::string>(), "first");
	ASSERT_EQ(result[3].get<bool>(), true);
	ASSERT_EQ(result[4].get<std::string>().size(), 128);
}

TEST(Redis, Mget)
{
	const std::unique_ptr<database::Table>& table = db->get("");

	std::vector<database::SqlValue> names = database::utility::makeSqlValues
	(
		"multi_integer",
		"multi_double",
		"multi_string",
		"multi_bool"
	);
	std::unordered_map<std::string, database::SqlValue> result = table->execute(names, "MGET")[0];

	ASSERT_EQ(result.at("0").get<int64_t>(), 10);
	ASSERT_EQ(result.at("1").get<double>(), 20.2);
	ASSERT_EQ(result.at("2").get<std::string>(), "second");
	ASSERT_EQ(result.at("3").get<bool>(), false);
}
#endif
