//#include <catch2/catch.hpp>
#include <catch2/catch_test_macros.hpp>
#include <deque>
#include <iostream>
using std::deque;
/*
class Pair {
public:
    int first;
    int last;
    Pair(int a,int b) : first(a), last(b) {} 
};
*/
TEST_CASE("DQ Create a stack")
{
    deque<int> stack;
    stack.push_back(42);
    int& res = stack.back();
    REQUIRE( 42 == res );
}

void showdq(deque<int> g)
{
    deque<int>::iterator it;
    std::cout << "Curent stack" << g.size() << "\n";
    for (it = g.begin(); it != g.end(); ++it)
        std::cout << '\t' << *it;
    std::cout << '\n';
}

TEST_CASE("DQ Pushing and popping")
{
    deque<int> stack;
    showdq(stack);
    stack.push_back(42);
    showdq(stack);
    stack.push_back(123);
    showdq(stack);
    stack.pop_back();
    showdq(stack);
    REQUIRE( 42 == stack.back() );
}

TEST_CASE("DQ insert element")
{
    deque<int> mydeque;
    mydeque.push_back(42);
    mydeque.push_back(123);
    deque<int>::iterator it;
    it = mydeque.begin();
    int val0 = *it;
    REQUIRE( 42 == val0 );
    ++it;

    mydeque.insert(it,53);

    deque<int>::iterator it2;
    it2 = mydeque.begin();
    int val1 = *it;
    REQUIRE( 42 == val0 );
    ++it2;
    int val2 = *it2;
    REQUIRE( 53 == val2 );
    ++it2;
    int val3 = *it2;
    REQUIRE( 123 == val3 );
}


/*
TEST_CASE("DQ A stack of doubles")
{
    deque<double> stack;
    stack.push_back(1.23456);
    double& ref = stack.back();
    REQUIRE( ref == Approx(1.23456) );
}

TEST_CASE("DQ A stack of pairs")
{
    deque<Pair> stack;
    Pair p1{2,3};
    stack.push_back(p1);
//    REQUIRE( stack.size() == 1);
    Pair& p2 = stack.back();
//    REQUIRE( p2.first ==2);
    stack.pop_back();
//    REQUIRE( stack.size() == 0);
//    REQUIRE( stack.empty());
    
}
*/