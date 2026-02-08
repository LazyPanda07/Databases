#include "Implementations/RedisTable.h"

#include <format>
#include <algorithm>

#include <hiredis/hiredis.h>

#include "Implementations/RedisDatabase.h"
#include "Exceptions/DatabaseException.h"

class RedisReplyDeleter
{
public:
	void operator ()(redisReply* reply) const noexcept;
};

using ReplyPointer = std::unique_ptr<redisReply, RedisReplyDeleter>;

static std::unordered_map<std::string, database::SQLValue> makeRow(const database::SQLValue::ValueType& value);

namespace database
{
	Table* RedisTable::createTable(std::string_view tableName, const CreateTableQuery& query, Database* database)
	{
		static RedisTable instance("", query, database);

		return &instance;
	}

	RedisTable::RedisTable(std::string_view tableName, const CreateTableQuery& query, Database* database) :
		Table(tableName, database)
	{

	}

	SQLResult RedisTable::execute(const IQuery& query, const std::vector<SQLValue>& values)
	{
		// TODO: multi threading

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

		ReplyPointer reply(static_cast<redisReply*>(redisCommandArgv(context, static_cast<int>(argv.size()), argv.data(), argvlen.data())));

		if (!reply)
		{
			throw exception::DatabaseException(std::format("Can't process command {}. Error message: {}", query.getQuery(), context->errstr));
		}
		else if (reply->type == REDIS_REPLY_ERROR)
		{
			throw exception::DatabaseException(std::format("Can't process command {}. Error message: {}", query.getQuery(), reply->str));
		}

		switch (reply->type)
		{
		case REDIS_REPLY_STRING:
		case REDIS_REPLY_STATUS:
		case REDIS_REPLY_BIGNUM: // large number as string
		case REDIS_REPLY_VERB:

			result.addRow(makeRow(reply->str));

			break;

		case REDIS_REPLY_ARRAY:
			// TODO: parse array

			break;

		case REDIS_REPLY_INTEGER:
			result.addRow(makeRow(reply->integer));

			break;

		case REDIS_REPLY_NIL:
			result.addRow(makeRow(nullptr));

			break;

		case REDIS_REPLY_DOUBLE:
			result.addRow(makeRow(reply->dval));

			break;

		case REDIS_REPLY_BOOL:
			result.addRow(makeRow(static_cast<bool>(reply->integer)));

			break;

		case REDIS_REPLY_MAP:
			// TODO: parse map

			break;

		case REDIS_REPLY_SET:
			// TODO: parse set

			break;

		case REDIS_REPLY_ATTR:
		case REDIS_REPLY_PUSH:
		default:
			throw exception::DatabaseException(std::format("Wrong type: {}", reply->type));
		}

		return result;
	}
}

void RedisReplyDeleter::operator ()(redisReply* reply) const noexcept
{
	if (reply)
	{
		freeReplyObject(reply);
	}
}

static std::unordered_map<std::string, database::SQLValue> makeRow(const database::SQLValue::ValueType& value)
{
	return 
		std::unordered_map<std::string, database::SQLValue>
		(
			{
				{ "", database::SQLValue(value) }
			}
		);
}
