#pragma once

#include <string>
#include <memory>
#include <vector>

#include "SQLValue.h"
#include "SQLResult.h"
#include "Queries/IQuery.h"

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

		virtual SQLResult execute(const std::unique_ptr<IQuery>& query, const std::vector<SQLValue>& values, bool insertTableNameAsFirstArgument = true) = 0;

		const std::string& getTableName() const;

		template<std::derived_from<IQuery> T, typename... Args>
		SQLResult execute(const std::vector<SQLValue>& values, bool insertTableNameAsFirstArgument, Args&&... args);

		virtual ~Table() = default;

		friend class Database;
	};

	template<std::derived_from<IQuery> T, typename... Args>
	SQLResult Table::execute(const std::vector<SQLValue>& values, bool insertTableNameAsFirstArgument, Args&&... args)
	{
		std::unique_ptr<T> query = std::make_unique<T>(std::forward<Args>(args)...);

		this->execute(query, values, insertTableNameAsFirstArgument);
	}
}
