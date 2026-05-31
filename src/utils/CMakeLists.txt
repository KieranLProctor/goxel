add_library(goxel_utils STATIC)

target_sources(goxel_utils PRIVATE
        system_info.cpp
)

target_sources(goxel_utils PUBLIC FILE_SET HEADERS
        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
        FILES
        system_info.h
)

add_library(goxel::utils ALIAS goxel_utils)

target_link_libraries(goxel_utils PUBLIC
        goxel::core
)