cmake_minimum_required(VERSION 3.20)
project(Battleship LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Librería del proyecto
add_library(battleship_lib
    src/board.cpp
    src/neighbors.cpp
)
target_include_directories(battleship_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

# ---- GoogleTest vía FetchContent (portátil) ----
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
# Para MSVC / Windows
set(gtest_force_shared_crt ON CACHE BOOL "Force gtest to use shared CRT" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_subdirectory(tests)
