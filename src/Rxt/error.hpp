#pragma once

#include <stdexcept>

namespace Rxt
{
struct result
{
    const char* m_message {};
    result(bool ok) : m_message(ok ? nullptr : "unspecified error") {}
    operator bool() const { return !m_message; }
};

struct unimplemented_error : std::exception
{
    const char* what() const noexcept {return "unimplemented";}
};
}
