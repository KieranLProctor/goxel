add_library(goxel_voxel STATIC)

target_sources(goxel_voxel PRIVATE
        chunk.cpp
)

target_sources(goxel_voxel PUBLIC FILE_SET HEADERS
        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
        FILES
        block.h
        chunk.h
)

add_library(goxel::voxel ALIAS goxel_voxel)

target_link_libraries(goxel_voxel PUBLIC
        goxel::core
        goxel::rendering
)