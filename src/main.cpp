#include "config.h"
#include "core/application.h"
#include "core/window.h"
#include "spdlog/spdlog.h"
#include "ui/main_layer.h"
#include "ui/overlay_layer.h"

int main()
{
    spdlog::info("goxel version: {}.{}.{}", goxel::version::major, goxel::version::minor, goxel::version::patch);

    goxel::core::ApplicationSpec spec;
    spec.name = "goxel";
    spec.window_spec.title = "goxel";
    spec.window_spec.width = 1000;
    spec.window_spec.height = 600;
    spec.window_spec.is_resizable = true;
    spec.window_spec.is_vsync = false;

    auto application = goxel::core::Application(spec);
    application.push_layer<goxel::ui::MainLayer>();
    application.push_layer<goxel::ui::OverlayLayer>();
    application.run();

    return 0;
}
