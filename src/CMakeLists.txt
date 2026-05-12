add_executable(goxel main.cpp
        ../external/stb/stb_image.cpp
        core/window.cpp
        utils/system_info.cpp
        core/application.cpp
        game/camera.cpp
        core/layer.h
        core/event.h
        core/input_event.h
        core/window_event.h
        ui/main_layer.cpp
        ui/overlay_layer.cpp
        core/renderer/renderer.cpp
)

configure_file(${PROJECT_SOURCE_DIR}/src/config.h.in ${PROJECT_SOURCE_DIR}/src/config.h)

## Link everything
#target_link_libraries(goxel PRIVATE
#        glfw
#        glad
#        imgui
#        glm::glm
#        OpenGL::GL
#        spdlog
#)

# Link everything
target_link_libraries(goxel PRIVATE
        glfw
        glad
        imgui
        glm::glm
        spdlog::spdlog
        stb::stb
)