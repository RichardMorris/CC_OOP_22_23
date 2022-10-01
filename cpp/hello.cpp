#include <iostream>
using std::cout;
using std::endl;

void recurse(int num) {
    int dub = 2 * num;
    int trip = 3 * num;
    int *ptr = &num;
    cout << " num " << num << " address " << ptr << " dub " << &dub << " trip " << &trip << endl;
    if(num>0)
        recurse(num-1);
}

void hack(int* p) {
    *p = 7;
    *(p-2) = 8;
}

int main() {
    int num = 5;
    int dub = 2 * num;
    int trip = 3 * num;
    int *ptr = &num;
    cout << " num " << num << " address " << ptr << " dub " << &dub << " trip " << &trip << endl;
    recurse(num);
    
    cout << " num " << num << " dub " << dub << " trip " << trip << endl;
    hack(ptr);
    cout << " num " << num << " dub " << dub << " trip " << trip << endl;
    return 0;
}