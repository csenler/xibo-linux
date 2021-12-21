#include <gtkmm/application.h>
#include <spdlog/sinks/stdout_sinks.h>

#include "MainWindowController.hpp"

#include "common/logger/Logging.hpp"
#include "config/AppConfig.hpp"

#include "Bypass.hpp"
#include <iostream>

int main(int argc, char* argv[])
{
    if (argc > 1)
    {
        Bypass bypass;
        // bypass.initialize();
        std::cout << "num of parameters : " << std::to_string(argc) << std::endl;
        int retVal = bypass.initWithArguments(argc, argv);
        if (retVal == 0)
        {
            std::cout << "empty parameters" << std::endl;
        }
        else if (retVal == -1)
        {
            std::cout << "wrong number of parameters" << std::endl;
        }
        return 1;
    }

    auto app = Gtk::Application::create();
    // auto ui = Gtk::Builder::create_from_file(AppConfig::uiFile().string());
    auto ui = Gtk::Builder::create_from_file("/home/svrn/src/xibo-linux/player/resources/ui.glade");

    std::vector<spdlog::sink_ptr> sinks{std::make_shared<spdlog::sinks::stdout_sink_mt>()};
    Log::create(sinks);

    Gtk::Window* mainWindow;
    ui->get_widget(Resources::Ui::MainWindow, mainWindow);

    MainWindowController controller{mainWindow, ui};

    mainWindow->show_all();

    return app->run(*mainWindow);
}
