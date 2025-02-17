#include "AppConfig.hpp"

#include "common/PlayerRuntimeError.hpp"
#include "common/fs/FileSystem.hpp"
#include "common/logger/Logging.hpp"

#if !defined(SNAP_ENABLED)
#include "GitHash.hpp"
#endif

#include <filesystem>
#include <linux/limits.h>
#include <unistd.h>

#include <iostream>

FilePath AppConfig::resourceDirectory_;

std::string AppConfig::version()
{
    return releaseVersion() + "-" + codeVersion();
}

std::string AppConfig::releaseVersion()
{
#if defined(SNAP_ENABLED)
    return getenv("SNAP_VERSION");
#elif defined(APPIMAGE_ENABLED)
    return "1.8-R6";
#else
    return "dev";
#endif
}

std::string AppConfig::codeVersion()
{
#if defined(SNAP_ENABLED)
    return getenv("SNAP_REVISION");
#else
    return GIT_HASH;
#endif
}

FilePath AppConfig::resourceDirectory()
{
    return resourceDirectory_;
}

void AppConfig::resourceDirectory(const FilePath& directory)
{
    if (!FileSystem::exists(directory))
        throw PlayerRuntimeError{
            "AppConfig", "Resource directory doesn't exist. Create or use exsiting one in the player options app."};

    std::cout << "AppConfig::resourceDirectory : " << directory.string() << std::endl;
    resourceDirectory_ = directory;
}

FilePath AppConfig::configDirectory()
{
#if defined(SNAP_ENABLED)
//    return FilePath{getenv("SNAP_USER_COMMON")};
    return FilePath{getenv("XIBO_CONFIG_PATH")};
#elif defined(APPIMAGE_ENABLED)
    return FilePath{getenv("XDG_CONFIG_HOME")};
#elif defined(CUSTOM_CONFIG_DIR) // !!!cagri!!!
//    return FilePath{std::filesystem::path("/home/svrn/snap/savron-player/common")};
    std::cout << "XIBO_CONFIG_PATH : " << getenv("XIBO_CONFIG_PATH") << std::endl;
//    return FilePath{"/home/svrn/xibo_config"}; // WORKAROUND IN CASE ENV NOT SET
    return FilePath{getenv("XIBO_CONFIG_PATH")};
#else
    return execDirectory();
#endif
}

FilePath AppConfig::publicKeyPath()
{
    return configDirectory() / "id_rsa.pub";
}

FilePath AppConfig::privateKeyPath()
{
    return configDirectory() / "id_rsa";
}

FilePath AppConfig::cmsSettingsPath()
{
    return configDirectory() / "cmsSettings.xml";
}

FilePath AppConfig::playerSettingsPath()
{
    return configDirectory() / "playerSettings.xml";
}

FilePath AppConfig::schedulePath()
{
    return configDirectory() / "schedule.xml";
}

FilePath AppConfig::cachePath()
{
    return configDirectory() / "cacheFile.xml";
}

FilePath AppConfig::statsCache()
{
    return configDirectory() / "stats.sqlite";
}

FilePath AppConfig::additionalResourcesDirectory()
{
#if defined(SNAP_ENABLED)
    return FilePath{getenv("SNAP")} / "share" / "xibo-player";
#elif defined(APPIMAGE_ENABLED)
    return FilePath{getenv("APPDIR")} / "usr" / "share" / "xibo-player";
#elif defined(CUSTOM_CONFIG_DIR) // !!!cagri!!!
    std::cout << "additionalResourcesDirectory : " << configDirectory() / "resources" << std::endl;
    return configDirectory() / "resources";
#else
    return execDirectory();
#endif
}

FilePath AppConfig::splashScreenPath()
{
    return additionalResourcesDirectory() / "splash.jpg";
}

FilePath AppConfig::uiFile()
{
    return additionalResourcesDirectory() / "ui.glade";
}

FilePath AppConfig::execDirectory()
{
#if defined(SNAP_ENABLED)
    return FilePath{getenv("SNAP")} / "bin";
#elif defined(APPIMAGE_ENABLED)
    return FilePath{getenv("APPDIR")} / "usr" / "bin";
#else
    // workaround for those who starts the player out of snap and appimage
    char result[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
    assert(count != -1);
    return FilePath{std::filesystem::path(std::string(result, static_cast<size_t>(count))).parent_path()};
#endif
}

std::string AppConfig::playerBinary()
{
    return (execDirectory() / "xibo-player").string();
}

std::string AppConfig::optionsBinary()
{
    return (execDirectory() / "xibo-options").string();
}

// !!!cagri!!!
void AppConfig::setupNewConfigDir()
{
//#ifdef CUSTOM_CONFIG_DIR
//    try
//    {
//        const std::string CmsSettingsFile = "cmsSettings.xml";
//        const std::string PlayerSettingsFile = "playerSettings.xml";
//        const std::string PrivateKeyFile = "id_rsa";
//        const std::string PublicKeyFile = "id_rsa.pub";
//        const std::string ResourcesDir = "resources";
//        if (FileSystem::exists(AppConfig::oldConfigDirectory() / CmsSettingsFile))
//        {
//            CmsSettings settings;
//            settings.fromFile(AppConfig::oldConfigDirectory() / CmsSettingsFile);

//            std::vector<std::string> filesToMove{CmsSettingsFile, PlayerSettingsFile, PrivateKeyFile, PublicKeyFile};
//            for (auto&& file : filesToMove)
//            {
//                FileSystem::move(AppConfig::oldConfigDirectory() / file, AppConfig::configDirectory() / file);
//            }

//            if (settings.resourcesPath() == AppConfig::oldConfigDirectory() / ResourcesDir)
//            {
//                settings.resourcesPath().setValue(AppConfig::configDirectory() / ResourcesDir);
//                settings.saveTo(AppConfig::cmsSettingsPath());
//            }
//        }
//    }
//    catch (std::exception& e)
//    {
//        std::cout << "Error during setting up new config directory: " << e.what() << std::endl;
//    }
//#endif
}
