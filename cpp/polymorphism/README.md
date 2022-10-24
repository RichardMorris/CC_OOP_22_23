# Operator overloading

Covered in week 4 lectures

## Files

* **vec3d.h** version of 3D vector with operator overloads
* **main.cpp** testing program

# A calculator for exploring binary operations

One problem encountered during the lecture was there was no easy way
to display binary values in binary. We can solve this by making our
own type to represent a single byte, with I/O as binary digits.

Step 1 is to make a type.

* **pbyte.h** definition
* **pbyte.cpp** implementation
* **pbytemain.cpp* testing program

* x = 10101010
* y = 00001111
* x | y
* x & y
* x ^ y
* x + y
* x - y
* ^ x
* x | 00001100
* - x
* ++ x
* x ++
*