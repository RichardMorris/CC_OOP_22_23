#include <iostream>
#include <memory>
#include <math.h>

using std::cout;
using std::endl;
using std::unique_ptr;
using std::make_unique;
using std::shared_ptr;

class Vec3D {
public:
    double x;
    double y;
    double z;
    Vec3D(double xi, double yi, double zi) : x{xi}, y{yi}, z{zi} {
        cout << "Vec3D constructor (" << x << " " << y << " " << z << ") addr " << &x << endl;
    }

    ~Vec3D() {
        cout << "Vec3D distructor ("  << x << " " << y << " " << z << ") addr " << &x << endl;
    }

    Vec3D(const Vec3D& v) {
        x = v.x; y = v.y; z = v.z;
        cout << "Vec3D copy ("  << x << " " << y << " " << z << ") addr " << &x << endl;
    }
    
    unique_ptr<Vec3D> negate(); // defined later
};

void stack_and_new() {
    Vec3D v1(1,2,3);
    cout << "v1 " << &v1 << endl;
    Vec3D *v2 = new Vec3D(4,5,6);
    cout << "v2 " << v2 << endl;
    Vec3D *v3 = new Vec3D(7,8,9);
    cout << "v3 " << v3 << endl;

    Vec3D v4 = v1;
    cout << "v4 " << &v4 << endl;
    Vec3D *v5 = &v1;
    cout << "v5 " << &v5 << endl;

    delete v2;
    cout << "end of stack_and_new()\n";
}

double lengthPTR(const Vec3D *v) {
    cout << "lengthPTR\t" << v << "\t" << &(v->x) << endl;
    double lsq = v->x * v->x + v->y * v-> y + v->z * v->z;
    // v->y = 0;
    return sqrt(lsq);
}

double lengthREF(const Vec3D &v) {
    cout << "lengthREF\t" << &v << "\t" << &(v.x) << endl;
    double lsq = v.x * v.x + v.y * v.y + v.z * v.z;
    // v.y = 0;
    return sqrt(lsq);
}

double lengthCP(const Vec3D v) {
    cout << "lengthCP\t" << &v << "\t" << &(v.x) << endl;
    double lsq = v.x * v.x + v.y * v.y + v.z * v.z;
    // v.x = 0;
    return sqrt(lsq);
}

void pointer_and_refs() {
    Vec3D v1(1,2,3);
    Vec3D *v2 = new Vec3D(4,5,6);

    const double l1 = lengthPTR(&v1);
    cout << "using ptr " << l1 << endl;
    const double l2 = lengthCP(v1);
    cout << "using copy " << l2 << endl;
    const double l3 = lengthREF(v1);
    cout << "using ref " << l3 << endl;

    const double l4 = lengthPTR(v2);
    cout << "using ptr " << l4 << endl;
    const double l5 = lengthCP(*v2);
    cout << "using copy " << l5 << endl;
    const double l6 = lengthREF(*v2);
    cout << "using ref " << l6 << endl;
}


double lengthUP(const unique_ptr<Vec3D> &v) {
    cout << "lengthUP\t" << &v << "\t" << &(v->x) << endl;
    double lsq = v->x * v->x + v->y * v->y + v->z * v->z;
    v->y = 0;
    return sqrt(lsq);
}

double lengthMOV(const unique_ptr<Vec3D> v) {
    cout << "lengthMOV\t" << &v << "\t" << &(v->x) << endl;
    double lsq = v->x * v->x + v->y * v->y + v->z * v->z;
    v->y = 0;
    return sqrt(lsq);
}

double lengthSP(const shared_ptr<Vec3D> v) {
    cout << "lengthSP\t" << &v << "\t" << &(v->x) << endl;
    double lsq = v->x * v->x + v->y * v->y + v->z * v->z;
    v->y = 0;
    return sqrt(lsq);
}

void smart_pointers() {
    unique_ptr<Vec3D> v4 { new Vec3D(10,11,12) };
    cout << "v4 " << &v4 << " " << v4.get() << endl;

    unique_ptr<Vec3D> v5 { new Vec3D(13,14,15) };
    cout << "v5 " << &v5 << endl;
    
    shared_ptr<Vec3D> v6 { new Vec3D(16,17,18) };
    cout << "v6 " << &v6 << endl;

    
    cout << "\nusing unique ptr\n";
    lengthUP(v4);
    cout << "v4 " << &v4 << (v4 == nullptr ? " null" : " not null") << endl;
    
    cout << "\nusing raw ptr\n";
    lengthPTR(v4.get());
    cout << "v4 " << &v4 << (v4 == nullptr ? " null" : " not null") << endl;
    
    cout << "\nusing move\n";
    lengthMOV(move(v5));
    cout << "v5 " << &v5 << (v5 == nullptr ? " null" : " not null") << endl;
    // v4->y = 16; // core dump
    
    cout << "\nusing shared ptr\n";
    lengthSP(v6);
    
    cout << "end of smart_pointers()\n";
}

unique_ptr<Vec3D> tripple(unique_ptr<Vec3D> v) {
    v->x *= 3; v->y *= 3; v->z *= 3;
    return v;
}

unique_ptr<Vec3D> Vec3D::negate() {
    return make_unique<Vec3D>(-x,-y,-z);
}


void make_unique() {
    Vec3D v1(1,2,3);

    unique_ptr<Vec3D> v2 = v1.negate();

    unique_ptr<Vec3D> v3 = tripple(move(v2));

    cout << "end of make_unique()\n";
}

int main() {
    stack_and_new();
    cout << "stack_and_new() finished\n\n";
    pointer_and_refs();
    cout << "end of pointer_and_refs finished\n\n";
    
    smart_pointers();
    cout << "smart_pointers() finished\n\n";
   
    make_unique();
    cout << "make_unique() finished\n\n";
   
    cout << "end of main\n";
}