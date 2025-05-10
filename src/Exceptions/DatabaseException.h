#pragma once

#include <stdexcept>

namespace database::exception
{
	class DatabaseException : public std::runtime_error
	{
	public:
		DatabaseException(std::string_view message);

		~DatabaseException() = default;
	};
}
