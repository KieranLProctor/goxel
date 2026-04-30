#include "core/window.h"

#include <GLFW/glfw3.h>

int main()
{
    auto window = core::Window(core::WindowMode::WINDOWED, "Test", 600, 300, true, false);
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
