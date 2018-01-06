cmake_minimum_required(VERSION 3.5)

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

set(CMAKE_CXX_FLAGS "-g -O3 -I../include -Iinclude")

set(CMAKE_CXX_STANDARD 11)

find_package(Threads)

set(SOURCE_FILES
        source/main.cpp
        source/trie.cpp
        include/trie.hpp
        include/mvector.hpp
        include/logger.hpp source/logger.cpp
        include/cmd_parser.hpp
        include/hash_table.hpp
        include/parser.hpp source/parser.cpp
        include/date_time.hpp
        include/constants.hpp
        include/linear_hash.hpp source/linear_hash.cpp
        include/bit_vector.hpp source/bit_vector.cpp
        include/bloom_filter.hpp source/bloom_filter.cpp
        include/binary_search.hpp
        include/helpers.hpp
        include/mqueue.hpp
        include/mstack.hpp
        include/trie_nodes.hpp source/trie_nodes.cpp
        include/pair.hpp
        include/minHeap.hpp
        include/thread.hpp source/thread.cpp
        include/task.hpp source/task.cpp
        include/thread_pool.hpp source/thread_pool.cpp
        include/worker.hpp source/worker.cpp
        include/work_queue.hpp source/work_queue.cpp
        source/task.cpp)

set(TEST_SOURCE
        ngrams-testing/basic_tests.cpp
        ngrams-testing/vector-testing.cpp
        ngrams-testing/parser-testing.cpp
        ngrams-testing/cmd-parser-testing.cpp
        ngrams-testing/trie-testing.cpp
        ngrams-testing/queue-testing.cpp
        ngrams-testing/stack-testing.cpp
        ngrams-testing/linear-hash-testing.cpp
        ngrams-testing/bloom-filter-testing.cpp
        ngrams-testing/bit-vector-testing.cpp
        ngrams-testing/trie-node-testing.cpp
        ngrams-testing/minHeap-testing.cpp
        ngrams-testing/thread-testing.cpp
        ngrams-testing/thread-pool-testing.cpp
        include/mqueue.hpp
        source/parser.cpp
        source/trie.cpp
        source/trie_nodes.cpp
        source/linear_hash.cpp
        source/logger.cpp
        source/bloom_filter.cpp
        source/bit_vector.cpp
        include/thread.hpp source/thread.cpp
        include/task.hpp
        include/thread_pool.hpp source/thread_pool.cpp
        include/worker.hpp source/worker.cpp
        include/work_queue.hpp source/work_queue.cpp
        )

add_executable(ngrams ${SOURCE_FILES})
target_link_libraries(ngrams ${CMAKE_THREAD_LIBS_INIT})

add_executable(basic_tests ${TEST_SOURCE})
target_link_libraries(basic_tests gtest)
target_link_libraries(basic_tests ${CMAKE_THREAD_LIBS_INIT})
