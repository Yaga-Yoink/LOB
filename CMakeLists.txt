cmake_minimum_required(VERSION 3.14)
project(OrderBookProject)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pg")

# Include the "include/" directory so your #include "OrderBook.hpp" works
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Add all source files
set(SOURCES
    src/main.cpp
    src/OrderBook.cpp
    src/OrderBookBuilder.cpp
    src/CSVParser.cpp
)

# Define the main executable
add_executable(main ${SOURCES})

# Add GoogleTest as a subdirectory
add_subdirectory(googletest)

# Enable testing with CTest
enable_testing()

# List of all test source files (add new tests here)
set(TEST_SOURCES
    tests/RingBuffer_test.cpp
    tests/CSVParser_test.cpp
    tests/OrderBook_test.cpp
    # Add more tests here as you create them
)

# Loop through the test sources and automatically create test executables
foreach(TEST_FILE ${TEST_SOURCES})
    # Extract the base name of the test file (e.g., "RingBuffer_test" from "RingBuffer_test.cpp")
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)

    # Create the test executable from the source file, including the necessary source files
    add_executable(${TEST_NAME} ${TEST_FILE} src/OrderBook.cpp src/OrderBookBuilder.cpp src/CSVParser.cpp)

    # Link the test executable with GoogleTest's main function and pthread
    target_link_libraries(${TEST_NAME} GTest::gtest_main pthread)

    # Automatically register the test with CTest
    include(GoogleTest)  # Make sure this is included after adding googletest
    gtest_discover_tests(${TEST_NAME})
endforeach()
