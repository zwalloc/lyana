#pragma once

#include <ulib/yaml.h>
#include <futile/futile.h>
#include <filesystem>
#include <ulib/string.h>
#include <ulib/format.h>

#include "yaml_merge.h"

namespace lyana
{
    namespace fs = std::filesystem;

    // inline bool IsTargetDirectory(const fs::path &path, ulib::string_view filename)
    // {
    //     auto srvymlPath = path / filename;
    //     return fs::exists(srvymlPath);
    // }

    struct resolve_params
    {
        ulib::string overridePrefix;
        ulib::list<ulib::string> overrideAlways;
    };

    // class Target;
    // class TargetResolver
    // {
    // public:
    //     TargetResolver(const TargetResolverParams& params)
    //     {
    //         mParams = params;
    //     }

    //     void Resolve(ulib::yaml& resolvedConfig, ulib::yaml& config)
    //     {
    //         resolvedConfig = config;
    //     }

    // private:
    //     TargetResolverParams mParams;
    // };
   
} // namespace netunit