#include <iostream>

#include <lyana/target.h>
#include <lyana/parser.h>

void print_target_tree(lyana::target &target, size_t tabs = 0)
{
    ulib::string tab;
    for (size_t i = 0; i != tabs; i++)
        tab += "  ";

    fmt::print("{} - {}\n", tab, target.resolved_name());
    for (auto &obj : target.childs())
    {
        print_target_tree(*obj, tabs + 1);
    }
}

int main()
{
    try
    {
        lyana::resolve_params resolve;
        resolve.overridePrefix = "override.";

        lyana::parser parser(u8"re.yml", resolve);
        auto target = parser.parse_directory_tree(std::filesystem::current_path(), nullptr);

        fmt::print("child count: {}\n", target->childs().size());
        fmt::print("{}\n", target->resolved_config().dump());

        print_target_tree(*target);

        if (lyana::target *lib = target->get_child("lib"))
        {
            fmt::print("{}\n", lib->name());

            if (auto tgt = lib->get_visible_target("targeter.futile-project.futile"))
            {
                fmt::print("{}\n", tgt->resolved_name());
            }
            // fmt::print("{}\n", tar->resolved_config().dump());
        }
    }
    catch (const std::exception &ex)
    {
        fmt::print("exception: {}\n", ex.what());
    }

    return 0;
}
