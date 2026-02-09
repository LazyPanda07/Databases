#include "Implementations/RedisTable.h"

#include <format>
#include <algorithm>
#include <charconv>
#include <any>

#include <hiredis/hiredis.h>

#include "Implementations/RedisDatabase.h"
#include "Exceptions/DatabaseException.h"

class RedisReplyDeleter
{
public:
	void operator ()(redisReply* reply) const noexcept;
};

using ReplyPointer = std::unique_ptr<redisReply, RedisReplyDeleter>;

static database::SqlValue::ValueType parseCommonValueType(const redisReply& reply);

template<typename T>
static bool isType(std::string_view value, std::any& result) requires (std::integral<T> || std::floating_point<T>);

namespace database
{
	Table* RedisTable::createTable(std::string_view tableName, const IQuery& query, Database* database)
	{
		return new RedisTable("", query, database);
	}

	RedisTable::RedisTable(std::string_view tableName, const IQuery& query, Database* database) :
		Table(tableName, database)
	{

	}

	SqlResult RedisTable::execute(const IQuery& query, const std::vector<SqlValue>& values)
	{
		// TODO: multi threading

		static std::vector<std::string> indices;

		SqlResult result;
		redisContext* context = **static_cast<RedisDatabase*>(database);
		std::vector<const char*> argv;
		std::vector<size_t> argvlen;
		std::vector<std::string> temp;

		if (std::string_view queryData = query.getQuery(); queryData.size())
		{
			temp.emplace_back(queryData);
			argvlen.emplace_back(queryData.size());
		}

		for (const SqlValue& value : values)
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

		std::unordered_map<std::string, SqlValue> row;

		if (reply->type == REDIS_REPLY_ARRAY || reply->type == REDIS_REPLY_SET)
		{
			if (indices.size() < reply->elements)
			{
				for (size_t i = indices.size(); i < reply->elements; i++)
				{
					indices.emplace_back(std::to_string(i));
				}
			}

			for (size_t i = 0; i < reply->elements; i++)
			{
				row.emplace(indices[i], parseCommonValueType(*reply->element[i]));
			}
		}
		else if (reply->type == REDIS_REPLY_MAP)
		{
			for (size_t i = 0; i < reply->elements; i += 2)
			{
				row.emplace
				(
					std::get<std::string>(parseCommonValueType(*reply->element[i])),
					parseCommonValueType(*reply->element[i + 1])
				);
			}
		}
		else
		{
			row.emplace("", parseCommonValueType(*reply));
		}

		result.addRow(std::move(row));

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

database::SqlValue::ValueType parseCommonValueType(const redisReply& reply)
{
	std::any result;

	switch (reply.type)
	{
	case REDIS_REPLY_STRING:
	case REDIS_REPLY_STATUS:
	case REDIS_REPLY_BIGNUM: // large number as string
	case REDIS_REPLY_VERB:

		if (isType<int64_t>(reply.str, result))
		{
			return std::any_cast<int64_t>(result);
		}
		else if (isType<double>(reply.str, result))
		{
			return std::any_cast<double>(result);
		}

		return std::string(reply.str, reply.len);

	case REDIS_REPLY_INTEGER:
		return reply.integer;

	case REDIS_REPLY_NIL:
		return nullptr;

	case REDIS_REPLY_DOUBLE:
		return reply.dval;

	case REDIS_REPLY_BOOL:
		return static_cast<bool>(reply.integer);

	case REDIS_REPLY_ATTR:
	case REDIS_REPLY_PUSH:
	default:
		throw database::exception::DatabaseException(std::format("Wrong type: {} in {}", reply.type, __func__));
	}

	return nullptr;
}

template<typename T>
bool isType(std::string_view value, std::any& result) requires (std::integral<T> || std::floating_point<T>)
{
	T temp{};

	auto state = std::from_chars(value.data(), value.data() + value.size(), temp);

	result = temp;

	return state.ec == std::errc() && state.ptr == value.data() + value.size();
}
