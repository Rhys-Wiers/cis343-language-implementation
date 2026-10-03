# begin AI code
cmake_minimum_required(VERSION 3.16)
project(myinterp CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(myinterp
    src/main.cpp
    src/scanner.cpp
    src/token.cpp
    src/error.cpp
)

target_include_directories(myinterp PRIVATE src)
# end AI code
