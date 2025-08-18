cmake_minimum_required(VERSION 3.20)

project(Battleship
  VERSION 0.1.0
  LANGUAGES CXX)

# Ajustes globales
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Opcional: advertencias útiles
if (MSVC)
  add_compile_options(/W4 /permissive-)
else()
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Biblioteca principal (lógica del juego)
add_library(battleship_lib
  src/board.cpp
  src/fleet.cpp
  src/placement.cpp
  src/shooting.cpp
  src/coords.cpp
)
target_include_directories(battleship_lib PUBLIC ${CMAKE_SOURCE_DIR}/include)

# Binario demo / juego (puedes reemplazarlo luego por tu Game Loop real)
add_executable(battleship
  src/battleship.cpp
)
target_link_libraries(battleship PRIVATE battleship_lib)

# ---- Tests (opción con asserts, sin framework) ----
add_executable(test_game_loop tests/test_game_loop.cpp)
target_link_libraries(test_game_loop PRIVATE battleship_lib)

add_executable(test_geometry tests/test_geometry.cpp)
target_link_libraries(test_geometry PRIVATE battleship_lib)

add_executable(test_shooting tests/test_shooting.cpp)
target_link_libraries(test_shooting PRIVATE battleship_lib)

# Habilita ctest (para ejecutar todos los tests desde la IDE/CLI)
include(CTest)
add_test(NAME geometry COMMAND test_geometry)
add_test(NAME shooting COMMAND test_shooting)
add_test(NAME game_loop COMMAND test_game_loop)
