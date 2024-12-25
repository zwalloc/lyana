#include "target.h"

#include <ulib/strutility.h>

namespace lyana
{
    target::target(const ulib::yaml &config, const fs::path &path, const fs::path &dir,
                   const resolve_params &resolveParams)
        : mParent(nullptr), mAdam(this), mConfig(config)
    {
        mPath = path;
        mDir = dir;

        resolve(resolveParams);
    }

    target::target(const ulib::yaml &config, target *pParent, const fs::path &path, const fs::path &dir,
                   const resolve_params &resolveParams)
        : mParent(pParent), mAdam(pParent ? pParent->mAdam : this), mConfig(config)
    {
        mPath = path;
        mDir = dir;

        resolve(resolveParams);
    }

    target *target::get_child(ulib::string_view targetName)
    {
        auto names = targetName.split(".");

        target *current = this;
        auto it = names.begin();
        auto end = names.end();
        while (it != end)
        {
            current = current->find_child(*it);
            if (!current)
                return nullptr;
            it++;
        }

        if (current == this)
            return nullptr;

        return current;
    }

    target *target::get_target(ulib::string_view targetName) 
    { 
        return nullptr;
    }

    target *target::get_visible_target(ulib::string_view targetName)
    {
        // ulib::list<ulib::string> names = targetName.split(".");

        target *current = mParent;
        while (current)
        {
            if (target *result = current->get_child(targetName))
                return result;

            if (current->mParent == nullptr)
            {
                ulib::list<ulib::string> names = targetName.split(".");
                if (!names.empty())
                {
                    if (current->name() == names[0])
                    {
                        if (names.size() == 1)
                            return current;

                        names.erase(names.begin());
                        ulib::string child = ulib::join(names, ".");
                        if (!child.empty())
                            return current->get_child(child);
                    }
                }
            }

            current = current->mParent;
        }

        return nullptr;
    }

    void target::resolve(const resolve_params &params)
    {
        mName = mConfig["name"].scalar();

        ulib::string_view nameView = mName;
        while (nameView.starts_with("."))
            nameView.remove_prefix(1);
        mName = ulib::string{nameView};

        if (mParent)
        {
            // auto name = mConfig.search("name");
            // if (!name)
            //     throw ulib::RuntimeError{"required: name"};
            // if (!name->is_scalar())
            //     throw ulib::RuntimeError{"name required to be a scalar"};
            // mName = name->scalar();

            mResolvedConfig = mParent->mResolvedConfig;
            MergeYamlNode(mResolvedConfig, mConfig);

            mResolvedName = mParent->mResolvedName + "." + mName;
        }
        else
        {
            mResolvedConfig = mConfig;
            mResolvedName = mName;
        }
    }

} // namespace targeter