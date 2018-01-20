#include "Image.hpp"
#include "Video.hpp"
#include "Region.hpp"
#include "MainLayout.hpp"
#include "WebView.hpp"

#include "MainParser.hpp"
#include "constants.hpp"

#include <spdlog/spdlog.h>
#include <gtkmm/application.h>

int main(int argc, char *argv[])
{
    spdlog::stdout_logger_st(LOGGER);
    spdlog::set_level(spdlog::level::debug);
    spdlog::set_pattern("[%H:%M:%S] [%l]: %v");

    auto logger = spdlog::get(LOGGER);
    auto app = Gtk::Application::create(argc, argv, "org.gtkmm.xibo");

    MainParser parser("GoPro/85.xml");
    MainLayout layout(0, 640, 480, "", "#000");

    layout.add_region(0, Size{300, 300}, Point{0, 0}, 0, false, Transition{});
    layout.region(0).add_media<Video>(0, 0, false, "/home/stivius/video.webm", false, false);

    layout.show_regions();

    return app->run(layout);
}
