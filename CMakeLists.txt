# begin AI code
cmake_minimum_required(VERSION 3.16)
project(hashbrown CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(hashbrown
    src/main.cpp
    src/scanner.cpp
    src/token.cpp
    src/error.cpp
)

target_include_directories(hashbrown PRIVATE src)
# end AI code
