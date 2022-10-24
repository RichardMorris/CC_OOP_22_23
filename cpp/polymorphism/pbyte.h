// include guard so we don't include def twice
#ifndef PBYTE_H
#define PBYTE_H

#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::ostream;
using std::istream;
using std::string;

// handy def for our underlying datatype
typedef uint8_t byte;

class pbyte; // need a forward declaration of class as bit_ref needs this

class pbyte {
	// inner class to refer to an individual bit in the byte
	// used by the member access operator [] both as an
	// l-value  b[4] = 1   and r-value  x = b[0]
    class bit_ref {
        friend class pbtye;
        pbyte& p;
        int bit_index; //
    public:
        // constructor
        bit_ref(pbyte &p_in, int ii) :  p{p_in}, bit_index{ii} {};
        // when used as an l-value: bit_ref = 1
        void operator=(int bit_val); // forward declaration as implementation
        // cast to int, used as an rvalue: cout << bit_ref
        operator int() const;  // uses class pbyte which is not full described yet
    };

private:
	byte b;

public:
	// Default Constructor
	pbyte() : b{0x00} { }

	// Constructor
	pbyte(byte b_in) : b{b_in} { }

    // Copy constructor
    pbyte(const pbyte& b_in) : b{b_in.b} { }

    // Copy constructor
    pbyte(const string& s);

    // destructor
    ~pbyte() {    }

    // Assignment operator
    void operator=(const pbyte &v) {
        b = v.b;
    }
    
    // assignment from native type
    void operator=(const byte &v) {
        b = v;
    }
    
    // assignment +=
    pbyte& operator+=(const pbyte &u) {
        b += u.b;
        return *this;
    }

    // element access

    // read only version
    // return 0,1 depending on i-th bit
	int operator[](int index) const {
        byte bit = b >> index;
	    return bit & 0x01;
	}

    // Returns an l-value reference object
	bit_ref operator[](int index) {
        bit_ref p(*this, index);
        return p;
	}
 	
 	// type cast operators
	operator byte() const { return b; }
	operator int() const { return b; }

    // prefix
	pbyte& operator++() { ++b; return *this; }
	// postfix
	pbyte operator++(int i) { pbyte res { b++ }; return res; }
    // prefix
	pbyte& operator--() { --b; return *this; }
	// postfix
	pbyte operator--(int i) { pbyte res { b-- }; return res; }

    // unary not
	pbyte operator~() { pbyte res {(byte) ~b}; return res; }
	
	// comparision
	inline friend bool operator==(const pbyte &u,const pbyte &v) { return u.b == v.b; }
	inline friend bool operator!=(const pbyte &u,const pbyte &v) { return u.b != v.b; }

    // addition subtraction
	inline friend pbyte operator+(const pbyte &u,const pbyte &v)  { pbyte res { (byte) (u.b + v.b) }; return res; }
	inline friend pbyte operator-(const pbyte &u,const pbyte &v)  { pbyte res { (byte) (u.b - v.b) }; return res; }

    // bitwise and, or, xor
	inline friend pbyte operator&(pbyte u,const pbyte &v) { pbyte res { (byte) (u.b & v.b) }; return res; }
	inline friend pbyte operator|(pbyte u,const pbyte &v) { pbyte res { (byte) (u.b | v.b) }; return res; }
	inline friend pbyte operator^(pbyte u,const pbyte &v) { pbyte res { (byte) (u.b ^ v.b) }; return res; }


    // left and right shift
	inline friend pbyte operator<<(pbyte u,int shift) { pbyte res { (byte) (u.b << shift) }; return res; }
	inline friend pbyte operator>>(pbyte u,int shift) { pbyte res { (byte) (u.b >> shift) }; return res; }

    // io
	friend ostream& operator<<(ostream &os,const pbyte &v);

	friend istream& operator>>(istream &os,pbyte &v);


};

ostream& operator<<(ostream &os,const pbyte &v);

#endif
