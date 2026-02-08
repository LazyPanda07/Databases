#include "Implementations/RedisTable.h"

#include <format>
#include <algorithm>

#include <hiredis/hiredis.h>

#include "Implementations/RedisDatabase.h"
#include "Exceptions/DatabaseException.h"

namespace database
{
	Table* RedisTable::createTable(std::string_view tableName, const CreateTableQuery& query, Database* database)
	{
		static RedisTable instance(tableName, query, database);

		return &instance;
	}

	RedisTable::RedisTable(std::string_view tableName, const CreateTableQuery& query, Database* database) :
		Table(tableName, database)
	{

	}

	SQLResult RedisTable::execute(const IQuery& query, const std::vector<SQLValue>& values)
	{
		SQLResult result;
		redisContext* context = **static_cast<RedisDatabase*>(database);
		std::vector<const char*> argv;
		std::vector<size_t> argvlen;
		std::vector<std::string> temp;

		if (std::string_view queryData = query.getQuery(); queryData.size())
		{
			temp.emplace_back(queryData);
			argvlen.emplace_back(queryData.size());
		}

		for (const SQLValue& value : values)
		{
			std::string argument;

			switch (value->index())
			{
			case 0: // int64_t
				argument = std::to_string(value.get<int64_t>());

				break;

			case 1: // double
				argument = std::to_string(value.get<double>());

				break;

			case 2: // std::string
				argument = value.get<std::string>();

				break;

			case 3: // bool
				argument = std::to_string(value.get<bool>());

				break;

			case 5: // blob
			{
				const std::vector<uint8_t>& blob = value.get<std::vector<uint8_t>>();

				argument = std::string(blob.begin(), blob.end());
			}

			break;

			case 4: // nullptr_t
			default:
				throw exception::DatabaseException(std::format("Wrong index: {}", value->index()));
			}

			argvlen.emplace_back(argument.size());
			temp.emplace_back(std::move(argument));
		}

		argv.reserve(temp.size());

		std::ranges::for_each(temp, [&argv](const std::string& value) { argv.emplace_back(value.data()); });

		redisCommandArgv(context, static_cast<int>(argv.size()), argv.data(), argvlen.data());

		return result;
	}
}
