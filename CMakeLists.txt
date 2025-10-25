cmake_minimum_required(VERSION 3.10)
project(StudentDatabase)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

set(SOURCES
    src/student.cpp
    src/database_appr_1.cpp
    src/database_appr_2.cpp
    src/database_appr_3.cpp
    src/csv_handler.cpp
    src/sorting.cpp
    src/benchmark.cpp
    src/main.cpp
)

add_executable(student_db ${SOURCES})

target_compile_options(student_db PRIVATE -Wall -Wextra -Wpedantic)
