add_library(goxel_game STATIC)

target_sources(goxel_game PRIVATE
        camera.cpp
)

target_sources(goxel_game PUBLIC
        FILE_SET HEADERS BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR} FILES
        camera.h
)

target_link_libraries(goxel_game PUBLIC
        goxel::core
        glm::glm
        spdlog::spdlog
)

target_include_directories(goxel_game PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(goxel::game ALIAS goxel_game)