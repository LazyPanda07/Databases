#include <filesystem>

#include "gtest/gtest.h"

int main(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);

	if (std::filesystem::exists("test.sqlite"))
	{
		std::filesystem::remove("test.sqlite");
	}

	return RUN_ALL_TESTS();
}
