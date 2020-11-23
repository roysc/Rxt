#pragma once

#include <stdexcept>

namespace Rxt::gl
{
struct exception : std::exception {};

struct value_error : exception
{
    GLuint m_value;
    value_error(GLuint value) : m_value(value) {}
};

struct message_error : std::runtime_error
{
    using std::runtime_error::runtime_error;
};
}
