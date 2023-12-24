enable_testing()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(
  specs
  test_coroutines.cpp
  test_circular_buffer.cpp
  test_serialization.cpp
  test_ecs.cpp
)
target_link_libraries(
  specs
  lengine
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(specs)