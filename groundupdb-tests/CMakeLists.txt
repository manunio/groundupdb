cmake_minimum_required(VERSION 3.12)

add_executable(groundupdb-tests
        dbmanagement-tests.cpp
        keyvalue-tests.cpp
        performane-tests.cpp)

include_directories(${groundupdb_SOURCE_DIR})

target_link_libraries(groundupdb-tests PRIVATE groundupdb)

target_compile_features(groundupdb-tests PRIVATE cxx_std_17)