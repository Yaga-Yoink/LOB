cmake_minimum_required(VERSION 3.14)
project(OrderBookProject)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g")

# Include the "include/" directory so your #include "OrderBook.hpp" works
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Add all source files
set(SOURCES
    src/main.cpp
    src/OrderBook.cpp
    src/OrderBookBuilder.cpp
)

add_executable(main ${SOURCES})

add_subdirectory(googletest)

enable_testing()

add_executable(
    RingBuffer_test
    tests/RingBuffer_test.cpp
)
add_executable(
    CSVParser_test
    tests/CSVParser_test.cpp
)

target_link_libraries(
    RingBuffer_test
    GTest::gtest_main
)

target_link_libraries(
    CSVParser_test
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(RingBuffer_test)
gtest_discover_tests(CSVParser_test)