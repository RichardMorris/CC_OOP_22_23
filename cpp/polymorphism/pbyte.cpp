#include <iostream>
#include <string>

#include "pbyte.h"

using std::cout;
using std::endl;
using std::ostream;
using std::istream;
using std::string;

typedef uint8_t byte;

// Constructor from a string
pbyte::pbyte(const string &s) {
    b = 0;
    for(int i=0;i<8;++i) {
        if(s[i]=='1') {
            b |= 0x01;
        }
        if(i<7)
            b <<= 1;
    }
}

// output to stream
ostream& operator<<(ostream &os, const pbyte &v) {
    os << v[7] << v[6] << v[5] << v[4] << v[3] << v[2] << v[1] << v[0];
    return os;
}

// input from stream
// inputs a string and then using assignment operator=
istream& operator>>(istream &is, pbyte &v) {
    string s;
    is >> s;
    v = s;
    return is;
}

// Assignment operator for a bit_ref
// just sets/clears an individual bit
void pbyte::bit_ref::operator=(int b) {
        byte bit = 0x01 << bit_index;
        if(b==0) {
            p.b &= ~bit;
        } else {
            p.b |= bit;
        }
    }

// cast to int for individual bit
// returns 0, 1 depending on whether bit is set
pbyte::bit_ref::operator int() const {
    byte bit = p.b >> bit_index;
    return bit & 0x01;
}
