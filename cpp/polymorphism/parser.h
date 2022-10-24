#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <functional>

#include "pbyte.h"

using std::cout;
using std::endl;
using std::ostream;
using std::istream;
using std::string;
using std::vector;
using std::map;

namespace parser {
    enum state { none, var, op, num, op_var, op_num, var_op, num_op,
        num_op_num, num_op_var, var_op_num, var_op_var };

    const pbyte binary_op_flag = 0x01;
    const pbyte unary_op_flag =  0x02;
    const pbyte prefix_op_flag =  0x04;
    const pbyte postfix_op_flag =  0x08;
    
    const pbyte unary_prefix = unary_op_flag | prefix_op_flag;
    const pbyte unary_postfix = unary_op_flag | postfix_op_flag;
    
    class token {
    protected:
        state st;
        token(state st_in) : st{st_in} {}
    public:
        virtual void print_tok(ostream&) const =0;
        friend ostream& operator<<(ostream &os,token &tok) { tok.print_tok(os); return os; };
        virtual ~token() {}
        state get_state() const { return st; }
        string get_state_str() const;
    };

    class number_token : public token {
        pbyte value;
    public:
        number_token(pbyte p) : token{num}, value{p} {}
        void print_tok(ostream& os) const {
            os << "NT " << value;
        }
        pbyte get_value() const { return value; }
        ~number_token() {
            cout << "descruct "; print_tok(cout); cout << endl;
        }
    };

    class operator_token : public token {
        string symbol;
        pbyte flags;
        std::function<pbyte(pbyte,pbyte)> bfun;
    public:
        operator_token(const operator_token& ot) : token{op}, symbol{ot.symbol}, flags{ot.flags}, bfun{ot.bfun}  { }
        operator_token(const string s, const pbyte op_flags,
            std::function<pbyte(pbyte,pbyte)> fun)
            : token{op}, symbol{s}, flags{op_flags}, bfun{fun} {}
        bool match(const string& word) {
            return symbol == word;
        }
        void print_tok(ostream &os) const { os << "OT " << symbol; }
        
        pbyte eval(pbyte l, pbyte r) { return bfun(l,r); }
        ~operator_token() {
            cout << "descruct "; print_tok(cout); cout << endl;
        }
    };

    class variable_token : public token {
        string variable_name;
    public:
        variable_token(const string name) : token{var}, variable_name{name} { }
        void print_tok(ostream &os) const { os << "VT " << variable_name; }
        string get_name() const { return variable_name; }
        ~variable_token() {
            cout << "descruct "; print_tok(cout); cout << endl;
        }
    };

    vector<unique_ptr<token>> scan(const string &line);
    
    unique_ptr<token> get_token(const string &word);
    
    void add_operator(operator_token tok);
    
    void parse(vector<unique_ptr<token>> &tokens);
    
    token& getToken(vector<unique_ptr<token>>& tokens, int i);
    
    pbyte evaluate(vector<unique_ptr<token>> &tokens);
    
    
    
    class Bad_state {};

}