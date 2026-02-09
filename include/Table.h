#pragma once

#include <string>
#include <memory>
#include <vector>

#include "SqlValue.h"
#include "SqlResult.h"
#include "Queries/RawQuery.h"

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

		virtual SqlResult execute(const IQuery& query, const std::vector<SqlValue>& values) = 0;

		const std::string& getTableName() const;

		template<std::derived_from<IQuery> T = RawQuery, typename... Args>
		SqlResult execute(const std::vector<SqlValue>& values, Args&&... args);

		virtual ~Table() = default;

		friend class Database;
	};

	template<std::derived_from<IQuery> T, typename... Args>
	SqlResult Table::execute(const std::vector<SqlValue>& values, Args&&... args)
	{
		T query(std::forward<Args>(args)...);

		return this->execute(query, values);
	}
}
