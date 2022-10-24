#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <memory>

#include "pbyte.h"

using std::cout;
using std::endl;
using std::ostream;
using std::istream;
using std::string;
using std::vector;
using std::map;

// All code here placed in name-space
namespace parser {
	// Indicates what type of operator we use
    enum state { var, op, num };
    // Flag to determine what type of operator we have
    const pbyte binary_op_flag = 0x01;
    const pbyte unary_op_flag =  0x02;
    const pbyte prefix_op_flag =  0x04;
    const pbyte postfix_op_flag =  0x08;
    const pbyte assignment_op = 0x10;
    
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
//            cout << "descruct "; print_tok(cout); cout << endl;
        }
    };

    class operator_token : public token {
    private:
        string sym;
        pbyte flags;
        std::function<pbyte(pbyte,pbyte)> bfun;
    public:
        operator_token(const operator_token& ot) :
        	token{op}, sym{ot.sym}, flags{ot.flags}, bfun{ot.bfun}  { }
        operator_token(const char* s, const pbyte& op_flags, const std::function<pbyte(pbyte,pbyte)>& fun)
            : token{op}, sym{s}, flags{op_flags}, bfun{fun} {}
        bool match(const string& word) const {
            return sym == word;
        }
        void print_tok(ostream &os) const { os << "OT " << sym; }
        
        pbyte eval(pbyte l, pbyte r) const { return bfun(l,r); }
        ~operator_token() {
//            cout << "destruct "; print_tok(cout); cout << endl;
        }

        std::unique_ptr<operator_token> unique_copy() const {
        	auto res = std::make_unique<operator_token>(*this);
        	return res;
        }

    };

    class variable_token : public token {
        string variable_name;
    public:
        variable_token(const string& name) : token{var}, variable_name{name} { }
        void print_tok(ostream &os) const { os << "VT " << variable_name; }
        string get_name() const { return variable_name; }
        ~variable_token() {
//            cout << "descruct "; print_tok(cout); cout << endl;
        }
    };

    // Methods available in this namespace

    // scan the input creating a list of tokens
    vector<unique_ptr<token>> scan(const string &line);
    
    // get the token for a word
    unique_ptr<token> get_token(const string &word);
    
    // add an operator to the list of known operators
    void add_operator(operator_token* tok);
    
    // get a reference to a token in the list
    token& getToken(vector<unique_ptr<token>>& tokens, int i);
    
    // evaluate a list of tokens
    pbyte evaluate(vector<unique_ptr<token>> &tokens);
    
    
    // Represent an error
    class Bad_state {
    	string message;
    public:
    	Bad_state(const string& str) : message{str} {}
    	string get_message() const { return message; }
    };

    class unmatched_token {
        string word;
    public:
        unmatched_token(string s) : word{s} {}
    	string get_message() const { return word; }
    };

}
