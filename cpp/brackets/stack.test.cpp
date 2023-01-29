// //#include <catch2/catch_all.hpp>
// #define CONFIG_CATCH_MAIN
// #include "catch.hpp"
#include <catch2/catch_test_macros.hpp>

#include <stack>
using std::stack;

class Pair {
public:
    int first;
    int last;
    Pair(int a,int b) : first(a), last(b) {} 
};
TEST_CASE("Create a stack")
{
    stack<int> stack;
    stack.push(42);
    REQUIRE( 42 == stack.top() );
}

TEST_CASE("Pushing and popping")
{
    stack<int> stack;
    stack.push(42);
    stack.push(123);
    stack.pop();
    REQUIRE( 42 == stack.top() );
}


TEST_CASE("A stack of pairs")
{
    stack<Pair> stack;
    Pair p1{2,3};
    stack.push(p1);
    REQUIRE( stack.size() == 1);
    Pair& p2 = stack.top();
    REQUIRE( p2.first ==2);
    stack.pop();
    REQUIRE( stack.size() == 0);
    REQUIRE( stack.empty());
    
}