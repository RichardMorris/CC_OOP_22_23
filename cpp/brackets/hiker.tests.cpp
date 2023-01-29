// For more on how to use the excellent Catch test framework see
// https://github.com/catchorg/Catch2

#include "hiker.hpp"
#include <catch2/catch.hpp>

TEST_CASE("global function example")
{
    REQUIRE( answer() == 42 );
}

TEST_CASE("instance method example")
{
    REQUIRE( hiker().answer() == 42 );
}
