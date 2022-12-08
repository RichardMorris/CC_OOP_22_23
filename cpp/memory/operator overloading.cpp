#include <iostream>
#include <memory>
#include <math.h>
using std::cout;
using std::endl;
using std::unique_ptr;
using std::make_unique;
using std::shared_ptr;
class Vec3D {
    double x;
    double y;
    double z;
public:
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
    void operator=(const Vec3D &v) {
        x = v.x; y = v.y; z = v.z;
        cout << "Vec3D = ("  << x << " " << y << " " << z << ") addr " << &x << endl;
    }
    class bad_index {};
    double operator[](int index) {
        switch(index) {
            case 0: return x;
            case 1: return y;
            case 2: return z;
            default: throw new bad_index();
        }
    }
    unique_ptr<Vec3D> negate(); // defined later
    friend Vec3D operator+ (const Vec3D &u, const Vec3D &v);
    friend std::ostream& operator <<(std::ostream& stream, const Vec3D& matrix);
};
Vec3D operator+ (const Vec3D &u, const Vec3D &v) {
    cout << "Vec3D + ("  << endl;
    Vec3D w { u.x + v.x,  u.y + v.y, u.z + v.z};
    return w;
}
//  cout << u << v << endl;
//  (cout << u) << v << endl;
//      cout << v << endl
//      (cout << v) << endl
//          cout << endl
std::ostream& operator <<(std::ostream& stream, const Vec3D& v) {
    stream << "(" << v.x << "," << v.y << "," << v.z << ")"; 
    return stream;
}
int main() {
    Vec3D u {1,2,3};
    Vec3D v {4,5,6};
    Vec3D w = u + v;
    cout << w << endl;
    u = w + v;
    cout << u << endl;
    try {
    double q = u[4];
    } catch(Vec3D::bad_index bi) { }
}