#ifndef REQUIREDFILES_HPP
#define REQUIREDFILES_HP

#include "field.hpp"
#include "soap.hpp"

#include <variant>

namespace RequiredFiles
{
    class response
    {
    public:
        enum class FileType
        {
            Media,
            Layout,
            Resource,
            Invalid
        };

        struct required_file
        {
            enum DownloadType
            {
                HTTP,
                XMDS,
                Invalid
            };

            int id;
            size_t size;
            std::string md5;
            std::string path;
            std::string filename;
            DownloadType download_type;
            FileType file_type;
        };

        struct required_resource
        {
            int layout_id;
            int region_id;
            int media_id;
            FileType file_type = FileType::Resource;
        };

        const std::vector<required_file>& required_files() const;
        const std::vector<required_resource>& required_resources() const;

        void add_file(const boost::property_tree::ptree& attrs);

    private:
        std::vector<required_file> m_required_files;
        std::vector<required_resource> m_required_resources;
    };

    struct request
    {
        field<std::string> server_key{"serverKey"};
        field<std::string> hardware_key{"hardwareKey"};
    };
}

template<>
struct soap::request_traits<RequiredFiles::request>
{
    static inline const std::string name = "RequiredFiles";
    using response_t = RequiredFiles::response;
};


#endif // REQUIREDFILES_HPP
