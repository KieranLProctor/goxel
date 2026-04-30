add_executable(goxel main.cpp
        core/window.cpp)

# Link everything
target_link_libraries(goxel PRIVATE
        glfw
        glad
        imgui
        glm::glm
        OpenGL::GL
)