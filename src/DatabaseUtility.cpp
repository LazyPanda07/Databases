#include "DatabaseUtility.h"

namespace database::utility
{
	constexpr std::string_view getVersion()
	{
		constexpr std::string_view version = "1.6.0";

		return version;
	}
}
