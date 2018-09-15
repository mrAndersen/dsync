project(dsync)
cmake_minimum_required(VERSION 3.8)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pthread")


message("Make sure you have libpqxx-dev, postgresql-server-dev-all")

find_package(PQXX REQUIRED)

if (NOT PQXX_FOUND)
    message(FATAL_ERROR "Please install libpqxx-dev")
    return()
endif ()

include_directories(${PQXX_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/vendor/fmt/include)

set(
        SOURCES
        main.cpp
        src/Initializer.cpp
        src/Initializer.h
        src/Platform/AbstractPlatform.cpp
        src/Platform/AbstractPlatform.h
        src/Platform/Postgresql.cpp
        src/Platform/Postgresql.h
        src/Platform/Mysql.cpp
        src/Platform/Mysql.h
        src/Util/util.cpp
        src/Util/util.h
        src/Util/Timer.h
        src/Util/Timer.cpp
)

add_executable(dsync ${SOURCES})
target_link_libraries(dsync ${PQXX_LIBRARIES})
