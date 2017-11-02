cmake_minimum_required(VERSION 3.8)
project(tries)

set(gtest_SOURCE_DIR googletest/googletest)

ADD_SUBDIRECTORY(googletest)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ".")

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "-g -std=c++0x")

include_directories(include ${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

set(SOURCE_FILES
        source/main.cpp
        source/trie.cpp
        include/trie.hpp
        include/mvector.hpp
        include/logger.hpp
        include/cmd_parser.hpp
        include/hash_table.hpp
        include/parser.hpp
        source/parser.cpp
        include/date_time.hpp
        include/helpers.hpp
        include/mqueue.hpp)

set(TEST_SOURCE
        ngrams-testing/basic_tests.cpp
        ngrams-testing/vector-testing.cpp
        ngrams-testing/parser-testing.cpp
        ngrams-testing/cmd-parser-testing.cpp
        ngrams-testing/trie-insert-testing.cpp
        ngrams-testing/trie-delete-testing.cpp
        ngrams-testing/queue-testing.cpp
        source/parser.cpp
        source/trie.cpp)

add_executable(ngrams ${SOURCE_FILES})

add_executable(basic_tests ${TEST_SOURCE})
target_link_libraries(basic_tests gtest gtest_main)
