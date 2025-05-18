#include "SQLiteTable.h"

#include <format>

#include "sqlite3.h"

#include "Implementations/SQLiteDatabase.h"
#include "Exceptions/DatabaseException.h"

namespace database
{
	sqlite3* SQLiteTable::getConnection() const
	{
		return **static_cast<SQLiteDatabase*>(database);
	}

	Table* SQLiteTable::createTable(std::string_view databaseName, const CreateTableQuery& query, Database* database)
	{
		return new SQLiteTable(databaseName, query, database);
	}

	SQLiteTable::SQLiteTable(std::string_view tableName, const CreateTableQuery& query, Database* database) :
		Table(tableName, database)
	{
		sqlite3* connection = this->getConnection();
		char* errorMessage = nullptr;

		if (sqlite3_exec(connection, query.getQuery().data(), nullptr, nullptr, &errorMessage) != SQLITE_OK)
		{
			exception::DatabaseException exception(errorMessage);

			sqlite3_free(errorMessage);

			throw exception;
		}
	}

	SQLResult SQLiteTable::execute(const IQuery& query, const std::vector<SQLValue>& values, bool insertTableNameAsFirstArgument)
	{
		sqlite3* connection = this->getConnection();
		sqlite3_stmt* statement;
		int responseCode;

		if (responseCode = sqlite3_prepare_v2(connection, query.getQuery().data(), -1, &statement, nullptr); responseCode != SQLITE_OK)
		{
			throw exception::DatabaseException(sqlite3_errmsg(connection));
		}

		auto deleter = [](sqlite3_stmt* ptr)
			{
				printf("deleter\n");

				sqlite3_finalize(ptr);
			};
		std::unique_ptr<sqlite3_stmt, decltype(deleter)> finalizer(statement, deleter);

		int currentIndex = 1;
		SQLResult result;

		if (insertTableNameAsFirstArgument)
		{
			sqlite3_bind_text(statement, currentIndex++, this->getTableName().data(), -1, nullptr);
		}

		for (size_t i = 0; i < values.size(); i++)
		{
			const SQLValue::ValueType& value = *values[i];

			switch (value.index())
			{
			case 0: // int64_t
				sqlite3_bind_int64(statement, currentIndex, std::get<int64_t>(value));

				break;

			case 1: // double
				sqlite3_bind_double(statement, currentIndex, std::get<double>(value));

				break;

			case 2: // std::string
				sqlite3_bind_text(statement, currentIndex, std::get<std::string>(value).data(), -1, SQLITE_STATIC);

				break;

			case 3: // bool
				sqlite3_bind_int(statement, currentIndex, std::get<bool>(value));

				break;

			case 4: // nullptr_t
				sqlite3_bind_null(statement, currentIndex);

				break;

			case 5: // blob
			{
				const std::vector<uint8_t>& blob = std::get<std::vector<uint8_t>>(value);

				sqlite3_bind_blob(statement, currentIndex, blob.data(), static_cast<int>(blob.size()), SQLITE_STATIC);
			}

			break;

			default:
				throw exception::DatabaseException(std::format("Wrong index: {}", value.index()));
			}

			currentIndex++;
		}

		while ((responseCode = sqlite3_step(statement)) != SQLITE_DONE)
		{
			switch (responseCode)
			{
			case SQLITE_ROW:
			{
				int cols = sqlite3_column_count(statement);
				std::unordered_map<std::string, SQLValue> column;

				for (int i = 0; i < cols; i++)
				{
					const char* columnName = sqlite3_column_name(statement, i);
					SQLValue value;

					switch (sqlite3_column_type(statement, i))
					{
					case SQLITE_INTEGER:
						value = sqlite3_column_int64(statement, i);

						break;

					case SQLITE_FLOAT:
						value = sqlite3_column_double(statement, i);

						break;

					case SQLITE_TEXT:
						value = reinterpret_cast<const char*>(sqlite3_column_text(statement, i));

						break;

					case SQLITE_BLOB: 
					{
						const uint8_t* blobData = static_cast<const uint8_t*>(sqlite3_column_blob(statement, i));
						int blobSize = sqlite3_column_bytes(statement, i);
						std::vector<uint8_t> data(blobData, blobData + blobSize);

						value = std::move(data);
					}

					break;

					case SQLITE_NULL:
						value = nullptr;

						break;

					default:
						throw exception::DatabaseException("Unknown type");
					}

					column.try_emplace(columnName, std::move(value));
				}

				result.addRow(std::move(column));
			}

			break;

			case SQLITE_BUSY:
				sqlite3_sleep(100);

				break;

			default:
				throw exception::DatabaseException(sqlite3_errmsg(connection));
			}
		}

		return result;
	}
}
