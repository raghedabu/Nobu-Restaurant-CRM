cmake_minimum_required(VERSION 3.10)
project(NobuCRM)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use local SQLite files
include_directories("${CMAKE_SOURCE_DIR}/sqlite")

# Include the project's include directory
include_directories(${CMAKE_SOURCE_DIR})

# Add all source files including SQLite
file(GLOB_RECURSE SOURCES
     "src/*.cpp"
     "sqlite/sqlite3.c"
     "main.cpp"
)

# Create the executable
add_executable(NobuCRM ${SOURCES})
