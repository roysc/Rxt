#pragma once

#include "gl_core.hpp"

#include <utility>
#include <vector>

namespace Rxt::gl
{
template <class... T>
static void log(T&&... args)
{
    if (!config().enable_logging) return;
    print(std::forward<T>(args)...);
}

template <class Tr>
GLint log_result(object<Tr>& obj, GLenum what, const char* subject)
{
    GLint result = GL_FALSE;
    int info_log_length = 0;

    (*Tr::get_param)(obj, what, &result);
    (*Tr::get_param)(obj, GL_INFO_LOG_LENGTH, &info_log_length);

    if (info_log_length > 1) {
        std::vector<char> message(info_log_length);
        (*Tr::get_info_log)(obj, info_log_length, nullptr, message.data());
        log("{0} [{1}]:\n{2}\n", subject, result, message.data());
    }
    return result;
}
}
