add_subdirectory(core)
add_subdirectory(rendering)
add_subdirectory(voxel)
add_subdirectory(game)
add_subdirectory(ui)
add_subdirectory(utils)

add_executable(goxel main.cpp)

target_link_libraries(goxel PRIVATE
        goxel::core
        goxel::rendering
        goxel::voxel
        goxel::game
        goxel::ui
        goxel::utils
)