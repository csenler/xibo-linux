#include "Bypass.hpp"

int Bypass::initialize()
{
    cmsSettings_.fromFile(AppConfig::cmsSettingsPath());
    playerSettings_.fromFile(AppConfig::playerSettingsPath());

    return 1;
}

int Bypass::initWithArguments(int argc, char** argv)
{
    int nRet = 1;
    int i = 1;
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

        // if (strCurArg == "")

        //                auto connectionResult = connectToCms(cmsAddressField_->get_text(), keyField_->get_text(),
        //                displayId);

        //                connectionStatus_->set_text(connectionResult);
        //                displayIdField_->set_text(displayId);

        //                updateSettings();

        i++;
    }

    return nRet;
}
