add_library(goxel_ui STATIC)

target_sources(goxel_ui PRIVATE
        main_layer.cpp
        overlay_layer.cpp
)

target_link_libraries(goxel_ui PUBLIC
        goxel::core
        goxel::game
        glm::glm
        spdlog::spdlog
)

target_include_directories(goxel_ui PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(goxel::ui ALIAS goxel_ui)