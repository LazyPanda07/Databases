#pragma once

#include <string>
#include <memory>
#include <vector>

#include "SQLValue.h"

namespace database
{
	class Database;

	class Table
	{
	public:
		static inline constexpr std::string_view implementationName = "";

	protected:
		std::string tableName;
		Database* database;
		
	public:
		Table(std::string_view tableName, Database* database);

		virtual void execute(const std::vector<SQLValue>& values) = 0;

		const std::string& getTableName() const;

		virtual ~Table() = default;

		friend class Database;
	};
}
