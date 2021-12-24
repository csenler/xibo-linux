#pragma once

#include "common/fs/FilePath.hpp"

//#include "config/CmsSettings.hpp"
#include <iostream>

class AppConfig
{
public:
    static std::string version();
    static std::string releaseVersion();      // TODO: strong type
    static std::string codeVersion();  // TODO: strong type

    static FilePath resourceDirectory();
    static void resourceDirectory(const FilePath& directory);
    static FilePath execDirectory();
    static FilePath additionalResourcesDirectory();
    static FilePath configDirectory();

#ifdef SNAP_ENABLED
    static FilePath oldConfigDirectory()
    {
        return FilePath{getenv("SNAP_USER_DATA")};
    }
#elif defined(CUSTOM_CONFIG_DIR) // !!!cagri!!!
    static FilePath oldConfigDirectory()
    {
       return FilePath{getenv("XIBO_CONFIG_PATH")};
    }
#endif

    static FilePath publicKeyPath();
    static FilePath privateKeyPath();

    static FilePath splashScreenPath();
    static FilePath uiFile();

    static FilePath cmsSettingsPath();
    static FilePath playerSettingsPath();
    static FilePath schedulePath();
    static FilePath cachePath();
    static FilePath statsCache();

    static std::string playerBinary();
    static std::string optionsBinary();

    // !!!cagri!!!
    static void setupNewConfigDir();

private:
    static FilePath resourceDirectory_;
};
