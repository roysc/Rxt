#pragma once

#include "gl_prelude.hpp"
#include <stdexcept>

namespace Rxt::gl
{
struct exception : std::exception {};

struct value_error : exception
{
    GLuint m_value;
    mutable std::string m_buf;
    value_error(GLuint value) : m_value(value) {}
    const char* what() const noexcept { return (m_buf = std::to_string(m_value)).c_str(); }
};

struct message_error : std::runtime_error
{
    using std::runtime_error::runtime_error;
};
}
