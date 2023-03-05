#include <string>
using std::string;
class bracket_matcher
{
    string text;
    int open_count = 0; // ( 
    int closed_count = 0; // )
    int open_brace_count = 0; // {
    int closed_brace_count = 0; // }
    int open_square_count = 0; // [
    int closed_square_count = 0; // ]
    bool depth_check = false;
    // Don't want to allow end user to make this
protected:
    bracket_matcher(string s);
    bool nested_bracket_test(string text);

public:
    int num_bracket_count();
    int num_open_bracket();
    int num_close_bracket();
    bool is_balanced() const;
};

class bracket_matcher_factory 
{
    bracket_matcher make_bracket_matcher();    
};

bool match_open_close_pair(char open, char close);