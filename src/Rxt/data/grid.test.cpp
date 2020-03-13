#include "grid.hpp"
#include "Rxt/_testing.hpp"

namespace Rxt
{
TEST_CASE("grid traits") {
    using C10C10 = grid_spec<clamp_axis<10>, clamp_axis<10>>;
    using W10W10 = grid_spec<wrap_axis<10>, wrap_axis<10>>;
    using C10W10 = grid_spec<clamp_axis<10>, wrap_axis<10>>;

    REQUIRE( C10C10::extent[0] == 10 );
    REQUIRE( C10C10::extent[1] == 10 );
    REQUIRE( !C10C10::is_wrapped[0] );
    REQUIRE( !C10C10::is_wrapped[1] );

    REQUIRE( C10W10::extent[0] == 10 );
    REQUIRE( C10W10::extent[1] == 10 );
    REQUIRE( !C10W10::is_wrapped[0] );
    REQUIRE( C10W10::is_wrapped[1] );

    REQUIRE( W10W10::extent[0] == 10 );
    REQUIRE( W10W10::extent[1] == 10 );
    REQUIRE( W10W10::is_wrapped[0] );
    REQUIRE( W10W10::is_wrapped[1] );
}
}
