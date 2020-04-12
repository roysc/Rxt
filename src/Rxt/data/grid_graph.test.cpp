#include "grid_graph.hpp"
#include "../_testing.hpp"

namespace Rxt
{
TEST_CASE("grid_2d[clamp]")
{
    using G = grid_2d<>;
    G g{3, 3};

    G::vertex u{0u, 0u}, v{1u, 0u}, w{2u, 0u};

    {
        auto [e, ok] = edge(u, v, g);
        REQUIRE(ok);
        REQUIRE(e == G::edge{{0,0}, 0});
    } {
        auto [e, ok] = edge(w, u, g);
        REQUIRE(!ok);
    }
}
TEST_CASE("grid_2d[wrap]")
{
    using G = grid_2d<grid_boundary::wrap>;
    G g{3, 3};

    G::vertex u{0u, 0u}, v{1u, 0u}, w{2u, 0u};

    {
        auto [e, ok] = edge(u, v, g);
        REQUIRE(ok);
        REQUIRE(e == G::edge{{0,0}, 0});
    } {
        auto [e, ok] = edge(w, u, g);
        REQUIRE(ok);
        REQUIRE(e == G::edge{{2,0}, 0});
    }
}
}
