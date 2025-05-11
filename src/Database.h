#pragma once

#include <memory>
#include <unordered_map>

#include "Table.h"

namespace database
{
	class Database
	{
	public:
		static inline constexpr std::string_view implementationName = "";

	private:
		struct StringViewHash
		{
			using is_transparent = void;

			size_t operator ()(std::string_view value) const;
		};

		struct StringViewEqual
		{
			using is_transparent = void;

			bool operator ()(std::string_view left, std::string_view right) const;
		};

	private:
		void addTable(std::shared_ptr<Table> table);

	protected:
		std::string databaseName;
		std::unordered_map<std::string, std::shared_ptr<Table>, StringViewHash, StringViewEqual> tables;

	public:
		Database(std::string_view databaseName);

		bool contains(std::string_view tableName, Table* outTable = nullptr) const;

		std::shared_ptr<Table> get(std::string_view tableName) const;

		const std::string& getDatabaseName() const;

		virtual std::string getDatabaseFileName() const = 0;

		virtual ~Database() = default;

		friend Table* createRawTable(std::string_view implementationName, std::string_view tableName, Database* database);
		friend std::shared_ptr<Table> createTable(std::string_view implementationName, std::string_view tableName, std::shared_ptr<Database> database);

		template<std::derived_from<Table> T>
		friend std::shared_ptr<Table> createTable(std::string_view tableName, std::shared_ptr<Database> database);
	};
}
