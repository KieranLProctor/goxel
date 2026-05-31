add_library(goxel_core STATIC)

target_sources(goxel_core PRIVATE
        application.cpp
        window.cpp
)

target_sources(goxel_core PUBLIC
        FILE_SET HEADERS BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR} FILES
        application.h
        event.h
        input_event.h
        layer.h
        window.h
        window_event.h
)

add_library(goxel::core ALIAS goxel_core)

target_link_libraries(goxel_core PUBLIC
        glfw
        glad
        imgui
        glm::glm
        spdlog::spdlog
)