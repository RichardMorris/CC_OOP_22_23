#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <sstream>
#include <memory>
#include <cctype>

using std::unique_ptr;
using std::make_unique;

#include "pbyte.h"
#include "parser.h"

using std::cout;
using std::endl;
using std::ostream;
using std::istream;
using std::string;
using std::vector;

namespace parser {
    vector<operator_token*> operators;
    map<string, pbyte> variables ; // lookup table for variables

    void add_operator(operator_token* tok) { operators.push_back(tok); }

// https://stackoverflow.com/questions/236129/how-do-i-iterate-over-the-words-of-a-string
vector<unique_ptr<token>> scan(const string &line) {
    vector<unique_ptr<token>> res;
    std::istringstream iss{line};
    string word;
    while(iss >> word) {
        unique_ptr<token> t = get_token(word);
        res.push_back(std::move(t)); // Ownership now rests with vector
    }
    return res;
}

// Functional object
class operator_match : public std::unary_function<operator_token*,bool> {
    string word;
public:
    operator_match(const string &s) : word{s} {}
    bool operator() (operator_token* op) {
        return op->match(word);
    }
};

// Turn the next word into a token
unique_ptr<token> get_token(const string &word) {

    if(isalpha(word[0])) {
        unique_ptr<variable_token> res = make_unique<variable_token>(word);
        return res;
    }
    if(isdigit(word[0])) {
        unique_ptr<number_token> res = make_unique<number_token>(word);
        return res;
    }
    // use the find_if algorithm with the operator_match predicate
    auto itr = find_if(operators.begin(), operators.end(), operator_match{word});
    if(itr != operators.end()) {
        operator_token* tok = (*itr);
        auto res = tok->unique_copy();
        return res;
    }

    throw unmatched_token{word};
}

token& getToken(vector<unique_ptr<token>> &tokens, int i) {
    auto e0 = tokens[i].get();
    return *e0;
}

void print(vector<unique_ptr<token>> &tokens) {
    // it would be nice to use
    // for(auto it : tokens) {}
    // but that requires copying of a UP
    for (auto it = tokens.begin(); it != tokens.end(); ++it) {
            token *ptr = it->get();   // (*it) is the element in vector, a UP. We then get the element its pointing to
            cout << *ptr << endl;
    }
    cout << endl;
}

pbyte evaluate(vector<unique_ptr<token>> &tokens) {
        // test if assignments
        if(tokens.size()>2) {
            token& e0 = getToken(tokens, 0);
            token& e1 = getToken(tokens, 1);
            
            if(e0.test_state(var_tok) && e1.test_state(assignment_op)) {
                variable_token& vt = (variable_token&) e0;
                assign_op_token& ot = (assign_op_token&) e1;

                unique_ptr<token> t0 = std::move(tokens[0]); //
                tokens.erase(tokens.begin());
                auto t1 = std::move(tokens[0]);
                tokens.erase(tokens.begin());

                pbyte rval = evaluate(tokens);
                pbyte vval = variables[vt.get_name()];
                pbyte lval = ot.eval(vval,rval);
                variables[vt.get_name()] = lval;
                return lval;
            }
        }
        if(tokens.size()==1) {
            token& e0 = getToken(tokens, 0);
            if(e0.test_state(var_tok)) {
                variable_token& vt = dynamic_cast<variable_token&>(e0);

                auto it = variables.find(vt.get_name());
                 if (it != variables.end()) {
                	 return it->second;
                 }
                 throw Bad_state("Variable " + vt.get_name() + " not found.");
            }
            else if(e0.test_state(number_tok)) {
                number_token& nt = dynamic_cast<number_token&>(e0);
                return nt.get_value();
            }
            throw Bad_state("Operator with no arguments");
        }
        if(tokens.size()==2) {
            token& e0 = getToken(tokens, 0);
            token& e1 = getToken(tokens, 1);
            pbyte l, r;
            unary_op_token* uot = nullptr;
            bool prefix = false;
            bool postfix = false;

            if(e0.test_state(var_tok)) {
                variable_token& vt = dynamic_cast<variable_token&>(e0);
                l = variables[vt.get_name()];
            }
            else if(e0.test_state(number_tok)) {
                number_token& nt = dynamic_cast<number_token&>(e0);
                l = nt.get_value();
            }
            else if(e0.test_state(unary_prefix)) {
            	uot = & (dynamic_cast<unary_op_token&>(e0));
            	prefix = true;
            }
            else
                throw Bad_state("First argument should be variable, number or prefix op");

            if(e1.test_state(var_tok)) {
                variable_token& vt = dynamic_cast<variable_token&>(e1);
                r = variables[vt.get_name()];
            }
            else if(e1.test_state(number_tok)) {
                number_token& nt = dynamic_cast<number_token&>(e1);
                r = nt.get_value();
            }
            else if(e1.test_state(unary_postfix)) {
            	uot = &(dynamic_cast<unary_op_token&>(e1));
            	postfix = true;
            }
            else
                throw Bad_state("Second argument should be variable, number or postfix op");

            if(prefix && !postfix) {
            	return uot->eval(r);
            } else if(!prefix && postfix) {
            	return uot->eval(l);
            }
            throw Bad_state("Either first argument is a prefix op or second argument is a postfix op, but not both");
        }

        if(tokens.size()==3) {
            token& e0 = getToken(tokens, 0);
            token& e1 = getToken(tokens, 1);
            token& e2 = getToken(tokens, 2);
            pbyte l, r;
            
            if(e0.test_state(var_tok)) {
                variable_token& vt = dynamic_cast<variable_token&>(e0);
                l = variables[vt.get_name()];
            }
            else if(e0.test_state(number_tok)) {
                number_token& nt = dynamic_cast<number_token&>(e0);
                l = nt.get_value();
            }
            else
                throw Bad_state("First argument should be variable or number");

            if(e2.test_state(var_tok)) {
                variable_token& vt = dynamic_cast<variable_token&>(e2);
                r = variables[vt.get_name()];
            }
            else if(e2.test_state(number_tok)) {
                number_token& nt = dynamic_cast<number_token&>(e2);
                r = nt.get_value();
            }
            else
                throw Bad_state("Third argument should be variable or number");

            if(e1.test_state(binary_op_flag) ) {
            	binary_op_token& ot = dynamic_cast<binary_op_token&>(e1);
                return ot.eval(l,r);
            }
            else
                throw Bad_state("Second argument should be an operator");
        }
        throw Bad_state("Bad number of tokens");
}


}
