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
    vector<operator_token> operators;
    map<string, pbyte> variables ; // lookup table for variables

    void add_operator(operator_token tok) { operators.push_back(tok); }


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
class operator_match : public std::unary_function<operator_token,bool> {
    string word;
public:
    operator_match(const string &s) : word{s} {}
    bool operator() (operator_token& op) {
        return op.match(word);
    }
};

class unmatched_token {
public:
    string word;
    unmatched_token(string s) : word{s} {}
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
        operator_token& tok = (*itr);
        unique_ptr<operator_token> res = make_unique<operator_token>(tok);
        return res;
    }

    throw unmatched_token{word};
}

void parse(vector<unique_ptr<token>> &tokens) {
        state st = none;

        for (auto it = tokens.begin(); it != tokens.end(); ++it) {
            auto& m = *it;
            token *tok = m.get();
            state next_st = tok->get_state();
            cout << "state " << st << tok->get_state_str() << endl;
            
            switch(st) {
            case none:
                st = next_st;
                break;
            }
            
            
        }
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
            token *ptr = (*it).get();   // (*it) is the element in vector, a UP. We then get the element its pointing to
            cout << *ptr << endl;
    }
    cout << endl;
}

pbyte evaluate(vector<unique_ptr<token>> &tokens) {
        state st = none;
        print(tokens);
        
        // test if assignments
        if(tokens.size()>2) {
            token& e0 = getToken(tokens, 0);
            token& e1 = getToken(tokens, 1);
            
            if(e0.get_state() == var && e1.get_state() == op ) {
                variable_token& vt = (variable_token&) e0;
                operator_token& ot = (operator_token&) e1;
                if(ot.match("=")) {
                    unique_ptr<token> t0 = std::move(tokens[0]); //
                    tokens.erase(tokens.begin());
                    auto t1 = std::move(tokens[0]);
                    tokens.erase(tokens.begin());
                    pbyte val = evaluate(tokens);

                    variables[vt.get_name()] = val;
                    return val;
                }
            }
        }
        if(tokens.size()==1) {
            token& e0 = getToken(tokens, 0);
            if(e0.get_state() == var) {
                variable_token& vt = dynamic_cast<variable_token&>(e0);
                return variables[vt.get_name()];
            }
            else if(e0.get_state() == num) {
                number_token& nt = dynamic_cast<number_token&>(e0);
                return nt.get_value();
            }
            throw Bad_state();
        }

        if(tokens.size()==3) {
            token& e0 = getToken(tokens, 0);
            token& e1 = getToken(tokens, 1);
            token& e2 = getToken(tokens, 2);
            pbyte l, r;
            
            if(e0.get_state() == var) {
                variable_token& vt = dynamic_cast<variable_token&>(e0);
                l = variables[vt.get_name()];
            }
            else if(e0.get_state() == num) {
                number_token& nt = dynamic_cast<number_token&>(e0);
                l = nt.get_value();
            }
            else
                throw Bad_state();

            if(e2.get_state() == var) {
                variable_token& vt = dynamic_cast<variable_token&>(e2);
                r = variables[vt.get_name()];
            }
            else if(e2.get_state() == num) {
                number_token& nt = dynamic_cast<number_token&>(e2);
                r = nt.get_value();
            }
            else
                throw Bad_state();

            if(e1.get_state() == op ) {
                operator_token& ot = dynamic_cast<operator_token&>(e1);
                return ot.eval(l,r);
            }
            else
                throw Bad_state();
        }

        pbyte pb { 0xff };
        return pb;
}

const string state_str[] { "none", "var", "op", "num", "op_var", "op_num", "var_op", "num_op",
    "num_op_num", "num_op_var", "var_op_num", "var_op_var" };
    
string token::get_state_str() const {
    return state_str[st];
}


}