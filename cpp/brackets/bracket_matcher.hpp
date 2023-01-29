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

public:
    bracket_matcher(string s);
    int num_bracket_count();
    int num_open_bracket();
    int num_close_bracket();
    bool is_balanced() const;
};

bool match_open_close_pair(char open, char close);