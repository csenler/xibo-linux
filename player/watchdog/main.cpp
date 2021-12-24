#include "common/fs/FilePath.hpp"
#include "common/fs/FileSystem.hpp"
#include "config/AppConfig.hpp"
#include "config/CmsSettings.hpp"

#include "ProcessWatcher.hpp"

#include <boost/program_options.hpp>

void setupNewConfigDir()
{
#ifdef SNAP_ENABLED
    try
    {
        const std::string CmsSettingsFile = "cmsSettings.xml";
        const std::string PlayerSettingsFile = "playerSettings.xml";
        const std::string PrivateKeyFile = "id_rsa";
        const std::string PublicKeyFile = "id_rsa.pub";
        const std::string ResourcesDir = "resources";
        if (FileSystem::exists(AppConfig::oldConfigDirectory() / CmsSettingsFile))
        {
            CmsSettings settings;
            settings.fromFile(AppConfig::oldConfigDirectory() / CmsSettingsFile);

            std::vector<std::string> filesToMove{CmsSettingsFile, PlayerSettingsFile, PrivateKeyFile, PublicKeyFile};
            for (auto&& file : filesToMove)
            {
                FileSystem::move(AppConfig::oldConfigDirectory() / file, AppConfig::configDirectory() / file);
            }

            if (settings.resourcesPath() == AppConfig::oldConfigDirectory() / ResourcesDir)
            {
                settings.resourcesPath().setValue(AppConfig::configDirectory() / ResourcesDir);
                settings.saveTo(AppConfig::cmsSettingsPath());
            }
        }
        std::cout << "oldConfigDIR : " << AppConfig::oldConfigDirectory() << std::endl;
    }
    catch (std::exception& e)
    {
        std::cout << "Error during setting up new config directory: " << e.what() << std::endl;
    }
#endif
}

// !!!cagri!!! --------------------
void on_cms(std::string cms)
{
  std::cout << "On cms: " << cms << '\n';
}
void on_key(std::string key)
{
  std::cout << "On key: " << key << '\n';
}
void on_lib(std::string lib)
{
  std::cout << "On lib: " << lib << '\n';
}
void on_displayID(std::string dID)
{
  std::cout << "On displayID: " << dID << '\n';
}
// ---------------------------------

int main(int argc, char** argv)
{
    setupNewConfigDir();

    try
    {
        boost::program_options::options_description desc{"Options"};
        desc.add_options()("disable-restart", "Don't restart player (disable watchdog)");
        desc.add_options()("config-app", "Run config application");

        // !!!cagri!!! ---------------------------------------
        desc.add_options()("cms", boost::program_options::value<std::string>()->notifier(on_cms), "cms");
        desc.add_options()("key", boost::program_options::value<std::string>()->notifier(on_key), "key");
        desc.add_options()("lib", boost::program_options::value<std::string>()->notifier(on_lib), "lib");
        desc.add_options()("dID", boost::program_options::value<std::string>()->notifier(on_displayID), "dID");
        boost::program_options::parsed_options parsed_options = boost::program_options::command_line_parser(argc, argv)
              .options(desc)
              .run();
        // ---------------------------------------------------

        boost::program_options::variables_map vm;
        store(parse_command_line(argc, argv, desc), vm);

#if defined(SNAP_ENABLED)
        std::cout << "Running in SNAP environment" << std::endl;
#elif defined(APPIMAGE_ENABLED)
        std::cout << "Running in AppImage environment" << std::endl;
        FilePath configDirPath{std::string{getenv("APPIMAGE")} + ".config"};
        if (!FileSystem::exists(configDirPath))
            throw PlayerRuntimeError{"XiboApp", "Config directory is not correctly setup in AppImage env"};
#endif

//        if (FileSystem::exists(AppConfig::cmsSettingsPath()) && vm.count("config-app") == 0)
//        {
//            ProcessWatcher playerWatcher{AppConfig::playerBinary(), vm.count("disable-restart") > 0};
//            playerWatcher.run();
//        }
//        else
//        {
//            boost::process::child optionsBin{AppConfig::optionsBinary()};
//            optionsBin.wait();
//        }

        std::cout << "watchdog -> XIBO_CONFIG_PATH : " << getenv("XIBO_CONFIG_PATH") << std::endl;


        if (vm.count("cms") > 0 && vm.count("key") > 0 && vm.count("lib") > 0 && vm.count("config-app") == 0)
        {
            std::cout << "watchdog -> bypass condition" << std::endl;

            std::string argString = "";
            std::cout << "vm size : " << vm.size() << std::endl;
            for (const auto& it : vm)
            {
                if (it.first == "cms" || it.first == "key" || it.first == "lib"
                        || (vm.count("dID") > 0 && it.first == "dID"))
                {
                    argString.append("--" + it.first + " " + it.second.as<std::string>() + " ");
                }
            }
            std::cout << "arguments string : " << argString << std::endl;

            ProcessWatcher playerWatcher{AppConfig::playerBinary(), vm.count("disable-restart") > 0};
//            playerWatcher.run();
            playerWatcher.runWithArgs(argString);
        }
        else if (FileSystem::exists(AppConfig::cmsSettingsPath()) && vm.count("config-app") == 0)
        {
            std::cout << "watchdog -> original start" << std::endl;
            ProcessWatcher playerWatcher{AppConfig::playerBinary(), vm.count("disable-restart") > 0};
            playerWatcher.run();
        }
        else
        {
            std::cout << "watchdog -> start with xibo-options" << std::endl;
            boost::process::child optionsBin{AppConfig::optionsBinary()};
            optionsBin.wait();
        }

    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
    return 0;
}
