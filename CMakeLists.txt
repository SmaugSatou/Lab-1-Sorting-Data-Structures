cmake_minimum_required(VERSION 3.10)
project(StudentDatabase)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
    include
    include/models
    include/database
    include/benchmark
    include/sorting
    include/utils
)

set(SOURCES
    src/models/student.cpp
    
    src/database/database_vector.cpp
    src/database/database_hashmap.cpp
    src/database/database_treemap.cpp
    src/database/database_hybrid.cpp
    
    src/utils/csv_handler.cpp
    
    src/sorting/sorting.cpp
    
    src/benchmark/benchmark.cpp
    
    src/main.cpp
)

add_executable(student_db ${SOURCES})

target_compile_options(student_db PRIVATE -Wall -Wextra -Wpedantic)
