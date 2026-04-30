add_executable(goxel main.cpp
        core/window.cpp
        utils/system_info.cpp
)

configure_file(${PROJECT_SOURCE_DIR}/src/config.h.in ${PROJECT_SOURCE_DIR}/src/config.h)

# Link everything
target_link_libraries(goxel PRIVATE
        glfw
        glad
        imgui
        glm::glm
        OpenGL::GL
        spdlog
)