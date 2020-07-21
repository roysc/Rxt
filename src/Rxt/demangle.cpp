#include "demangle.hpp"

#include <cxxabi.h>

namespace rcxt {

namespace {
// GCC-specific
// http://gcc.gnu.org/onlinedocs/libstdc++/libstdc++-html-USERS-4.3/a01696.html
enum Error
{
    success      = 0,
    bad_alloc    = -1,
    invalid_name = -2,
    invalid_arg  = -3
};
}

const char*
demangle(const char* name)
{
    int ret;
    char* demangled = abi::__cxa_demangle(name, nullptr, nullptr, &ret);

    Error error = (Error)(ret);
    switch (error)
    {
    case success:
        return demangled;

    default:
        // throw std::runtime_error{__PRETTY_FUNCTION__};
        return name;
    }
}

}
