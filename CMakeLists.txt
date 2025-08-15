cmake_minimum_required(VERSION 3.27.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/Databases CACHE STRING "")
set(CMAKE_BUILD_TYPE Release CACHE STRING "")

project(Databases VERSION 1.2.0)

set(SQLITE_LIBRARY_PATH ${PROJECT_SOURCE_DIR}/vendor/sqlite3/lib CACHE STRING "Path to lib directory")
set(SQLITE_DLL_PATH ${PROJECT_SOURCE_DIR}/vendor/sqlite3 CACHE STRING "Path to .dll or .so file")

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
	set(SQLITE_DLL_DESTINATION_PATH ${CMAKE_INSTALL_PREFIX}/lib)
	set(CMAKE_POSITION_INDEPENDENT_CODE ON)

	if (${CMAKE_SYSTEM_NAME} STREQUAL "Android")
		set(SQLITE_LIBRARY_PATH ${SQLITE_LIBRARY_PATH}/Android PARENT_SCOPE)
		set(SQLITE_DLL_PATH ${SQLITE_DLL_PATH}/lib/Android/libsqlite3.so)
		
		install(DIRECTORY vendor/sqlite3/lib/Android/ DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/)	
	else()
		if (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "aarch64")
			set(SQLITE_LIBRARY_PATH ${SQLITE_LIBRARY_PATH}/LinuxARM PARENT_SCOPE)
			set(SQLITE_DLL_PATH ${SQLITE_DLL_PATH}/lib/LinuxARM/libsqlite3.so)

			install(DIRECTORY vendor/sqlite3/lib/LinuxARM/ DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/)
		else()
			set(SQLITE_LIBRARY_PATH ${SQLITE_LIBRARY_PATH}/Linux PARENT_SCOPE)
			set(SQLITE_DLL_PATH ${SQLITE_DLL_PATH}/lib/Linux/libsqlite3.so)

			install(DIRECTORY vendor/sqlite3/lib/Linux/ DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/)
		endif()
		
		add_compile_options(-rdynamic)
	endif()
else()
	set(SQLITE_LIBRARY_PATH ${SQLITE_LIBRARY_PATH}/Windows PARENT_SCOPE)
	set(SQLITE_DLL_PATH ${SQLITE_DLL_PATH}/dll/sqlite3.dll)
	set(SQLITE_DLL_DESTINATION_PATH ${CMAKE_INSTALL_PREFIX}/dll)

	install(DIRECTORY vendor/sqlite3/lib/Windows/ DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/)
endif()

target_include_directories(
	${PROJECT_NAME} PUBLIC 
	src/
	vendor/sqlite3/include/
)

install(
	TARGETS ${PROJECT_NAME} 
	ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
)

install(DIRECTORY src/ DESTINATION ${CMAKE_INSTALL_PREFIX}/include FILES_MATCHING PATTERN "*.h")
install(FILES ${SQLITE_DLL_PATH} DESTINATION ${SQLITE_DLL_DESTINATION_PATH})
