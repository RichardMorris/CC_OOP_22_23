#include "bracket_matcher.hpp"
#include <catch2/catch_test_macros.hpp>
#define SKIP_BAD true
TEST_CASE("Brackets class initialisation")
{
    bracket_matcher bm1("");
}

TEST_CASE("Empty string is balanced")
{
    bracket_matcher bm1("");
    REQUIRE( bm1.is_balanced() );
}

TEST_CASE("Single character not bracket string is balanced")
{
    bracket_matcher bm1("a");
    REQUIRE( bm1.is_balanced() );
}

TEST_CASE("Single open bracket character fails")
{
    bracket_matcher bm1("(");
    REQUIRE( ! bm1.is_balanced() );
}

TEST_CASE("Single close bracket character fails")
{
    bracket_matcher bm1(")");
    REQUIRE( ! bm1.is_balanced() );
}

TEST_CASE("Single open square bracket character fails")
{
    bracket_matcher bm1("[");
    REQUIRE( ! bm1.is_balanced() );
}

TEST_CASE("Pair of open & close square bracket character passes")
{
    bracket_matcher bm1("[]");
    REQUIRE( bm1.is_balanced() );
}

TEST_CASE("Pair of open square bracket and cloded brace fails")
{
    if(SKIP_BAD) return;
    bracket_matcher bm1("[}");
    REQUIRE( !bm1.is_balanced() );
}

TEST_CASE("Revesed Pair of open & close square bracket character fails")
{
    bracket_matcher bm1("][");
    REQUIRE( ! bm1.is_balanced() );
}

TEST_CASE("Sequence ((())()) passes")
{
    bracket_matcher bm1("((())())");
    REQUIRE( bm1.is_balanced() );
}


TEST_CASE("Count total number of open and close brackets")
{
    REQUIRE( bracket_matcher("").num_bracket_count() == 0 );
    REQUIRE( bracket_matcher("a").num_bracket_count() == 0 );
    REQUIRE( bracket_matcher("(").num_bracket_count() == 1 );
    REQUIRE( bracket_matcher("()").num_bracket_count() == 2 );
    REQUIRE( bracket_matcher("a(").num_bracket_count() == 1 );
    REQUIRE( bracket_matcher("(a").num_bracket_count() == 1 );
    REQUIRE( bracket_matcher("((").num_bracket_count() == 2 );
    REQUIRE( bracket_matcher("(((").num_bracket_count() == 3 );
    REQUIRE( bracket_matcher("a(b(c(d").num_bracket_count() == 3 );

    REQUIRE( bracket_matcher(")").num_bracket_count() == 1 );
    REQUIRE( bracket_matcher("a)").num_bracket_count() == 1 );
    REQUIRE( bracket_matcher(")a").num_bracket_count() == 1 );
    REQUIRE( bracket_matcher("))").num_bracket_count() == 2 );
    REQUIRE( bracket_matcher(")))").num_bracket_count() == 3 );
    REQUIRE( bracket_matcher("a)b)c)d").num_bracket_count() == 3 );
}

TEST_CASE("Count total number of open brackets")
{
    REQUIRE( bracket_matcher("").num_open_bracket() == 0 );
    REQUIRE( bracket_matcher("a").num_open_bracket() == 0 );
    REQUIRE( bracket_matcher("(").num_open_bracket() == 1 );
    REQUIRE( bracket_matcher("()").num_open_bracket() == 1 );
    REQUIRE( bracket_matcher("a(").num_open_bracket() == 1 );
    REQUIRE( bracket_matcher("(a").num_open_bracket() == 1 );
    REQUIRE( bracket_matcher("((").num_open_bracket() == 2 );
    REQUIRE( bracket_matcher("(((").num_open_bracket() == 3 );
    REQUIRE( bracket_matcher("a(b(c(d").num_open_bracket() == 3 );
}

TEST_CASE("Count total number of close brackets")
{
    REQUIRE( bracket_matcher("").num_close_bracket() == 0 );
    REQUIRE( bracket_matcher("a").num_close_bracket() == 0 );
    REQUIRE( bracket_matcher("(").num_close_bracket() == 0 );
    REQUIRE( bracket_matcher(")").num_close_bracket() == 1 );
    REQUIRE( bracket_matcher("()").num_close_bracket() == 1 );
    REQUIRE( bracket_matcher("a)").num_close_bracket() == 1 );
    REQUIRE( bracket_matcher("(a)").num_close_bracket() == 1 );
    REQUIRE( bracket_matcher("(())").num_close_bracket() == 2 );
    REQUIRE( bracket_matcher(")))").num_close_bracket() == 3 );
    REQUIRE( bracket_matcher("a)b)c)d").num_close_bracket() == 3 );
}

TEST_CASE("Nested () and [] passes")
{
    bracket_matcher bm1("([])");
    REQUIRE( bm1.is_balanced() );
}

TEST_CASE("Incorrectly nested () and [] fails")
{
        if(SKIP_BAD) return;

    bracket_matcher bm1("([)]");
    REQUIRE(! bm1.is_balanced() );
}

TEST_CASE("Test match_open_close_pair")
{
    REQUIRE( match_open_close_pair('(',')') );
    REQUIRE( match_open_close_pair('[',']') );
    REQUIRE( match_open_close_pair('{','}') );

    REQUIRE( !match_open_close_pair('(',']') );
    REQUIRE( !match_open_close_pair('(','}') );

    REQUIRE( !match_open_close_pair('[',')') );
    REQUIRE( !match_open_close_pair('[','}') );

    REQUIRE( !match_open_close_pair('{',')') );
    REQUIRE( !match_open_close_pair('{',']') );
}

