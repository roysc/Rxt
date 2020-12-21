#include "demangle.hpp"

// #include <stdexcept>
#include <cxxabi.h>

namespace Rxt
{
namespace {
// GCC-specific
// http://gcc.gnu.org/onlinedocs/libstdc++/libstdc++-html-USERS-4.3/a01696.html
enum error_code
{
    success      = 0,
    bad_alloc    = -1,
    invalid_name = -2,
    invalid_arg  = -3
};
}

char const* demangle(char const* name)
{
    int ret;
    char* demangled = abi::__cxa_demangle(name, nullptr, nullptr, &ret);
    error_code error = (error_code)ret;
    switch (error)
    {
    case error_code::success:
        return demangled;
    default:
        // throw std::runtime_error{__PRETTY_FUNCTION__};
        return name;
    }
}
}
