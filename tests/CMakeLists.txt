# add_executable(test_serialization
#     test_serialization.cpp)

# target_link_libraries(test_serialization
#     lengine)

# add_test(NAME test_serialization
#     COMMAND test_serialization
# )


# add_executable(test_circular_buffer
#     test_circular_buffer.cpp)
# target_link_libraries(test_circular_buffer
#     lengine)

# add_test(NAME test_circular_buffer
#     COMMAND test_circular_buffer
# )

enable_testing()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(
  specs
  test_coroutines.cpp
  test_circular_buffer.cpp
  test_serialization.cpp
)
target_link_libraries(
  specs
  lengine
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(specs)