cmake_minimum_required(VERSION 3.6)

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/googletest-download" )
execute_process(COMMAND "${CMAKE_COMMAND}" --build .
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/googletest-download" )

# Prevent GoogleTest from overriding our compiler/linker options
# when building with Visual Studio
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This adds
# the following targets: gtest, gtest_main, gmock
# and gmock_main
add_subdirectory("${CMAKE_BINARY_DIR}/googletest-src"
                 "${CMAKE_BINARY_DIR}/googletest-build")

# The gtest/gmock targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if(CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include"
                        "${gmock_SOURCE_DIR}/include")
endif()

# Now simply link your own targets against gtest, gmock,
# etc. as appropriate


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

set(TEST_SOURCE
        ngrams-testing/basic_tests.cpp
        ngrams-testing/vector-testing.cpp
        ngrams-testing/parser-testing.cpp
        ngrams-testing/cmd-parser-testing.cpp
        ngrams-testing/trie-insert-testing.cpp
        ngrams-testing/trie-delete-testing.cpp
        ngrams-testing/trie-search-testing.cpp
        ngrams-testing/queue-testing.cpp
        include/mqueue.hpp
        source/parser.cpp
        source/trie.cpp)

add_executable(ngrams ${SOURCE_FILES})

add_executable(basic_tests ${TEST_SOURCE})
target_link_libraries(basic_tests gtest)
