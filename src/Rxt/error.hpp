#pragma once

namespace Rxt::util
{
struct result
{
    const char* message {};

    result(bool ok)
        : message(ok ? nullptr : "Unspecified error")
    {}

    operator bool() const { return !message; }
};
}
