#pragma once

#include <ulib/yaml.h>
#include <filesystem>

#include "resolve_params.h"

namespace lyana
{
    namespace fs = std::filesystem;


    class target;
    class parser
    {
    public:
        parser(ulib::u8string_view configName, const resolve_params& resolveParams);

        ulib::yaml parse_directory(const fs::path &dir);
        std::unique_ptr<target> parse_directory_tree(const fs::path &dir, target* parent);
        bool is_target_directory(const fs::path &path);

    private:

        void parse_directory_tree(target* parent, const fs::path& dir);

        ulib::u8string mConfigName;
        ulib::u8string mConfigAddonExt;
        resolve_params mResolveParams;
    };
}