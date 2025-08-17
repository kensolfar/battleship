add_executable(battleship_tests
    test_neighbors.cpp
)
target_link_libraries(battleship_tests
    PRIVATE
    battleship_lib
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(battleship_tests)
