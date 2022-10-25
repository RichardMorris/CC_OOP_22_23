# Operator overloading

Covered in week 4 lectures

## Files

* **vec3d.h** version of 3D vector with operator overloads
* **main.cpp** testing program

# A calculator for exploring binary operations

One problem encountered during the lecture was there was no easy way
to display binary values in binary. We can solve this by making our
own type to represent a single byte, with I/O as binary digits.

## Step 1 is to make a type to represent a binary byte

* **pbyte.h** definition, overloads many operations, so it can be used like a native type
* **pbyte.cpp** implementation, for longer methods
* **pbytemain.cpp* testing program

Most of **pbyte.h** is relatively straigthforward.
One narly aspect is the need to use an inner class to allow [] to index bits
so we can set a bit with b[3] = 1

## Step 2 define the syntax

A simple syntax with binary numbers, 01010101, variables and operators
separated by spaces.

* 11110000
* x
* x | y
* x | 00001100
* 00001111 | x
* 00001111 | 00001100
* x & y
* x ^ y
* x + y
* x - y  // binary minus
* - X    // unary minus
* ~ x
* x = 10101010 // assignment operator
* y = 00001111
* x = x & y
* x -= 00000001
* ++ x  // TODO
* x ++  // TODO

## Step 3 token classes

Each type of token is represented by a class

[Class diagram](classdiagram.drawio.png)