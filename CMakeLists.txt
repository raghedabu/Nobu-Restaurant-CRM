cmake_minimum_required(VERSION 3.10)

# Specify Visual Studio compiler before project declaration
set(CMAKE_C_COMPILER "C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.37.32822/bin/Hostx64/x64/cl.exe")
set(CMAKE_CXX_COMPILER "C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.37.32822/bin/Hostx64/x64/cl.exe")

project(NobuCRM)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Explicitly use Visual Studio's include paths
include_directories("C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.37.32822/include")
include_directories("C:/Program Files (x86)/Windows Kits/10/Include/10.0.22000.0/ucrt")

# Use local SQLite files instead of system SQLite
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

# No need to link against external SQLite library anymore
# target_link_libraries(NobuCRM ${SQLite3_LIBRARIES})