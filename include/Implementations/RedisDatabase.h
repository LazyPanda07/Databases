#pragma once

#include "Database.h"

#include "DatabaseUtility.h"

typedef struct redisContext redisContext;

namespace database
{
	class RedisDatabase final : public Database
	{
	public:
		static inline constexpr std::string_view implementationName = implementation::redis;
		static inline constexpr std::string_view fileExtension = "";

	public:
		static Database* createDatabase(std::string_view connectString);

	private:
		redisContext* context;

	public:
		/**
		 * @brief Constructs a RedisDatabase object configured to connect to a Redis server at the specified address and port.
		 * @param ipAndPort A string view containing the server address and port, typically in the form "host:port" (for example "127.0.0.1:6379" or "redis.example.com:6379").
		 */
		RedisDatabase(std::string_view connectString);

		redisContext* operator *() const;

		std::string_view getDatabaseFileName() const override;

		~RedisDatabase();
	};
}
