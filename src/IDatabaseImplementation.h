#pragma once

#include <string>

#include "IDatabase.h"

namespace database
{
	class IDatabaseImplementation
	{
	public:
		static inline constexpr std::string_view implementationName;
		
	public:
		static IDatabase* createDatabase(std::string_view databaseName);

	public:
		IDatabaseImplementation() = default;

		virtual ~IDatabaseImplementation() = default;
	};
}
