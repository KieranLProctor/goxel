add_library(goxel_core STATIC)

target_sources(goxel_core PRIVATE
        application.cpp
        window.cpp
        renderer/renderer.cpp
        renderer/shader.cpp
)

target_sources(goxel_core PUBLIC
        FILE_SET HEADERS BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR} FILES
        event.h
        input_event.h
        window_event.h
)

target_link_libraries(goxel_core PUBLIC
        glfw
        glad
        imgui
        glm::glm
        spdlog::spdlog
)

target_include_directories(goxel_core PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(goxel::core ALIAS goxel_core)