cmake_minimum_required(VERSION 3.27.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/Databases CACHE STRING "")
set(CMAKE_BUILD_TYPE Release CACHE STRING "")

project(Databases VERSION 1.0.0)

add_library(
	${PROJECT_NAME} STATIC 
	src/IDatabaseImplementation.cpp
	src/IDatabase.cpp
	src/DatabaseFactory.cpp
	src/Implementations/SQLiteDatabase.cpp
	src/Implementations/SQLiteTable.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC src/)

install(
	TARGETS ${PROJECT_NAME} 
	ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
)

install(DIRECTORY src/ DESTINATION ${CMAKE_INSTALL_PREFIX}/include FILES_MATCHING PATTERN "*.h")
