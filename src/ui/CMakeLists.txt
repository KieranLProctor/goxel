add_library(goxel_ui STATIC)

target_sources(goxel_ui PRIVATE
        main_layer.cpp
        overlay_layer.cpp
)

target_sources(goxel_ui PUBLIC FILE_SET HEADERS
        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
        FILES
        main_layer.h
        overlay_layer.h
)

add_library(goxel::ui ALIAS goxel_ui)

target_link_libraries(goxel_ui PUBLIC
        goxel::core
        goxel::rendering
        goxel::game
)