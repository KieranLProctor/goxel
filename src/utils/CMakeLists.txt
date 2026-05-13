add_library(goxel_utils STATIC)

target_sources(goxel_utils PRIVATE
        system_info.cpp
)

target_link_libraries(goxel_utils PUBLIC
        goxel::core
        glm::glm
        spdlog::spdlog
)

target_include_directories(goxel_utils PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(goxel::utils ALIAS goxel_utils)