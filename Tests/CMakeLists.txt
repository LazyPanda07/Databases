cmake_minimum_required(VERSION 3.27.0)

set(CMAKE_CXX_STANDARD 20)
set(GTEST_LIB_DIR ${CMAKE_SOURCE_DIR}/gtest/lib/)
set(DATABASES_DIR ${CMAKE_SOURCE_DIR}/../Databases)
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/bin)
set(GTEST_VERSION 1.17.0)

project(Tests)

add_executable(
	${PROJECT_NAME}
	main.cpp
	sqlite.cpp
)

include(FetchContent)

FetchContent_Declare(
	gtest
	GIT_REPOSITORY https://github.com/google/googletest.git
	GIT_TAG v${GTEST_VERSION}
)

FetchContent_MakeAvailable(gtest)

target_include_directories(
	${PROJECT_NAME} PRIVATE
	${DATABASES_DIR}/include
)

target_link_directories(
	${PROJECT_NAME} PRIVATE
	${DATABASES_DIR}/lib
)

target_link_libraries(
	${PROJECT_NAME} PRIVATE
	Databases
	sqlite3
	hiredis
	gtest
	gtest_main
)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX})

if (UNIX)
	install(DIRECTORY ${DATABASES_DIR}/lib/ DESTINATION . FILES_MATCHING PATTERN "*.so")
else()
	install(DIRECTORY ${DATABASES_DIR}/dll/ DESTINATION .)
endif()
