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
	// Indicates what type of operator we use, not actually used
    enum token_type { NUM, VAR, OP };
    
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
    
    // virtual (abstract) base type of all tokens
    class token {
    protected:
        // byte containing flag info
        pbyte state;
        // protected constructor, visable to subclasses
        token(pbyte st_in) : state{st_in} {}
        // pure virtual function, each sub-class must provide an implementation
        virtual void print_tok(ostream&) const =0;
    public:
        // virtual distructor must be declared for reasons
        virtual ~token() {}
        // common output operator, uses virtual function
        friend ostream& operator<<(ostream &os,const token &tok) {
            tok.print_tok(os); return os;
        }
        // check if the state matches
        bool test_state(pbyte st) const {
        	pbyte pb = state & st;
        	return pb == st;
        }
    };

    // derived type for numbers
    class number_token : public token {
        pbyte value;
        void print_tok(ostream& os) const {
            os << "NT " << value;
        }
    public:
        number_token(pbyte p) : token{number_tok}, value{p} {}
        pbyte get_value() const { return value; }
        ~number_token() {
//            cout << "descruct "; print_tok(cout); cout << endl;
        }
    };

    class variable_token : public token {
        string variable_name;
        void print_tok(ostream &os) const { os << "VT " << variable_name; }
    public:
        variable_token(const string& name) : token{var_tok}, variable_name{name} { }
        string get_name() const { return variable_name; }
        ~variable_token() {
//            cout << "descruct "; print_tok(cout); cout << endl;
        }
    };

    class operator_token : public token {
    protected:
        string sym;
        operator_token(const pbyte& flags, const string& s)
            : token{flags}, sym{s} {}
        void print_tok(ostream &os) const { os << "OT " << sym; }
     public:
        bool match(const string& word) const {
            return sym == word;
        }
        ~operator_token() {
//            cout << "destruct "; print_tok(cout); cout << endl;
        }

        // Used to return a unique_ptr to a copy of the token
        // sub classes must override
        virtual std::unique_ptr<operator_token> unique_copy() const =0;

    };

    class unary_op_token : public virtual operator_token {
    protected:
        std::function<pbyte(pbyte)> ufun;
        void print_tok(ostream &os) const { os << "BOT " << sym; }
    public:
        unary_op_token(const char* s, const pbyte& op_flags,
            const std::function<pbyte(pbyte)>& fun)
        	: operator_token{op_flags, s}, ufun{fun} {}

        pbyte eval(pbyte l) const { return ufun(l); }

        std::unique_ptr<operator_token> unique_copy() const {
        	auto res = std::make_unique<unary_op_token>(*this);
        	return res;
        }
    };

    class binary_op_token : public virtual operator_token {
    protected:
        std::function<pbyte(pbyte,pbyte)> bfun;
        void print_tok(ostream &os) const { os << "BOT " << sym; }
    public:
        binary_op_token(const char* s, const pbyte& op_flags,
            const std::function<pbyte(pbyte,pbyte)>& fun)
        	: operator_token{op_flags, s}, bfun{fun} {}

        pbyte eval(pbyte l, pbyte r) const { return bfun(l,r); }

        std::unique_ptr<operator_token> unique_copy() const {
        	auto res = std::make_unique<binary_op_token>(*this);
        	return res;
        }

    };

    class assign_op_token : public operator_token {
        std::function<pbyte(pbyte,pbyte)> afun;
        void print_tok(ostream &os) const { os << "AOT " << sym; }
    public:
        assign_op_token(const char* s, const pbyte& op_flags,
            const std::function<pbyte(pbyte,pbyte)>& fun)
        	: operator_token{op_flags, s}, afun{fun} {}

        byte eval(pbyte varval, pbyte r) const { return afun(varval, r); }

        std::unique_ptr<operator_token> unique_copy() const {
        	auto res = std::make_unique<assign_op_token>(*this);
        	return res;
        }

    };

    // some operators like - can be used in both a binary and unary
    // context:  x - y, - x.
    class unary_binary_op_token : public binary_op_token, public unary_op_token {
        // must override virtual function defined in both subclasses
        void print_tok(ostream &os) const { os << "UBOT " << sym; }
    public:
        unary_binary_op_token(const char* s, const pbyte& op_flags,
            const std::function<pbyte(pbyte)>& ufun_in,
            const std::function<pbyte(pbyte,pbyte)>& bfun_in)
        	: operator_token{op_flags, s}, // directly calls common baseclass
        	  binary_op_token{s, op_flags, bfun_in}, //
        	  unary_op_token{s, op_flags, ufun_in} {}

        std::unique_ptr<operator_token> unique_copy() const {
        	auto res = std::make_unique<unary_binary_op_token>(*this);
        	return res;
        }

    };
    
    
    // Methods available in this namespace

    // parse an expression, then evaluate
    pbyte parse_evaluate(string line);


    // scan the input creating a list of tokens
    vector<unique_ptr<token>> scan(const string &line);

    // evaluate a list of tokens
    pbyte evaluate(vector<unique_ptr<token>> &tokens);
    
    // get the token for a word
    unique_ptr<token> get_token(const string &word);
    
    // add an operator to the list of known operators
    void add_operator(operator_token* tok);
    
    // get a reference to a token in the list
    token& getToken(vector<unique_ptr<token>>& tokens, int i);
    
    
    
    // Represent an error
    class Bad_state {
    	string message;
    public:
    	Bad_state(const string& str) : message{str} {}
    	string get_message() const { return message; }
    };

    // Represents a unmatched symbol while scanning
    class unmatched_token {
        string word;
    public:
        unmatched_token(string s) : word{s} {}
    	string get_message() const { return word; }
    };

    void test_op_match();
    void print_operators();
}
