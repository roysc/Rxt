#pragma once
#include <stdexcept>

namespace Rxt::util
{
struct result
{
    const char* message {};

    result(bool ok)
        : message(ok ? nullptr : "unspecified error")
    {}

    operator bool() const { return !message; }
};

struct _unimplemented : std::exception
{
    const char* what() const throw() {return "unimplemented";}
};
}
