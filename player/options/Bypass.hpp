#pragma once

//#include "Resources.hpp"
#include "config/CmsSettings.hpp"
#include "config/PlayerSettings.hpp"

#include "cms/xmds/XmdsRequestSender.hpp"
#include "common/fs/FileSystem.hpp"
#include "common/logger/Logging.hpp"
#include "common/system/System.hpp"
#include "config/AppConfig.hpp"

class Bypass
{
public:
    int initialize();
    int initWithArguments(int argc, char** argv);

private:
    CmsSettings cmsSettings_;
    PlayerSettings playerSettings_;
};
