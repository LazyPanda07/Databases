#include "DatabaseException.h"

namespace database::exception
{
	DatabaseException::DatabaseException(std::string_view message) :
		runtime_error(message.data())
	{

	}
}
