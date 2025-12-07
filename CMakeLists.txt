cmake_minimum_required(VERSION 3.27.0)

set(CMAKE_CXX_STANDARD 20)

project(Databases VERSION 1.3.1)

set(SQLITE_LIBRARY_PATH ${PROJECT_SOURCE_DIR}/vendor/sqlite3/lib CACHE STRING "Path to sqlite3 lib directory")

add_library(
	${PROJECT_NAME} STATIC 
	src/Database.cpp
	src/Table.cpp
	src/DatabaseFactory.cpp
	src/SQLValue.cpp
	src/SQLResult.cpp
	src/DatabaseUtility.cpp
	src/Implementations/SQLiteDatabase.cpp
	src/Implementations/SQLiteTable.cpp
	src/Exceptions/DatabaseException.cpp
	src/Queries/RawQuery.cpp
	src/Queries/CreateTableQuery.cpp
)

if (DEFINED ENV{MARCH} AND NOT "$ENV{MARCH}" STREQUAL "")
	target_compile_options(${PROJECT_NAME} PRIVATE -march=$ENV{MARCH})
endif()

if(UNIX)
	set(SQLITE_DLL_DESTINATION_PATH lib)
	set(CMAKE_POSITION_INDEPENDENT_CODE ON)

	if (${CMAKE_SYSTEM_NAME} STREQUAL "Android")
		set(SQLITE_LIBRARY_PATH ${SQLITE_LIBRARY_PATH}/Android PARENT_SCOPE)
		
		install(DIRECTORY vendor/sqlite3/lib/Android/ DESTINATION lib)	
	else()
		if (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "aarch64")
			set(SQLITE_LIBRARY_PATH ${SQLITE_LIBRARY_PATH}/LinuxARM PARENT_SCOPE)

			install(DIRECTORY vendor/sqlite3/lib/LinuxARM/ DESTINATION lib)
		else()
			set(SQLITE_LIBRARY_PATH ${SQLITE_LIBRARY_PATH}/Linux PARENT_SCOPE)

			install(DIRECTORY vendor/sqlite3/lib/Linux/ DESTINATION lib)
		endif()
		
		add_compile_options(-rdynamic)
	endif()
else()
	set(SQLITE_LIBRARY_PATH ${SQLITE_LIBRARY_PATH}/Windows PARENT_SCOPE)
	set(SQLITE_DLL_DESTINATION_PATH dll)

	install(DIRECTORY vendor/sqlite3/lib/Windows/ DESTINATION lib)
	install(FILES ${PROJECT_SOURCE_DIR}/vendor/sqlite3/dll/sqlite3.dll DESTINATION dll)
endif()

target_include_directories(
	${PROJECT_NAME} PUBLIC 
	include/
	vendor/sqlite3/include/
)

install(
	TARGETS ${PROJECT_NAME} 
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib
)

install(DIRECTORY include DESTINATION .)
install(DIRECTORY vendor/sqlite3/license/ DESTINATION license)
