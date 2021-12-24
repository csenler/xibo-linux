#include "Bypass.hpp"

void Bypass::initialize()
{
    std::cout << "bypass::initialize" << std::endl;
    std::cout << "[DEBUG] cmSettingsPath : " << AppConfig::cmsSettingsPath().string() << std::endl;
    std::cout << "[DEBUG] playerSettings_ : " << AppConfig::playerSettingsPath().string() << std::endl;
    cmsSettings_.fromFile(AppConfig::cmsSettingsPath());
    playerSettings_.fromFile(AppConfig::playerSettingsPath());
}

int Bypass::initWithArguments(int argc, char* argv[])
{
    // std::cout << "Bypass::initWithArguments" << std::endl;
    int nRet = 1;
    int i = 1;

    if (argc != 7)
    {
        nRet = -1;
        return nRet;
    }

    while (i < argc)
    {
        if (i + 1 > argc)
        {
            break;
        }

        std::string strCurArg = std::string(argv[i]);
        if (std::string(argv[i]).empty())
        {
            nRet = 0;
            break;
        }

        if (strCurArg == "--cms")
        {
            cmsAddress = std::string(argv[i + 1]);
        }
        else if (strCurArg == "--key")
        {
            key = std::string(argv[i + 1]);
        }
        else if (strCurArg == "--lib")
        {
            resourcePath = std::string(argv[i + 1]);
        }

        i++;
    }

    if (nRet == 1)
    {
        auto keyHash = static_cast<Md5Hash>(System::hardwareKey());
        displayId = std::string(keyHash);
        std::cout << "displayID : " << displayId << std::endl;

        std::cout << "cms adress : " << cmsAddress << std::endl;
        std::cout << "key : " << key << std::endl;
        std::cout << "resourcePath : " << resourcePath << std::endl;

        auto connectionResult = connectToCms(cmsAddress, key, displayId);
        std::cout << "connectionResult : " << connectionResult << std::endl;
        updateSettings();
    }

    return nRet;
}

std::string Bypass::connectToCms(const std::string& cmsAddress, const std::string& key, const std::string& displayId)
{
    try
    {
        XmdsRequestSender xmdsRequester{cmsAddress, key, displayId};

        auto connectionResult =
            xmdsRequester
                .registerDisplay(AppConfig::codeVersion(), AppConfig::releaseVersion(), playerSettings_.displayName())
                .then([](auto future) {
                    auto [error, result] = future.get();

                    if (!error)
                        return result.status.message;
                    else
                        return error.message();
                });

        return connectionResult.get();
    }
    catch (std::exception& e)
    {
        return e.what();
    }
}

std::string Bypass::createDefaultResourceDir()
{
    auto defaultDir = AppConfig::configDirectory() / "resources";
    if (!FileSystem::exists(defaultDir))
    {
        bool result = FileSystem::createDirectory(defaultDir);
        if (!result)
        {
            Log::error("Unable to create resources directory");
        }
    }
    return defaultDir.string();
}

void Bypass::updateSettings()
{
    cmsSettings_.address().setValue(cmsAddress);
    cmsSettings_.key().setValue(key);
    std::string path = resourcePath;
    cmsSettings_.resourcesPath().setValue(path.empty() ? createDefaultResourceDir() : path);
    cmsSettings_.displayId().setValue(displayId);
    // cmsSettings_.updateProxy(domainField_->get_text(), usernameField_->get_text(), passwordField_->get_text());
    cmsSettings_.saveTo(AppConfig::cmsSettingsPath());
}
