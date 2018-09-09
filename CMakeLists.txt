project(dsync)
cmake_minimum_required(VERSION 3.8)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/deps)
set(CMAKE_CXX_STANDARD 14)

message("Make sure you have libqxx-dev, postgresql-server-dev-all")

find_package(PQXX REQUIRED)

if (NOT PQXX_FOUND)
    message(FATAL_ERROR  "Please install libpqxx-dev")
    return()
endif ()

include_directories(${PQXX_INCLUDE_DIRS})

set(
        SOURCES
        main.cpp
        src/Initializer.cpp
        src/Initializer.h
        src/Platform/AbstractPlatform.cpp
        src/Platform/AbstractPlatform.h
        src/Platform/Postgresql.cpp src/Platform/Postgresql.h src/Platform/Mysql.cpp src/Platform/Mysql.h src/Util/util.h)

add_executable(dsync ${SOURCES})
target_link_libraries(dsync ${PQXX_LIBRARIES})
