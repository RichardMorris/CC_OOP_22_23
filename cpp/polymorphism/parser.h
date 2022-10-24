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
//    enum state { num, var, bop, uop, aop  };
    // Flag to determine what type of operator we have
    const pbyte binary_op_flag = 0x01;
    const pbyte unary_op_flag =  0x02;
    const pbyte prefix_op_flag =  0x04;
    const pbyte postfix_op_flag =  0x08;
    const pbyte assignment_op = 0x10;
    const pbyte number_tok = 0x40;
    const pbyte var_tok = 0x80;
    
    const pbyte unary_prefix = unary_op_flag | prefix_op_flag;
    const pbyte unary_postfix = unary_op_flag | postfix_op_flag;
    
    class token {
    protected:
        pbyte state;
        token(pbyte st_in) : state{st_in} {}
    public:
        virtual void print_tok(ostream&) const =0;
        friend ostream& operator<<(ostream &os,token &tok) { tok.print_tok(os); return os; };
        virtual ~token() {}
        bool test_state(pbyte st) const {
        	pbyte pb = state & st;
        	bool res = pb == st;
        	return res;
        }
    };

    class number_token : public token {
        pbyte value;
    public:
        number_token(pbyte p) : token{number_tok}, value{p} {}
        void print_tok(ostream& os) const {
            os << "NT " << value;
        }
        pbyte get_value() const { return value; }
        ~number_token() {
//            cout << "descruct "; print_tok(cout); cout << endl;
        }
    };

    class variable_token : public token {
        string variable_name;
    public:
        variable_token(const string& name) : token{var_tok}, variable_name{name} { }
        void print_tok(ostream &os) const { os << "VT " << variable_name; }
        string get_name() const { return variable_name; }
        ~variable_token() {
//            cout << "descruct "; print_tok(cout); cout << endl;
        }
    };

    class operator_token : public token {
    protected:
        string sym;
        operator_token(const operator_token& ot)
        	: token{ot.state}, sym{ot.sym} { }
        operator_token(const pbyte& flags, const string& s)
            : token{flags}, sym{s} {}
     public:
        bool match(const string& word) const {
            return sym == word;
        }
        void print_tok(ostream &os) const { os << "OT " << sym; }
        pbyte get_flags() const { return state; }
        ~operator_token() {
//            cout << "destruct "; print_tok(cout); cout << endl;
        }

        virtual std::unique_ptr<operator_token> unique_copy() const =0; //{ auto res = std::make_unique<operator_token>(*this); return res; }

    };

    class unary_op_token : public operator_token {
        std::function<pbyte(pbyte)> ufun;
    public:
        unary_op_token(const char* s, const pbyte& op_flags, const std::function<pbyte(pbyte)>& fun)
        	: operator_token{op_flags, s}, ufun{fun} {}
        unary_op_token(const unary_op_token& ot) :
            	operator_token{ot.state, ot.sym}, ufun{ot.ufun} { }

        pbyte eval(pbyte l) const { return ufun(l); }
        void print_tok(ostream &os) const { os << "BOT " << sym; }

        std::unique_ptr<operator_token> unique_copy() const {
        	auto res = std::make_unique<unary_op_token>(*this);
        	return res;
        }
    };

    class binary_op_token : public operator_token {
        std::function<pbyte(pbyte,pbyte)> bfun;
    public:
        binary_op_token(const char* s, const pbyte& op_flags, const std::function<pbyte(pbyte,pbyte)>& fun)
        	: operator_token{op_flags, s}, bfun{fun} {}
        binary_op_token(const binary_op_token& ot) :
            	operator_token{ot.state, ot.sym}, bfun{ot.bfun} { }

        pbyte eval(pbyte l, pbyte r) const { return bfun(l,r); }
        void print_tok(ostream &os) const { os << "BOT " << sym; }

        std::unique_ptr<operator_token> unique_copy() const {
        	auto res = std::make_unique<binary_op_token>(*this);
        	return res;
        }

    };

    class assign_op_token : public operator_token {
        std::function<pbyte(pbyte,pbyte)> afun;
    public:
        assign_op_token(const char* s, const pbyte& op_flags, const std::function<pbyte(pbyte,pbyte)>& fun)
        	: operator_token{op_flags, s}, afun{fun} {}
        assign_op_token(const assign_op_token& ot)
        	: operator_token{ot.state, ot.sym}, afun{ot.afun} { }

        byte eval(pbyte varval, pbyte r) const { return afun(varval, r); }
        void print_tok(ostream &os) const { os << "AOT " << sym; }

        std::unique_ptr<operator_token> unique_copy() const {
        	auto res = std::make_unique<assign_op_token>(*this);
        	return res;
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
