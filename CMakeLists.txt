cmake_minimum_required(VERSION 3.27.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/Databases CACHE STRING "")
set(CMAKE_BUILD_TYPE Release CACHE STRING "")
set(SQLITE_LIBRARY_PATH ${CMAKE_SOURCE_DIR}/vendor/sqlite3/lib)
set(SQLITE_DLL_PATH ${CMAKE_SOURCE_DIR}/vendor/sqlite3)

project(Databases VERSION 1.0.0)

add_library(
	${PROJECT_NAME} STATIC 
	src/IDatabaseImplementation.cpp
	src/IDatabase.cpp
	src/DatabaseFactory.cpp
	src/Implementations/SQLiteDatabase.cpp
	src/Implementations/SQLiteTable.cpp
)

if(UNIX)
	set(SQLITE_DLL_DESTINATION_PATH ${CMAKE_INSTALL_PREFIX}/lib)
	set(CMAKE_POSITION_INDEPENDENT_CODE ON)

	if (${CMAKE_SYSTEM_NAME} STREQUAL "Android")
		set(SQLITE_LIBRARY_PATH ${SQLITE_LIBRARY_PATH}/Android)
		set(SQLITE_DLL_PATH ${SQLITE_DLL_PATH}/lib/Android/libsqlite3.so)
		
		install(DIRECTORY vendor/sqlite3/lib/Android/ DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/)
	else()
		set(SQLITE_LIBRARY_PATH ${SQLITE_LIBRARY_PATH}/Linux)
		set(SQLITE_DLL_PATH ${SQLITE_DLL_PATH}/lib/Linux/libsqlite3.so)

		add_compile_options(-rdynamic)

		install(DIRECTORY vendor/sqlite3/lib/Linux/ DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/)
	endif()
else()
	set(SQLITE_LIBRARY_PATH ${SQLITE_LIBRARY_PATH}/Windows)
	set(SQLITE_DLL_PATH ${SQLITE_DLL_PATH}/dll/sqlite3.dll)
	set(SQLITE_DLL_DESTINATION_PATH ${CMAKE_INSTALL_PREFIX}/dll)

	install(DIRECTORY vendor/sqlite3/lib/Windows/ DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/vendor/sqlite3)
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
