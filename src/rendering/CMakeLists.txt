add_library(goxel_rendering STATIC)

target_sources(goxel_rendering PRIVATE
        renderer.cpp
        shader.cpp
        camera.cpp
)

target_sources(goxel_rendering PUBLIC FILE_SET HEADERS
        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
        FILES
        renderer.h
        shader.h
        camera.h
        handles.h
)

add_library(goxel::rendering ALIAS goxel_rendering)

target_link_libraries(goxel_rendering PUBLIC
        goxel::core
)

target_include_directories(goxel_rendering PRIVATE ${CMAKE_SOURCE_DIR}/external)