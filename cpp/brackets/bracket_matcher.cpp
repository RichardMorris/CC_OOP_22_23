#include "bracket_matcher.hpp"
#include <stack>
#include <iostream>

using std::stack;
/*
Input 		cur	Stack

([]())       
[]())		(	(
]())		[	([
())			]	(		closing bracket input matches top of stack, hence pop 
))			(	((
)			)	(
			)	empty
            */

/*
    Returns true if we have ( )  or [ ]  or { }
    all other cases return false
*/
bool match_open_close_pair(char left, char right) {
    return ( (left == '(' && right == ')') 
      || (left == '{' && right == '}') 
      ||  (left == '[' && right == ']') );
}

bool nested_bracket_test(string str) {
    stack<char> stack;

    for (char c : str)
    {
        if( c=='(' || c == '[' || c == '{' ) {
            stack.push(c);
        }
        if( c==')' || c == ']' || c == '}' ) {
            if(stack.empty()) return false; 
            
            if(match_open_close_pair(stack.top(), c)){
                stack.pop();
            }
            else {
                return false; 
            }
        }
    }
    return stack.empty();
}

bracket_matcher::bracket_matcher(string s) : text{s} { 
    int depth=0;
    depth_check= nested_bracket_test(text); 
    
    for (char c : text)
    {
        switch (c) {
        case '(':
            open_count++;
            ++depth;
            break;
        case ')':
            closed_count++;
            --depth;
            break;
        case '[':
            open_square_count++;
            ++depth;
            break;
        case ']':
            closed_square_count++;
            --depth;
            break;
        case '{':
            open_brace_count++;
            ++depth;
            break;
        case '}':
            closed_brace_count++;
            --depth;
            break;
        }
      //  if(depth<0) depth_check = false;
    }
    //if(depth != 0) depth_check = false;
};


bool bracket_matcher::is_balanced() const
{
    return depth_check;
}

int bracket_matcher::num_open_bracket() {
    return open_count;
}
int bracket_matcher::num_close_bracket() {
    return closed_count;
}

int bracket_matcher::num_bracket_count()
{
    return open_count + closed_count;
}