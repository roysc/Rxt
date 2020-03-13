#pragma once

namespace Rxt
{
struct runtime
{
    bool _running = true;

    bool should_quit() const { return !_running; }
    void should_quit(bool v) { _running = !v; }
};

using simple_runtime = runtime;
}
