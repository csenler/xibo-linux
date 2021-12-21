#pragma once

//#include "Resources.hpp"
#include "config/CmsSettings.hpp"
#include "config/PlayerSettings.hpp"

#include "cms/xmds/XmdsRequestSender.hpp"
#include "common/fs/FileSystem.hpp"
#include "common/logger/Logging.hpp"
#include "common/system/System.hpp"
#include "config/AppConfig.hpp"
#include <iostream>

class Bypass
{
public:
    void initialize();
    int initWithArguments(int argc, char* argv[]);

    std::string connectToCms(const std::string& cmsAddress, const std::string& key, const std::string& displayId);
    void updateSettings();
    std::string createDefaultResourceDir();

private:
    CmsSettings cmsSettings_;
    PlayerSettings playerSettings_;

    std::string cmsAddress;
    std::string key;
    std::string displayId;
    std::string resourcePath;
};
