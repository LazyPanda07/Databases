#pragma once

#include "IQuery.h"

namespace database
{
	class RawQuery final : public IQuery
	{
	private:
		std::string query;

	public:
		RawQuery(std::string_view query);

		std::string_view getQuery() const override;

		~RawQuery() = default;
	};
}
