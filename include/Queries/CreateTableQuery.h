#pragma once

#include "IQuery.h"

#include <vector>

#include "SqlValue.h"

namespace database
{
	class CreateTableQuery : public IQuery
	{
	private:
		std::string query;

	public:
		CreateTableQuery(std::string_view query);

		std::string_view getQuery() const override;

		virtual ~CreateTableQuery() = default;
	};
}
