#pragma once

#include "parser.h"

#include <ulib/format.h>
#include <ulib/fmt/path.h>

#include "yaml_merge.h"
#include <futile/futile.h>

#include "target.h"

namespace lyana
{
    parser::parser(ulib::u8string_view configName, const resolve_params &resolveParams)
        : mConfigName(configName), mConfigAddonExt(u8"." + configName), mResolveParams(resolveParams)
    {
    }

    bool parser::is_target_directory(const fs::path &path)
    {
        auto srvymlPath = path / mConfigName;
        return fs::exists(srvymlPath);
    }

    ulib::yaml parser::parse_directory(const fs::path &path)
    {
        if (!fs::is_directory(path))
            throw ulib::RuntimeError(ulib::format("[ParseTargetDirectory] path: {} must be a directory", path));

        auto srvymlPath = path / mConfigName;
        if (!fs::exists(srvymlPath)) // "./srv.yml"
            throw ulib::RuntimeError(ulib::format("[ParseTargetDirectory] path: {} has not a valid target file", path));

        ulib::yaml result = ulib::yaml::parse(futile::open(srvymlPath).read());
        if (result.is_null())
            throw ulib::RuntimeError(ulib::format("Target {} is null", srvymlPath));

        for (const auto &entry : fs::directory_iterator(path))
        {
            if (!entry.is_regular_file())
                continue;

            auto u8path = ulib::u8string{entry.path().u8string()};
            if (u8path.ends_with(mConfigAddonExt)) // "something.srv.yml"
            {
                ulib::yaml addonyml = ulib::yaml::parse(futile::open(entry.path()).read());
                if (addonyml.is_map())
                    result = MergeYamlNodes(result, addonyml);
            }
        }

        return result;
    }

    void parser::parse_directory_tree(target *parent, const fs::path &dir)
    {
        for (const auto &entry : fs::directory_iterator(dir))
        {
            if (!fs::is_directory(entry.path()))
                continue;

            if (is_target_directory(entry.path()))
            {
                auto target = parse_directory_tree(entry.path(), parent);
                parent->add_child(std::move(target));
            }
            else
            {
                parse_directory_tree(parent, entry.path());
            }
        }
    }

    std::unique_ptr<target> parser::parse_directory_tree(const fs::path &dir, target *pParent)
    {
        ulib::yaml rootyml = parse_directory(dir);
        std::unique_ptr<target> result = std::make_unique<target>(rootyml, pParent, dir / mConfigName, dir, mResolveParams);

        for (const auto &entry : fs::directory_iterator(dir))
        {
            if (!entry.is_directory())
                continue;

            if (is_target_directory(entry.path()))
            {
                auto target = parse_directory_tree(entry.path(), result.get());
                result->add_child(std::move(target));
            }
            else
            {
                parse_directory_tree(result.get(), entry.path());
            }
        }

        return result;
    }
} // namespace targeter