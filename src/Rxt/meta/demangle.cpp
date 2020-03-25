#include <cxxabi.h>

namespace Rxt
{
// // GCC-specific
// // http://gcc.gnu.org/onlinedocs/libstdc++/libstdc++-html-USERS-4.3/a01696.html
// enum demangle_error
// {
//     success      = 0,
//     bad_alloc    = -1,
//     invalid_name = -2,
//     invalid_arg  = -3
// };

const char* demangle(const char* name)
{
    int ret;
    char* demangled = abi::__cxa_demangle(name, nullptr, nullptr, &ret);
    if (ret < 0) {
        return name;            // fixme?
    }
    return demangled;
}
}
