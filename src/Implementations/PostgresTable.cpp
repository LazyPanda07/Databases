#include "Implementations/PostgresTable.h"

#include <format>

#include <libpq-fe.h>

#include "Implementations/PostgresDatabase.h"
#include "Exceptions/DatabaseException.h"

inline constexpr Oid BOOLEAN = 16;
inline constexpr Oid BYTEA = 17;
inline constexpr Oid NAME = 19;
inline constexpr Oid BIGINT = 20;
inline constexpr Oid SMALLINT = 21;
inline constexpr Oid INTEGER = 23;
inline constexpr Oid TEXT = 25;
inline constexpr Oid REAL = 700;
inline constexpr Oid DOUBLE = 701;
inline constexpr Oid CHARN = 1042;
inline constexpr Oid VARCHAR = 1043;
inline constexpr Oid NUMERIC = 1700;

static void deleter(PGresult* ptr);

namespace database
{
	PGconn* PostgresTable::getConnection() const
	{
		return **static_cast<PostgresDatabase*>(database);
	}

	Table* PostgresTable::createTable(std::string_view tableName, const IQuery& query, Database* database)
	{
		return new PostgresTable(tableName, query, database);
	}

	PostgresTable::PostgresTable(std::string_view tableName, const IQuery& query, Database* database) :
		Table(tableName, database)
	{
		PGconn* connection = this->getConnection();
		std::unique_ptr<PGresult, decltype(&deleter)> result(PQexec(connection, query.getQuery().data()), &deleter);

		if (PQresultStatus(result.get()) != ExecStatusType::PGRES_COMMAND_OK)
		{
			throw exception::DatabaseException(PQerrorMessage(connection));
		}
	}

	SqlResult PostgresTable::execute(const IQuery& query, const std::vector<SqlValue>& values)
	{
		PGconn* connection = this->getConnection();
		std::vector<std::string> parameterValues;
		std::vector<const char*> parameters;
		std::vector<int> parameterLength;
		std::vector<int> parameterFormat;

		parameterValues.reserve(values.size());
		parameters.reserve(values.size());

		for (const SqlValue& value : values)
		{
			switch (value->index())
			{
			case 0: // int64_t
				parameters.push_back
				(
					parameterValues.emplace_back(std::to_string(value.get<int64_t>())).data()
				);
				parameterFormat.push_back(0);

				break;

			case 1: // double
				parameters.push_back
				(
					parameterValues.emplace_back(std::to_string(value.get<double>())).data()
				);
				parameterFormat.push_back(0);

				break;

			case 2: // std::string
				parameters.push_back
				(
					parameterValues.emplace_back(value.get<std::string>()).data()
				);
				parameterFormat.push_back(0);

				break;

			case 3: // bool
				parameters.push_back
				(
					parameterValues.emplace_back(std::to_string(value.get<bool>())).data()
				);
				parameterFormat.push_back(0);

				break;

			case 4: // nullptr_t
				parameters.push_back(nullptr);
				parameterFormat.push_back(0);

				break;

			case 5: // blob
			{
				const std::vector<uint8_t>& blob = value.get<std::vector<uint8_t>>();

				parameters.push_back
				(
					parameterValues.emplace_back(reinterpret_cast<const char*>(blob.data()), blob.size()).data()
				);

				parameterFormat.push_back(1);
			}

			break;

			default:
				throw exception::DatabaseException(std::format("Wrong index: {}", value->index()));
			}

			parameterLength.push_back
			(
				parameterFormat.back() == 0 ?
				0 :
				static_cast<int>(parameterValues.back().size())
			);
		}

		std::unique_ptr<PGresult, decltype(&deleter)> pqresult(PQexecParams(connection, query.getQuery().data(), static_cast<int>(values.size()), nullptr, parameters.data(), parameterLength.data(), parameterFormat.data(), 0), &deleter);
		ExecStatusType status = PQresultStatus(pqresult.get());
		SqlResult result;

		if (status != ExecStatusType::PGRES_TUPLES_OK && status != ExecStatusType::PGRES_COMMAND_OK)
		{
			throw exception::DatabaseException(std::format("Can't execute query: {} with error: {}", query.getQuery(), PQerrorMessage(connection)));
		}

		if (status == ExecStatusType::PGRES_TUPLES_OK)
		{
			int rows = PQntuples(pqresult.get());
			int cols = PQnfields(pqresult.get());

			for (int row = 0; row < rows; row++)
			{
				std::unordered_map<std::string, SqlValue> column;

				for (int col = 0; col < cols; col++)
				{
					char* valueAsString = PQgetvalue(pqresult.get(), row, col);
					SqlValue value;
					bool skip = false;

					if (PQgetisnull(pqresult.get(), row, col))
					{
						value = nullptr;

						column.emplace(PQfname(pqresult.get(), col), std::move(value));

						continue;
					}

					switch (PQftype(pqresult.get(), col))
					{
					case BIGINT:
					case SMALLINT:
					case INTEGER:
						value = std::stoll(valueAsString);

						break;

					case BOOLEAN:
						if (PQfformat(pqresult.get(), col) == 0)
						{
							value = PQgetvalue(pqresult.get(), row, col)[0] == 't';
						}
						else
						{
							value = reinterpret_cast<unsigned char*>(PQgetvalue(pqresult.get(), row, col))[0] != 0;
						}

						break;

					case REAL:
					case DOUBLE:
					case NUMERIC:
						value = std::stod(valueAsString);

						break;

					case NAME:
					case TEXT:
					case CHARN:
					case VARCHAR:
						value = valueAsString;

						break;

					case BYTEA:
					{
						size_t size = 0;
						
						if (uint8_t* data = PQunescapeBytea(reinterpret_cast<const uint8_t*>(valueAsString), &size))
						{
							std::vector<uint8_t> blobData(data, data + size);

							value = std::move(blobData);

							PQfreemem(data);
						}
					}

					break;

					default:
						skip = true;

						break;
					}

					if (skip)
					{
						continue;
					}

					column.emplace(PQfname(pqresult.get(), col), std::move(value));
				}

				result.addRow(std::move(column));
			}
		}

		return result;
	}
}

void deleter(PGresult* ptr)
{
	PQclear(ptr);
};
