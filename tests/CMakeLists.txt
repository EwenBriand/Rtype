set(CMAKE_CXX_STANDARD 17)

add_executable(test_serialization
    test_serialization.cpp)
target_link_libraries(test_serialization
    engine
    config++
    criterion)
add_test(test_serialization test_serialization)
