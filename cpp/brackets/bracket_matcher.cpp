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


class stack_bracket_matcher : bracket_matcher {
        stack_bracket_matcher(string s) : bracket_matcher(s) {}

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

};

class recursive_bracket_matcher : bracket_matcher {

    recursive_bracket_matcher(string s) : bracket_matcher(s) {}
/*
    Have a recursive method 
    bool check_until(iterator, char c)
    runs until it encounters the character c
    returns true if it does, false if end of string

Input 		cur	

([]())       
[]())		(	calls check_until(')') # 1
]())		[	calls check_until(']') # 2
())			]	check_unil#2 returns true 
))			(	((
)			)	(
			)	empty


*/

bool recursive_bracket_test(string::iterator& cur, string::iterator& end, char expect) {
    while(cur!=end) { // loop through character of string, stop when at end
        char c = *cur; // get current character
        ++cur; // move to next character
        std::cout << "rbt <" << c << "> expect <" << expect << "> " << std::endl; 
        if(c == expect) { // termination conditiion
            std::cout << "matched\n";
            return true;
        }
        
        bool flag = true;
        switch(c) {
        case '(':
            flag = recursive_bracket_test(cur,end,')');
            break;
        case '[':
            flag = recursive_bracket_test(cur,end,']');
            break;
        case '{':
            flag = recursive_bracket_test(cur,end,'}');
            break;
        case ')': case ']': case '}':
            return false;
        }

        if(!flag) return false;
    }
    // now at end of string
    // if expect is not null char fails
    return expect == '\0';
}

bool nested_bracket_test(string str) {
    std::cout << "nbt <" << str << ">\n";
    string::iterator it = str.begin();
    string::iterator end = str.end();
    return recursive_bracket_test(it,end,'\0');
}

};