add_executable(goxel main.cpp)

# Link everything
target_link_libraries(goxel PRIVATE
        glfw
        glad
        imgui
        OpenGL::GL
)