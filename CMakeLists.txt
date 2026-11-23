cmake_minimum_required(VERSION 4.0.0)

set(CMAKE_CXX_STANDARD 20)

project(Databases VERSION 1.8.2)

set(SQLITE_LIBRARY_PATH ${PROJECT_SOURCE_DIR}/vendor/sqlite3/lib CACHE STRING "Path to sqlite3 lib directory")
set(HIREDIS_LIBRARY_PATH ${PROJECT_SOURCE_DIR}/vendor/hiredis/lib CACHE STRING "Path to hiredis lib directory")
set(POSTGRES_LIBRARY_PATH ${PROJECT_SOURCE_DIR}/vendor/postgres/lib CACHE STRING "Path to postgres lib directory")

add_library(
	${PROJECT_NAME} STATIC 
	src/Database.cpp
	src/Table.cpp
	src/DatabaseFactory.cpp
	src/SqlValue.cpp
	src/SqlResult.cpp
	src/DatabaseUtility.cpp
	src/Implementations/SqliteDatabase.cpp
	src/Implementations/SqliteTable.cpp
	src/Implementations/RedisDatabase.cpp
	src/Implementations/RedisTable.cpp
	src/Implementations/PostgresDatabase.cpp
	src/Implementations/PostgresTable.cpp
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
		set(CMAKE_CXX_MODULE_STD OFF)

		set(SQLITE_LIBRARY_PATH ${SQLITE_LIBRARY_PATH}/Android PARENT_SCOPE)
		set(HIREDIS_LIBRARY_PATH ${HIREDIS_LIBRARY_PATH}/Android PARENT_SCOPE)
		set(POSTGRES_LIBRARY_PATH ${POSTGRES_LIBRARY_PATH}/Android PARENT_SCOPE)
		
		install(DIRECTORY vendor/sqlite3/lib/Android/ DESTINATION lib)
		install(DIRECTORY vendor/hiredis/lib/Android/ DESTINATION lib)
		install(DIRECTORY vendor/postgres/lib/Android/ DESTINATION lib)
	else()
		if (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "aarch64")
			set(SQLITE_LIBRARY_PATH ${SQLITE_LIBRARY_PATH}/LinuxARM PARENT_SCOPE)
			set(HIREDIS_LIBRARY_PATH ${HIREDIS_LIBRARY_PATH}/LinuxARM PARENT_SCOPE)
			set(POSTGRES_LIBRARY_PATH ${POSTGRES_LIBRARY_PATH}/LinuxARM PARENT_SCOPE)

			install(DIRECTORY vendor/sqlite3/lib/LinuxARM/ DESTINATION lib)
			install(DIRECTORY vendor/hiredis/lib/LinuxARM/ DESTINATION lib)
			install(DIRECTORY vendor/postgres/lib/LinuxARM/ DESTINATION lib)
		else()
			set(SQLITE_LIBRARY_PATH ${SQLITE_LIBRARY_PATH}/Linux PARENT_SCOPE)
			set(HIREDIS_LIBRARY_PATH ${HIREDIS_LIBRARY_PATH}/Linux PARENT_SCOPE)
			set(POSTGRES_LIBRARY_PATH ${POSTGRES_LIBRARY_PATH}/Linux PARENT_SCOPE)

			install(DIRECTORY vendor/sqlite3/lib/Linux/ DESTINATION lib)
			install(DIRECTORY vendor/hiredis/lib/Linux/ DESTINATION lib)
			install(DIRECTORY vendor/postgres/lib/Linux/ DESTINATION lib)
		endif()
		
		add_compile_options(-rdynamic)
	endif()
else()
	set(SQLITE_LIBRARY_PATH ${SQLITE_LIBRARY_PATH}/Windows PARENT_SCOPE)
	set(HIREDIS_LIBRARY_PATH ${HIREDIS_LIBRARY_PATH}/Windows PARENT_SCOPE)
	set(POSTGRES_LIBRARY_PATH ${POSTGRES_LIBRARY_PATH}/Windows PARENT_SCOPE)
	set(SQLITE_DLL_DESTINATION_PATH dll)

	install(DIRECTORY vendor/sqlite3/lib/Windows/ DESTINATION lib)
	install(DIRECTORY vendor/hiredis/lib/Windows/ DESTINATION lib)
	install(DIRECTORY vendor/postgres/lib/Windows/ DESTINATION lib)
	install(FILES ${PROJECT_SOURCE_DIR}/vendor/sqlite3/dll/sqlite3.dll DESTINATION dll)
	install(FILES ${PROJECT_SOURCE_DIR}/vendor/postgres/dll/pq.dll DESTINATION dll)
endif()

target_include_directories(
	${PROJECT_NAME} PUBLIC 
	include/
	vendor/sqlite3/include/
	vendor/hiredis/include/
	vendor/postgres/include/
)

target_link_directories(
	${PROJECT_NAME} PUBLIC
	${SQLITE_LIBRARY_PATH}
	${HIREDIS_LIBRARY_PATH}
	${POSTGRES_LIBRARY_PATH}
)

target_link_libraries(
	${PROJECT_NAME} PUBLIC
	sqlite3
	hiredis
	pq
)

if (UNIX)
	target_link_libraries(
		${PROJECT_NAME} PUBLIC
		pgcommon_shlib
		pgport_shlib
	)
endif()

install(
	TARGETS ${PROJECT_NAME} 
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib
)

install(DIRECTORY include DESTINATION .)
install(DIRECTORY vendor/sqlite3/license/ DESTINATION license)
install(DIRECTORY vendor/hiredis/license/ DESTINATION license)
install(DIRECTORY vendor/postgres/license/ DESTINATION license)
