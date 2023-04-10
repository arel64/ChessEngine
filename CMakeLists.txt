cmake_minimum_required(VERSION 3.2)
project(
    chess_engine
    VERSION 0.01
    LANGUAGES CXX
    )
set(CMAKE_CXX_STANDARD_REQUIRED 17)
file(GLOB SOURCES "*.cpp" "*.hpp")
add_executable(engine ${SOURCES})
