#pragma once

#include <memory>
#include <unordered_map>
#include <mutex>

#include "Table.h"
#include "Queries/CreateTableQuery.h"

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
		const std::unique_ptr<Table>& addTable(Table* table);

	protected:
		std::string databaseName;
		std::string_view databaseImplementationName;
		std::unordered_map<std::string, std::unique_ptr<Table>, StringViewHash, StringViewEqual> tables;
		mutable std::mutex tablesMutex;

	public:
		Database(std::string_view databaseName, std::string_view databaseImplementationName);

		bool contains(std::string_view tableName, Table** outTable = nullptr) const;

		const std::unique_ptr<Table>& get(std::string_view tableName) const;

		std::string_view getDatabaseName() const;

		std::string_view getDatabaseImplementationName() const;

		virtual std::string_view getDatabaseFileName() const = 0;

		virtual ~Database() = default;

		friend Table* createRawTable(std::string_view tableName, const CreateTableQuery& query, Database* database);
		friend const std::unique_ptr<Table>& createTable(std::string_view tableName, const CreateTableQuery& query, std::shared_ptr<Database> database);

		template<std::derived_from<Table> T, std::derived_from<CreateTableQuery> QueryT, typename... Args>
		friend const std::unique_ptr<Table>& createTable(std::string_view tableName, std::shared_ptr<Database> database, Args&&... args);
	};
}
