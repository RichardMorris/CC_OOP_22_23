// C++ program to demonstrate
// multithreading using three
// different callables.
// from https://www.geeksforgeeks.org/multithreading-in-cpp/ 

#include <iostream>
#include <thread>
#include <chrono>

using namespace std;
using namespace std::chrono_literals;

// A dummy function
void foo(int Z)
{
for (int i = 0; i < 10; i++)
{

	cout << "Thread " << Z << " using function"
			" pointer as callable\n";
	std::this_thread::sleep_for(1s);
}
}

// A callable object
class thread_obj {
public:
	void operator()(int x)
	{
	for (int i = 0; i < x; i++)
		cout << "Thread using function"
				" object with parameter " << x << " as callable\n";
	}
};

// Driver code
int main()
{
cout << "Threads 1 and 2 and 3 "
		"operating independently" << endl;

// This thread is launched by using
// function pointer as callable

thread th11(foo, 1);
thread th12(foo, 2);
thread th13(foo, 3);
thread th14(foo, 4);
thread th15(foo, 5);

cout << "\nWaiting for threads to finish\n\n";

// Wait for the threads to finish
// Wait for thread t1 to finish
th11.join();
th12.join();
th13.join();
th14.join();
th15.join();

cout << "\nAll threads finished\n\n";

// This thread is launched by using
// function object as callable
thread th_fun_obj(thread_obj(), 3);

// Define a Lambda Expression
auto f = [](int x)
{
	for (int i = 0; i < x; i++)
	cout << "Thread using lambda"
			" expression as callable\n";
};

// This thread is launched by using
// lambda expression as callable
thread th_lambda(f, 3);

// Wait for thread t2 to finish
th_fun_obj.join();

// Wait for thread t3 to finish
th_lambda.join();

return 0;
}
