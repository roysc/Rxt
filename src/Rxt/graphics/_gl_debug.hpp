#pragma once
// #include "../_debug.hpp"
#include <vector>

namespace Rxt::gl
{
template <class Tr>
GLint _log_result(object<Tr>& obj, GLenum what, const char* subject)
{
    GLint result = GL_FALSE;
    int info_log_length = 0;

    (*Tr::getparam)(obj, what, &result);
    (*Tr::getparam)(obj, GL_INFO_LOG_LENGTH, &info_log_length);

    if (info_log_length > 1) {
        std::vector<char> message(info_log_length);
        (*Tr::getinfolog)(obj, info_log_length, nullptr, message.data());
        dbg::print("{0} [{1}]:\n{2}\n", subject, result, message.data());
    }
    return result;
}
}
