#include "DatabaseUtility.h"

namespace database::utility
{
	constexpr std::string_view getVersion()
	{
		constexpr std::string_view version = "1.1.0";

		return version;
	}
}
