cmake_minimum_required(VERSION 3.2)
Include(FetchContent)

project(
    chess_engine
    VERSION 0.01
    LANGUAGES CXX
    )
set(CMAKE_CXX_STANDARD_REQUIRED 20)
include_directories( ${CMAKE_SOURCE_DIR}/inc )

file(GLOB SOURCES "./src/*.cpp" "./inc/**.hpp")
file(GLOB SOURCES_TEST "tests/*.cpp" "tests/*.hpp")
add_executable(chess_engine_main main.cpp ${SOURCES})

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.0.1 # or a later release
)

FetchContent_MakeAvailable(Catch2)

add_executable(tests ${SOURCES_TEST} ${SOURCES})
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)