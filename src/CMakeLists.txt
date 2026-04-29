#add_subdirectory(core)

add_executable(goxel main.cpp)

target_link_libraries(goxel glfw)
target_link_libraries(goxel glad)