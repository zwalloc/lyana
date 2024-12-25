#pragma once

#include <ulib/yaml.h>
#include <filesystem>

#include "resolve_params.h"

namespace lyana
{
    namespace fs = std::filesystem;

    class target
    {
    public:
        target(const ulib::yaml &config, const fs::path &path, const fs::path &dir,
               const resolve_params &resolveParams);
        target(const ulib::yaml &config, target *pParent, const fs::path &path, const fs::path &dir,
               const resolve_params &resolveParams);

        const ulib::yaml &resolved_config() { return mResolvedConfig; }
        void add_child(std::unique_ptr<target> &&target) { mChilds.emplace_back(std::move(target)); }

        const ulib::list<std::unique_ptr<target>> &childs() { return mChilds; }

        ulib::string_view name() const { return mName; }
        ulib::string_view resolved_name() const { return mResolvedName; }

        target *find_child(ulib::string_view name) const
        {
            for (auto &t : mChilds)
                if (t->mName == name)
                    return t.get();

            return nullptr;
        }

        target *find(ulib::string_view name)
        {
            for (auto &t : mChilds)
                if (t->mName == name)
                    return t.get();

            return nullptr;
        }

        target *get_child(ulib::string_view targetName);
        // target* get_child(ulib::string_view targetName)
        // {
        //     return ;
        // }

        target *get_target(ulib::string_view targetName);

        target *get_visible_target(ulib::string_view targetName);

        const fs::path &dir() { return mDir; }

    private:
        void resolve(const resolve_params &params);

        // target properties without inheritance
        ulib::yaml mConfig;

        // target properties after applying inheritance, overriding and etc.
        ulib::yaml mResolvedConfig;

        // field from name property
        ulib::string mName;

        // combined name which is depends by parent target like "project.library"
        ulib::string mResolvedName;

        // target path (.yml)
        fs::path mPath;

        // target directory path
        fs::path mDir;

        // upper parent target
        target *mAdam;

        // // target which is the base of project
        // Target *mRoot;

        // parent target which properties are inherited
        target *mParent;

        // targets which parent is this instance
        ulib::list<std::unique_ptr<target>> mChilds;
    };
} // namespace lyana