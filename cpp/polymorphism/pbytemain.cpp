#include <iostream>
using std::cout;
using std::cin;
using std::endl;

#include "pbyte.h"

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

    cout << "Enter an 8 digit binary value\n";
    cin >> b5;
    cout << "Entered " << b5 << endl;
}

int main(int argc, char* argv[]) {
	cout.setf(std::ios_base::fixed);
	cout.precision(3);

	test_ops();

}
