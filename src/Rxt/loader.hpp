#pragma once

#include <filesystem>

namespace Rxt
{
struct file_loader
{
    using path = std::filesystem::path;
    path root_path = "";

    path root() const
    {
        return root_path;
    }
};
}
