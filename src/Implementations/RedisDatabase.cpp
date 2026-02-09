#include "Implementations/RedisDatabase.h"

#include <format>
#include <sstream>
#include <optional>

#include <hiredis/hiredis.h>

#include "Exceptions/DatabaseException.h"

namespace database
{
	Database* RedisDatabase::createDatabase(std::string_view connectString)
	{
		return new RedisDatabase(connectString);
	}

	RedisDatabase::RedisDatabase(std::string_view connectString) :
		Database(connectString),
		context(nullptr)
	{
		std::string temp(connectString);
		std::istringstream stream(temp);
		std::vector<std::string> arguments;

		while (std::getline(stream, temp, ':'))
		{
			arguments.emplace_back(temp);
		}

		if (arguments.size() == 1 || arguments.size() > 4)
		{
			throw exception::DatabaseException(std::format("Wrong format connect string: {}", connectString));
		}

		std::string_view ip(arguments[0]);
		int port = std::stoi(arguments[1]);

		context = redisConnect(ip.data(), port);

		if (!context)
		{
			throw exception::DatabaseException(std::format("Can't connect to Redis server with {}:{}", ip, port));
		}

		if (arguments.size() > 2)
		{
			redisReply* reply = nullptr;
			std::optional<std::string> errorMessage;

			if (arguments.size() == 3)
			{
				reply = static_cast<redisReply*>(redisCommand(context, "AUTH %s", arguments[2].data()));
			}
			else
			{
				reply = static_cast<redisReply*>(redisCommand(context, "AUTH %s %s", arguments[2].data(), arguments[3].data()));
			}

			if (!reply)
			{
				errorMessage = context->errstr;
			}
			else if (reply->type == REDIS_REPLY_ERROR)
			{
				errorMessage = reply->str;

				freeReplyObject(reply);
			}

			if (errorMessage)
			{
				redisFree(context);

				throw exception::DatabaseException(*errorMessage);
			}
		}
	}

	std::mutex& RedisDatabase::getContextMutex()
	{
		return contextMutex;
	}

	redisContext* RedisDatabase::operator *() const
	{
		return context;
	}

	std::string_view RedisDatabase::getDatabaseFileName() const
	{
		return "";
	}

	RedisDatabase::~RedisDatabase()
	{
		if (context)
		{
			redisFree(context);

			context = nullptr;
		}
	}
}
