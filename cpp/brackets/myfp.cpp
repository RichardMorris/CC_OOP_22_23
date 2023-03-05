#include <string>
using std::string;


class myfp {
private:
    int mantissa;
    int exponent;

    // 299,792,458 = 2.99792458 * 10^8
    // matissa = 2.99792458
    // exponent = 8
    // mantissa >= 1   < 10    
    // scientific notation
    // pi = 3.14 * 10^0
    // small = 1.0000 * 10^-3  -> 0.001
    // 
    // floating point
    // signbit  (1+mantissa) * 2^exp
    // 0 <= matissa < 1
    // 1 <= 1+mantissa < 2 



public:
    myfp(int m,int e) : mantissa(m), exponent(e) {}
    myfp(string str)  { 
        // be able to parse a string like 3.14 or 123e7
        int pos_of_decimal_point  = str.find_first_of('.');
        string intpart = str.substr(0,pos_of_decimal_point); // begining of string
        string fractpart = str.substr(pos_of_decimal_point); // end of string

        for(int i=0; i<10 ; ++i ) {}

        int pos = 0;
         for(auto itt = str.begin(); itt != str.end(); ++itt, ++pos) {
            char c = *itt;
            if(c == '.') break;
        }
     };
    myfp operator+ (myfp summand);

};


// in C++

class myfpConvertrr {
    myfpConvertrr(myfp fp);


    // only in C++
    double toDouble() {
        return (1+fp.mantissa) * math.power(2,fp.exponent);
    }


};