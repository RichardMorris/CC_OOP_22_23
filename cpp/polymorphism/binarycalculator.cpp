#include <iostream>
#include <cmath>
#include <memory>
#include <vector>
#include <string>
#include <sstream>
#include <iterator>

#include <stdlib.h>
#include <math.h>
using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::unique_ptr;

#include "pbyte.h"
#include "parser.h"

using namespace parser;

void test_ops() {
    
    pbyte b1 {0x55};
    
    int bit0 = b1[0];
    int bit2 = b1[2];
    
    
    pbyte b2 {0xf0};
    pbyte b3 = b1 | b2;
    cout << b1 << " | " << b2 << " -> " << b3 << endl;
    b3 = b1 & b2;
    cout << b1 << " & " << b2 << " -> " << b3 << endl;
    b3 = b1 ^ b2;
    cout << b1 << " ^ " << b2 << " -> " << b3 << endl;
    
    for(int i=0;i<8;++i) {
        pbyte p1 = (b1 << i );
        cout << b1 << " << " << i << " -> " << p1 << endl;
    }

    for(int i=0;i<8;++i) {
        pbyte p2 = (b2 >> i );
        cout << b2 << " >> " << i << " -> " << p2 << endl;
    }
    
    
    b3 = b1 + b2;
    cout << b1 << " + " << b2 << " -> " << b3 << endl;
    b3 = b2 - b1;
    cout << b2 << " - " << b1 << " -> " << b3 << endl;
    
    cout << " ++" << b1;
    b2 = ++b1;
    cout << " -> " << b2 << " b1 " << b1 << endl;
    
    cout <<  b1 << "-- ";
    b3 = b1--;
    cout << " -> " << b3 << " b1 " << b1 << endl;

    byte b4 = b1;
    
    cout << ((int) b1) << " = " << b1 << endl;
    
    b3 = 127;
    
    cout << ((int) b3) << " = " << b3 << endl;
    
    string s { "10100101" };
    pbyte b5 {s};
    
    cout << s << " -> " << ((int) b5) << " = " << b5 << endl;
    
    b3 = ~b1;
    cout << "~" << b1 << " -> " << b3 << endl;
    

    cout << "b1 " << b1 << endl;
    
    cout << "set b1[1] = 1";
    b1[1] = 1;
    cout << b1 << endl;
    cout << "set b1[2] = 0";
    b1[2] = 0;
    cout << b1 << endl;

    cin >> b5;
    cout << "Entered " << b5 << endl;
}

int main(int argc, char* argv[]) {
	cout.setf(std::ios_base::fixed);
	cout.precision(3);
	
//	test_ops();
	
    
    add_operator(operator_token{"=",binary_op_flag, [](pbyte l,pbyte r) { return l;}});
    add_operator(operator_token{"&",binary_op_flag,
        [](pbyte l,pbyte r) { return l & r;}
    });
    add_operator(operator_token{"|",binary_op_flag,
        [](pbyte l,pbyte r) { return l | r; }
    });
    add_operator(operator_token{"^",binary_op_flag,
        [](pbyte l,pbyte r) { return l ^ r; }
    });
    // add_operator(operator_token{"~",unary_prefix,
    //     (pbyte l) { return ~l; }
    // });

    string s2;
    while(std::getline(cin,s2) ) {
        cout << "line " << s2 << endl;
        vector<unique_ptr<token>> tokens = scan(s2);

        try {
            pbyte res = evaluate(tokens);
            cout << "Result: " << res << endl;
        } catch (Bad_state* bs) { cout << "Bad state" << endl; }
    }
}
