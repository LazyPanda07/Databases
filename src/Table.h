#pragma once

#include <string>

namespace database
{
	class Table
	{
	public:
		static inline constexpr std::string_view implementationName = "";

	protected:
		std::string tableName;
		
	public:
		Table(std::string_view tableName);

		const std::string& getTableName() const;

		virtual ~Table() = default;
	};
}
