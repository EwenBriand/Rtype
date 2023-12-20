add_executable(test_serialization
    test_serialization.cpp)

target_link_libraries(test_serialization
    lengine)

add_test(NAME test_serialization
    COMMAND test_serialization)