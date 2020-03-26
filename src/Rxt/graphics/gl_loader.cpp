#include "gl_loader.hpp"

#include <memory>

namespace Rxt::gl
{
file_loader const& thread_file_loader()
{
    thread_local std::unique_ptr<file_loader> ret;

    if (!ret) {
        ret.reset(new file_loader {});
    }
    return *ret;
}
}
