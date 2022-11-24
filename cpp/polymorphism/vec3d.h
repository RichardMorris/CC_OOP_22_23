#include <iostream>
using std::cout;
using std::endl;
using std::ostream;
using std::unique_ptr;
using std::make_unique;
using std::shared_ptr;

#define PRINT_ALLOC 1

class Vec3D {
private:
	double x;
	double y;
	double z;

public:
	// Constructor
	Vec3D(double x_in,double y_in, double z_in) : x{x_in}, y{y_in}, z{z_in} {
#ifdef 	PRINT_ALLOC
        cout << "Vec3D construct ("  << x << " " << y << " " << z << ") addr " << &x << endl;
#endif
	}

    // Copy constructor
    Vec3D(const Vec3D& v) : x{v.x}, y{v.y}, z{v.z} {
#ifdef 	PRINT_ALLOC
        cout << "Vec3D copy ("  << x << " " << y << " " << z << ") addr " << &x << endl;
#endif
    }

    // Destructor
    ~Vec3D() {
#ifdef 	PRINT_ALLOC
        cout << "Vec3D desruct ("  << x << " " << y << " " << z << ") addr " << &x << endl;
#endif
    }

    // Assignment operator
    void operator=(const Vec3D &v) {
        x = v.x; y = v.y; z = v.z;
    #ifdef 	PRINT_ALLOC
        cout << "Vec3D = ("  << x << " " << y << " " << z << ") addr " << &x << endl;
     #endif
    }
    
    
    Vec3D& operator+=(const Vec3D &u) {
        x = x + u.x;
        y = y + u.y;
        z = z + u.z;
#ifdef 	PRINT_ALLOC
        cout << "Vec3D += ("  << x << " " << y << " " << z << ") addr " << &x << endl;
#endif
        return *this;
    }


	unique_ptr<Vec3D> sub(const Vec3D* other) {
		return make_unique<Vec3D>(x - other->x, y - other->y, z - other->z);
	}

	double dot(const Vec3D* v) {
		return x * v->x + y * v->y + z * v->z;
	}

	unique_ptr<Vec3D> cross(const Vec3D* v) {
		return make_unique<Vec3D>(y * v->z - z * v->y, z * v->x - x * v->z, x * v->y - y * v->x  );
	}
	double lensq() {
		return x*x + y*y + z*z;
	}

    class Bad_index {
        public:
            const int index;
            Bad_index(int ind)
                : index{ind}
                {}
    };
	
	double &operator[](int index) {
	    switch(index) {
	        case 0: return x;
	        case 1: return y;
	        case 2: return z;
	    }
        throw Bad_index {index};
	}
	
	friend Vec3D operator+(Vec3D u,const Vec3D &v);
	friend Vec3D operator-(const Vec3D &u,const Vec3D &v);
	friend bool operator==(const Vec3D &u,const Vec3D &v);
	
	friend ostream& operator<<(ostream &os,Vec3D &v) {
	    os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
	    return os;
	}
};

Vec3D operator+(Vec3D u,const Vec3D &v) {
    u += v;
#ifdef 	PRINT_ALLOC
    cout << "Vec3D +(V,V) ("  << u.x << " " << u.y << " " << u.z << ") addr " << &u << endl;
#endif

    return u;
}

Vec3D operator-(const Vec3D &u,const Vec3D &v) {
    Vec3D w {u.x - v.x, u.y - v.y, u.z - v.z};
#ifdef 	PRINT_ALLOC
    cout << "Vec3D -(V,V) ("  << w.x << " " << w.y << " " << w.z << ") addr " << &u << endl;
#endif
    return w;
}

bool operator==(const Vec3D &u,const Vec3D &v) {
#ifdef 	PRINT_ALLOC
    cout << "Vec3D ==(V,V) addr " << &u << " " << &v << endl;
#endif
    return u.x == v.x && u.y == v.y && u.z == v.z;
}
