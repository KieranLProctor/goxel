#include "config.h"
#include "core/application.h"
#include "core/window.h"
#include "spdlog/spdlog.h"
#include "utils/system_info.h"

#include <GLFW/glfw3.h>

int main()
{
    spdlog::info("goxel version: {}.{}.{}", goxel::version::major, goxel::version::minor, goxel::version::patch);

    core::ApplicationSpec spec;
    spec.name = "goxel";
    spec.window_spec.title = "goxel";
    spec.window_spec.width = 1000;
    spec.window_spec.height = 600;
    spec.window_spec.is_resizable = false;
    spec.window_spec.is_vsync = false;

    auto application = core::Application(spec);
    application.run();

    return 0;
}
