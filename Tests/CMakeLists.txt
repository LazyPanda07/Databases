cmake_minimum_required(VERSION 3.27.0)

set(CMAKE_CXX_STANDARD 20)
set(GTEST_LIB_DIR ${CMAKE_SOURCE_DIR}/gtest/lib/)
set(DATABASES_DIR ${CMAKE_SOURCE_DIR}/../Databases)
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/bin)

project(Tests)

add_executable(
	${PROJECT_NAME}
	main.cpp
	sqlite.cpp
)

find_package(GTest)

if(NOT ${GTest_FOUND})
	set(GTEST_BOTH_LIBRARIES gtest gtest_main)
	set(GTEST_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/gtest/include/)
endif()

target_include_directories(
	${PROJECT_NAME} PRIVATE
	${DATABASES_DIR}/include/
	${GTEST_INCLUDE_DIRS}
)

target_link_directories(
	${PROJECT_NAME} PRIVATE
	${DATABASES_DIR}/lib/
	${DATABASES_DIR}/lib/vendor/sqlite3/
	${GTEST_LIB_DIR}
)

target_link_libraries(
	${PROJECT_NAME} PRIVATE
	Databases
	sqlite3
	${GTEST_BOTH_LIBRARIES}
)

install(DIRECTORY ${DATABASES_DIR}/dll DESTINATION ${CMAKE_INSTALL_PREFIX})
install(DIRECTORY ${DATABASES_DIR}/lib/vendor/sqlite3/ DESTINATION ${CMAKE_INSTALL_PREFIX})
