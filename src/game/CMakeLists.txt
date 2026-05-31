add_library(goxel_game STATIC)

target_sources(goxel_game PRIVATE
        systems/render_system.cpp
)

target_sources(goxel_game PUBLIC FILE_SET HEADERS
        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
        FILES
        ecs.h
        components/transform.h
        components/renderable.h
)

add_library(goxel::game ALIAS goxel_game)

target_link_libraries(goxel_game PUBLIC
        goxel::core
        goxel::voxel
        goxel::rendering
        EnTT::EnTT
)