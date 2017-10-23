cmake_minimum_required(VERSION 3.8)
project(tries)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ".")

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "-g -std=c++0x -I../include -Iinclude")

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

add_executable(ngrams ${SOURCE_FILES})
