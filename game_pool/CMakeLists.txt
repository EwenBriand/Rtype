include_directories(
    ${CMAKE_SOURCE_DIR}
    ./scripts/
    ./
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcoroutines")

set(SRC
    GamePool.cpp
    LocalCarController.cpp
)

add_library(game_pool SHARED ${SRC})
target_link_libraries(game_pool lengine)


# manually build some scripts below if needed
add_library(Car SHARED scripts/Car.cpp ../.metadata/CarMeta.cpp)
set_target_properties(Car PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/metabuild)

add_library(RLCamera3D SHARED scripts/RLCamera3D.cpp ../.metadata/RLCamera3DMeta.cpp)
set_target_properties(RLCamera3D PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/metabuild)


# link the game pool to the car script
target_link_libraries(game_pool Car)
target_link_libraries(game_pool RLCamera3D)