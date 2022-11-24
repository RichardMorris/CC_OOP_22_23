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
	
	test_op_match();

    auto ot1 = binary_op_token{"&",binary_op_flag,
        [](pbyte l, pbyte r) { return l & r;}
    };
    add_operator(&ot1);
    auto ot2 = binary_op_token{"|",binary_op_flag,
        [](pbyte l, pbyte r) { return l | r; }
    };
    add_operator(&ot2);
    auto ot3 = binary_op_token{"^",binary_op_flag,
        [](pbyte l, pbyte r) { return l ^ r; }
    };
    add_operator(&ot3);
    auto ot4 = binary_op_token{"<<",binary_op_flag,
        [](pbyte l, pbyte r) { return l << r; }
    };
    add_operator(&ot4);
    auto ot5 = binary_op_token{">>",binary_op_flag,
        [](pbyte l, pbyte r) { return l >> r; }
    };
    add_operator(&ot5);
    auto ot6 = assign_op_token{"=",binary_op_flag | assignment_op,
    	[](pbyte l, pbyte r) { return r;}};
    add_operator(&ot6);

    auto ot7 = assign_op_token{"+=",binary_op_flag | assignment_op,
    	[](pbyte l, pbyte r) { return l + r;}};
    add_operator(&ot7);

    auto ot8 = assign_op_token{"-=",binary_op_flag | assignment_op,
    	[](pbyte l, pbyte r) { return l - r;}};
    add_operator(&ot8);

    auto ot9 = unary_op_token{"~", unary_prefix,
    	[](pbyte l) { return ~l;}};
    add_operator(&ot9);

    auto ot10 = unary_op_token{"!", unary_postfix,
    	[](pbyte l) { return ~l;}};
    add_operator(&ot10);

    auto ot11 = unary_binary_op_token{"-", unary_prefix | binary_op_flag,
    	[](pbyte r) { return - r;},
        [](pbyte l, pbyte r) { return l - r; }
    };
    add_operator(&ot11);

    auto ot12 = binary_op_token{"+", binary_op_flag,
        [](pbyte l, pbyte r) { return l + r; }
    };
    add_operator(&ot12);

    print_operators();

    string line;
    while(cout << "Input:\t", std::getline(cin,line) ) {
        //cout << "line " << s2 << endl;
    	if(line == "quit")
    		break;
        
        try {
            pbyte res = parse_evaluate(line);
            cout << "Result:\t" << res << endl;
            
        } catch(unmatched_token &ut) {
            cout << "Unmatched token:\t" << ut.get_message() << endl;
        } catch (Bad_state& bs) {
            cout << "Bad state:\t " << bs.get_message() << endl;
        }

    }
}
