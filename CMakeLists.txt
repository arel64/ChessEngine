cmake_minimum_required(VERSION 3.2)
Include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.0.1 # or a later release
)

FetchContent_MakeAvailable(Catch2)

project(
    chess_engine
    VERSION 0.01
    LANGUAGES CXX
    )
set(CMAKE_CXX_STANDARD_REQUIRED 20)
set(DCMAKE_EXPORT_COMPILE_COMMANDS 1)
include_directories( ${CMAKE_SOURCE_DIR}/include/Game)
include_directories( ${CMAKE_SOURCE_DIR}/include/Engine)
include_directories( ${CMAKE_SOURCE_DIR}/include/Engine/Commands)
file(GLOB_RECURSE  SOURCES "./src/**.cpp" "./include/**.hpp")
file(GLOB_RECURSE  SOURCES_TEST "tests/*.cpp" "tests/*.hpp")
add_executable(chess_engine_main main.cpp ${SOURCES} ${SOURCES_TEST})



add_executable(tests ${SOURCES_TEST} ${SOURCES})
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
target_link_libraries(chess_engine_main PRIVATE Catch2::Catch2WithMain)