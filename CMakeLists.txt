cmake_minimum_required(VERSION 3.8)
project(tries)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ".")

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "-g -std=c++0x -I../include -Iinclude")

set(SOURCE_FILES
        source/main.cpp
        source/trie.cpp
        include/trie.h
        include/mvector.h
        include/logger.h
        include/cmd_parser.h
        include/hash_table.h
        include/parser.h
        source/parser.cpp
        include/date_time.h
        include/helpers.h)

add_executable(ngrams ${SOURCE_FILES})
