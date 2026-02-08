#include "Implementations/RedisDatabase.h"

#include <format>
#include <sstream>

#include <hiredis/hiredis.h>

#include "Exceptions/DatabaseException.h"

namespace database
{
	Database* RedisDatabase::createDatabase(std::string_view ipAndPort)
	{
		static RedisDatabase instance(ipAndPort);

		return &instance;
	}

	RedisDatabase::RedisDatabase(std::string_view ipAndPort) :
		Database(ipAndPort),
		context(nullptr)
	{
		size_t splitIndex = ipAndPort.find(':');

		if (splitIndex == std::string_view::npos)
		{
			throw exception::DatabaseException(std::format("Wrong format for ip and port: {}", ipAndPort));
		}

		std::string ip(ipAndPort.data(), ipAndPort.data() + splitIndex);
		int port = std::stoi(std::string(ipAndPort.data() + splitIndex + 1));

		context = redisConnect(ip.data(), port);

		if (!context)
		{
			throw exception::DatabaseException(std::format("Can't connect to Redis server with {}:{}", ip, port));
		}
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
