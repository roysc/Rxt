#include "loader.hpp"

#include <memory>

namespace Rxt
{
file_loader const& default_file_loader()
{
    thread_local std::unique_ptr<file_loader> ret;

    if (!ret) {
        ret.reset(new file_loader {});
    }
    return *ret;
}
}
