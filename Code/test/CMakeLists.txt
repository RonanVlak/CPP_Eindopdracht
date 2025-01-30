cmake_minimum_required(VERSION 3.10)

# Set the project name
project(KerkersEnDrakenTests)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add Google Test
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_subdirectory(${PROJECT_SOURCE_DIR}/../external/tinyxml2 ${CMAKE_BINARY_DIR}/tinyxml2)

# Include directories from the main project
include_directories(${PROJECT_SOURCE_DIR}/../inc)
include_directories(${PROJECT_SOURCE_DIR}/../inc/FRONTEND)
include_directories(${PROJECT_SOURCE_DIR}/../inc/BACKEND)
include_directories(${PROJECT_SOURCE_DIR}/../external/sqlite3)

# Automatically search for source files
file(GLOB_RECURSE SOURCES
    ${PROJECT_SOURCE_DIR}/../src/FRONTEND/*.cpp
    ${PROJECT_SOURCE_DIR}/../src/BACKEND/*.cpp
    ${PROJECT_SOURCE_DIR}/../src/FSConverter.cpp
)

# Manually specify test source files
set(TESTSOURCES
    ${PROJECT_SOURCE_DIR}/test_main.cpp
    ${PROJECT_SOURCE_DIR}/test_locatiefactory.cpp
    ${PROJECT_SOURCE_DIR}/test_spelobjectfactory.cpp
    ${PROJECT_SOURCE_DIR}/test_vijandfactory.cpp
    ${PROJECT_SOURCE_DIR}/test_locatie.cpp
    ${PROJECT_SOURCE_DIR}/test_speler.cpp
    ${PROJECT_SOURCE_DIR}/test_spelwereld.cpp
    ${PROJECT_SOURCE_DIR}/test_vijand.cpp
    ${PROJECT_SOURCE_DIR}/test_wapenobject.cpp
    ${PROJECT_SOURCE_DIR}/test_wapenrustingobject.cpp
    ${PROJECT_SOURCE_DIR}/test_goudstukkenobject.cpp
    ${PROJECT_SOURCE_DIR}/test_consumeerbaarobject.cpp
    ${PROJECT_SOURCE_DIR}/test_customvector.cpp
    ${PROJECT_SOURCE_DIR}/test_xmlparser.cpp
    ${PROJECT_SOURCE_DIR}/test_dbloader.cpp
)

# Add SQLite3 source file
set(SQLITE3_SOURCES ${PROJECT_SOURCE_DIR}/../external/sqlite3/sqlite3.c)

# Add test executable
add_executable(runTests ${SOURCES} ${SQLITE3_SOURCES} ${TESTSOURCES}) 

# Link against Google Test and other necessary libraries
target_link_libraries(runTests ${GTEST_LIBRARIES} pthread tinyxml2)