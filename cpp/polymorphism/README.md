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

We have now separated code into multiple files. Compilation happens in 3 steps

1. Preprocessing: apply #include and #define directives
2. Compilation: for each `.cpp` file produce a `.o` object file
3. Linkage: link all the `.o` files and other library files together in a single executable
 
As the `pbyte.h` files will later be included in other `.h` files and both will included in `.cpp` files
an include guard `#ifndef PBYTE_H` is added to ensure it is only included once.

A `Makefile` is used to control compilation, with rules to define which set of object files
make which executables, and which object files depend on which headers.

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

Classes are defined in **parser.h**
* **token** the abstract base class, has some virtual methods
* **number_token** concrete class holding a number
* **variable_token** concrete class holding the name of a vaiable
* **operator_token** virtual class for operators represent by a symbol
* **unary_op_token** unary operators with associated unary function
* **binary_op_token** unary operators with associated binary function
* **assignment_op_token** assignment ops need special treatment
* **unary_binary_op_token** combines unary and binary ops together

These illustrate some core language features like calling base class constructors,
use of virtual functions, and multiple inheritance. Virtual functions require
a lookup vtable to find the correct function at runtime.

There are two examples of Method polymorphism here. `token` declares
a virtual `print_tok()` method to print a representation of the token.
Similarly `operator_token` declares a virtual `unique_copy()` method
that returns a unique pointer to a copy of the token. Sub-classes override
this to return copies of the correct type. The type system is clever enough
that a `unique_ptr<binary_op_token>` can be returned from a
function with return type `unique_ptr<operator_token>`.

Later we will use `dynamic_cast<binary_op_token&>(token_ref)` to cast a super-type to
the correct sub-type.

## Step 4 the parser

C++ is not a strictly OO language, here we don't have a separate classes for everything.
Here, rather than using a class, all the code relating to parsing and evaluation
is contained inside a single namespace `parser`.

Parsing consist of two steps

1. Tokenising: turning the input into a set of tokens
2. Evaluating: take a list of tokens and evaluate them
 
Both are performed by the `pbyte parse_evaluate(string line)` function.
 
To tokenize we split the input into a set of words and turn each word into a token using
the `unique_ptr<token> get_token(const string &word)`.
A line of input is broken into words and a vector of tokens is returned using
the `vector<unique_ptr<token>> scan(const string &line)` function.

As the tokens are dynamically created at runtime we can't allocate these on the stack, so
unique pointers to these tokens are created in `get_token` (a source)
added to the vector in `scan` and returned to `parse_evaluate`. The vector
is passed to `evaluate` and some tokens are removed from the list,
freeing up their memory. When `parse_evaluate` returns the vector and all the remaining
tokens are destroyed.

Number and variable tokens are simply created in `get_token` using
```
string word = ...;
unique_ptr<variable_token> res = make_unique<variable_token>(word);
```

For operator tokens we have a predefined set of operators, stored as a vector
of pointers `vector<operator_token*> operators`. Elements of this are created
in `main`:
```
auto ot1 = binary_op_token{"&",binary_op_flag,
    [](pbyte l, pbyte r) { return l & r;}
};
add_operator(&ot1);
```
These are created on the stack, and will be freed when main exits, so there is no memory leak.
As the different types of operator token have different sizes we cannot create a
`vector<operator_token>`. Here pointers are safe, an implementation using smart pointers
might be safer, and a little less verbose.

### Algorithms

To find the correct operator token whose symbol matches a given word we one of the
Standard Template Library algorithms defined in `<algorithm>`.
```
    // use the find_if algorithm with the operator_match predicate
    auto itr = find_if(operators.begin(), operators.end(), operator_match{word});
    if(itr != operators.end()) {
        operator_token* tok = *itr;
        auto res = tok->unique_copy();
        return res;
    }
```
`find_if` iterates through a collection, starting with `operators.begin()`,
continuing until `operators.end()` is reached. For each items a function object
`operator_match` is called which returns true if the match is found. `find_if` returns
a *iterator* object, that will either point to the found item, or `operators.end()`.
From the iterator we can use the dereference operator `*itr` to return the found
item, in this case a pointer to an operator token. We then call the `unique_copy()`
methods to return a unique pointer to a copy of the token.

There are many different algorithms to sort, transform, and search sequences of
a variety of different types. Most of these algorithms require some additional function
to be specified.

Here we use functional object ``
```
// Functional object
class operator_match : public std::unary_function<operator_token*,bool> {
    string word;
public:
    operator_match(const string &s) : word{s} {}
    bool operator() (operator_token* op) {
        return op->match(word);
    }
};
```
An object that behaves like a fuction, specifically a unary-function that
takes an `operator_token*` and returns a `bool`.
it has the function call operator: `operator()` taking a `operator_token*`
argument. We could use it like
```
    // create a function that matches operators with the symbol '|'
    auto matcher = operator_match("|"); // use auto to avoid complex type
    
    // two test object
    auto ot1 = binary_op_token{"&", binary_op_flag, [](pbyte l, pbyte r) { return l & r;}};
    auto ot2 = binary_op_token{"|", binary_op_flag, [](pbyte l, pbyte r) { return l | r;}};
    // apply the matcher to the tokens
    bool res1 = matcher(&ot1);  // normal function syntax
    bool res2 = matcher(&ot2);
    // assert results first fails, second succeeds
    assert(!res1);
    assert(res2);
```

We can pass this "function" to the `find_if` algorithm, it test it for each item in the sequence.

### Lambda functions

The above example shows the use of anoymous or lambda functions supplied as
arguments to the `binary_op_token` constructor. Lambda functions are specified like
```
[](pbyte l, pbyte r) { return l & r;}
```
The square brackets indicate a *capture* where local variable of the surronding function can be passed in.
The we have the two function arguments and the function method body.

We could have used a lambda function in our `find_if` search
```
    auto itr = find_if(operators.begin(), operators.end(),
        [word](operator_token* tok) { return tok->match(word); });
```
passing in the `word` local variable as part of the capture.

Lambda functions are used to set up our set of operators. The `binary_op_token`
constructor has one argument a function type
```
    binary_op_token(const char* s, const pbyte& op_flags,
        const std::function<pbyte(pbyte,pbyte)>& fun)
        : operator_token{op_flags, s}, bfun{fun} {}
```
So we can pass in any function taking two pbyte's and returning a pbyte.

### Other ways to loop through a collection

For normal collections the simplest way to loops through collections is
```
    for(auto tok : operators) {
        cout << *tok << endl;
    }
```
This does not work for collection of unique pointers as it involves copying elements
the following loops through a `vector<unique_ptr<token>>& tokens`
```
    for (auto it = tokens.begin(); it != tokens.end(); ++it) {
            token *ptr = it->get();   // (*it) is the element in vector, a UP.
                                      // We then get the element its pointing to using get()
            cout << *ptr << endl;
    }
```

## Step 5 Evaluation

So we now have a `vector<unique_ptr<token>>& tokens` and wish to evaluate the
sequence, in the `pbyte evaluate(vector<unique_ptr<token>>& tokens)` function.

Assignment operations `x = 01010101 & 11110000` are handles a bit differently to
others. First we test if the sequence starts with a variable and an assigment
```
    if(tokens.size()>2) {
        token& e0 = getToken(tokens, 0);
        token& e1 = getToken(tokens, 1);
        if(e0.test_state(var_tok) && e1.test_state(assignment_op)) {
            ...
        }
```
Using the `test_state` method of the base `token` type, which compares the
flags set during construction, with argument. A little bit of bit twiddling is
used in the implementation of the method
```
    bool test_state(pbyte st) const {
        pbyte pb = state & st;
        return pb == st;
    }
```

If the test passes, then we can `dynamic_cast` the tokens to the approprate type
```
    variable_token& vt = dynamic_cast<variable_token&>(e0);
    assign_op_token& ot = dynamic_cast<assign_op_token&>(e1);
```
Now we want to evalate the remainder of the sequence. Here we can use a bit of
recursions, take the first two elements from the vector and call `evaluate`
again using the vector. A bit of care is needed with the use of unique_ptrs.
```
    auto t0 = std::move(tokens[0]); //
    tokens.erase(tokens.begin());
    auto t1 = std::move(tokens[0]);
    tokens.erase(tokens.begin());
```
First we use *move semantics* to move the items out of the vector, and then
erase the element. Then we can simply call evaluate again
```
    pbyte rval = evaluate(tokens);
```

Variable values are stored in a map
```
    map<string, pbyte> variables ; // lookup table for variables and thier values
```
and these can be set using
```
    variables[vt.get_name()] = lval;
```
To make things a bit more complex we want to allow the `x += 00000001` syntax
so assignment operators are supplied with a binary function when the first
argument is the current variable value and the second is the rhs. The result
of the function is set as the new variable value. So the full code is
```
    pbyte rval = evaluate(tokens);          // rhs value
    pbyte vval = variables[vt.get_name()];  // current variable value
    pbyte lval = ot.eval(vval,rval);        // calculate new value
    variables[vt.get_name()] = lval;        // set it in the mat
    return lval;                            // return to calling program
```
`variables[name]` will work if the name is not set, `variable.get(name)` throws
and exception if name does not exist. To find if a key exists use
```
    auto it = variables.find(vt.get_name());
    if (it != variables.end()) {
        return it->second;
    }
    throw Bad_state("Variable " + vt.get_name() + " not found.");
```

The remainder of the algorithm tests for specific patterns of tokens.
For example if their are three tokens, the first being a variable
the second a binary operator and the third a number we get the
values corresponding the the valiable and number use the operators
`eval` method.
```
    pbyte l, r;
    variable_token& vt = dynamic_cast<variable_token&>(e0);
    l = variables[vt.get_name()];
    variable_token& vt = dynamic_cast<variable_token&>(e2);
    r = variables[vt.get_name()];
    binary_op_token& ot = dynamic_cast<binary_op_token&>(e1);
    return ot.eval(l,r);
```

## Conclusion

The example illustrates a number of C# features

* operator overloading
* Code split across multiple files
* Makefile
* Class hierarchies
* Multiple inheretance
* UML class diagrams
* Virtual function
* Dynamic cast
* Use of `unique_ptr` and `make_unique`
* Vectors of `unique_ptr`
* namespaces
* algorithms and function types
* functional objects
* lambda functions
* bit fiddling
* move semantics
* maps of key-value pairs
* recursion

The corresponding code took about 3 days to write. There was quite a bit of
re-engineering along the way and lots of figuring out error messages.


# Exercises

1) Implement a class hierarchy. For example with Person as the base class,
Student, Teacher, as sub classes.
Students will have a list of the lessons they attend (just an array of strings
for simplicity), and Teachers will have a list of
lessons taught. Then we can have a Student_Teacher who is both a student
and a teacher so inherits from both. Each class should have a `print_details` method
that prints all the information about the person.

1a) Test. Use the [Catch2](https://github.com/catchorg/Catch2) unit testing framework
to test creation of classes.

2) Define a simple csv file format with details of school members, read in file
and create people using the previous classes.

2a) Test

3) Create a vector of unique pointers in one function and return it.
Create another function to print out the list, another to only print teachers
and a third to un-enroll a student.

3a) Test

