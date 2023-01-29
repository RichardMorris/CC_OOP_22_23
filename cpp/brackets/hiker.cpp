// The starting files are unrelated to the exercise.
//
// They simply show syntax for writing and testing
//  o) a global function
//  o) an instance method
// Pick the style that best fits the exercise.
// Then delete the other one, along with this comment!

#include "hiker.hpp"

int answer()
{
    return 42;
}

int hiker::answer() const
{
    return ::answer();
}
