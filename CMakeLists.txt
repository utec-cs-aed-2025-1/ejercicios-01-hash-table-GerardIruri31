cmake_minimum_required(VERSION 3.29)
project(ejercicios_01_hash_table_GerardIruri31)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)


add_executable(p1 p1.cpp)
add_custom_command(
        TARGET p1 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/smalldata.csv
        $<TARGET_FILE_DIR:p1>
)


add_executable(p2 p2.cpp)
add_executable(p3 p3.cpp)