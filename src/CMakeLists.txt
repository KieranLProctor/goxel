add_subdirectory(core)
add_subdirectory(game)
add_subdirectory(ui)
add_subdirectory(utils)

add_executable(goxel
    main.cpp
)

target_link_libraries(goxel PRIVATE
        goxel::core
        goxel::game
        goxel::ui
        goxel::utils

        glfw
        glad
        imgui
        glm::glm
        spdlog::spdlog
        stb::stb
)

target_include_directories(goxel PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_custom_command(
        TARGET goxel POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        ${CMAKE_BINARY_DIR}/assets
)