#include "config.h"
#include "core/window.h"
#include "spdlog/spdlog.h"
#include "utils/system_info.h"

#include <GLFW/glfw3.h>

int main()
{
    spdlog::info("goxel version: {}.{}.{}", goxel::version::major, goxel::version::minor, goxel::version::patch);

    auto window = core::Window(core::WindowMode::WINDOWED, "Test", 1000, 600, true, false);
    window.create();

    /* Loop until the user closes the window */
    while (!window.get_should_close())
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        window.update();

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
