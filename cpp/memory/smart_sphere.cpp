#include <iostream>
#define _USE_MATH_DEFINES // Added this to fix the syntax error for M_PI - David.B
#include <cmath>
#include <memory>
#include <stdlib.h>
#include <math.h>
using std::cout;
using std::endl;
using std::unique_ptr;
using std::make_unique;
using std::shared_ptr;

//#define PRINT_ALLOC 1

const double pi = M_PI;
int vec_count=0;
int max_count=0;
int alloc_count=0;

class Vec3D {
private:
    // Assignment operator
    void operator=(const Vec3D &v) {
    //     // x = v.x; y = v.y; z = v.z;
    #ifdef 	PRINT_ALLOC
        cout << "Vec3D = ("  << x << " " << y << " " << z << ") addr " << &x << endl;
     #endif
    }

public:
	const double x;
	const double y;
	const double z;
	
	// Constructor
	Vec3D(double x_in,double y_in, double z_in) : x{x_in}, y{y_in}, z{z_in} {
#ifdef 	PRINT_ALLOC
        cout << "Vec3D construct ("  << x << " " << y << " " << z << ") addr " << &x << endl;
#endif
        ++vec_count; ++alloc_count;
        if(vec_count>max_count) max_count=vec_count;
	}

    // Copy constructor
    Vec3D(const Vec3D& v) : x{v.x}, y{v.y}, z{v.z} {
#ifdef 	PRINT_ALLOC
        cout << "Vec3D copy ("  << x << " " << y << " " << z << ") addr " << &x << endl;
#endif
        ++vec_count; ++alloc_count;
        if(vec_count>max_count) max_count=vec_count;
    }

    // Destructor
    ~Vec3D() {
#ifdef 	PRINT_ALLOC
        cout << "Vec3D desruct ("  << x << " " << y << " " << z << ") addr " << &x << endl;
#endif
        --vec_count;
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
};

// A shorthand for the type
using upVec3D = unique_ptr<Vec3D>;

class Triangle {
	upVec3D u;
	upVec3D v;
	upVec3D w;
public:
	Triangle(const Vec3D *ui, const Vec3D *vi, const Vec3D *wi) :
	    u {upVec3D(new Vec3D(*ui))},
	    v {upVec3D(new Vec3D(*vi))},
	    w {upVec3D(new Vec3D(*wi))}
	{}
	
	~Triangle() {
#ifdef 	PRINT_ALLOC
	    cout << "Triangle destruct\n";
#endif
	}

	double area() {
		upVec3D v_u = v->sub(u.get());
		upVec3D w_u = w->sub(u.get());
		upVec3D norm = v_u->cross(w_u.get());
		double l = sqrt(norm->lensq()) / 2.0;
		return l;
	}

};

class Sphere {
	double rad;
	int theta_step;
	int phi_step;

public:
	Sphere(double r, int th_in, int ph_inc);
	double area();
};

Sphere::Sphere(double r,int th_in, int phi_in) {
	rad = r;
	theta_step = th_in;
	phi_step = phi_in;
}

double Sphere::area() {
	double total_area = 0.0;
	unique_ptr<Vec3D> row0[phi_step+1];
	unique_ptr<Vec3D> row1[phi_step+1];
	for(int i=0; i<= phi_step; ++i) {
		row0[i] = make_unique<Vec3D>(
				rad * sin(0) * cos((2 * pi * i) / phi_step),
				rad * sin(0) * sin((2 * pi * i) / phi_step),
				rad * cos(0));
	}
	cout << endl;
	for(int j=1; j<= theta_step; ++j ) {

		for(int i=0; i<= phi_step; ++i) {
			row1[i] = make_unique<Vec3D>(
					rad * sin(pi * j / theta_step) * cos(2 * pi * i / phi_step),
					rad * sin(pi * j / theta_step) * sin(2 * pi * i / phi_step),
					rad * cos(pi * j / theta_step));
		}

		for(int i=0; i< phi_step; ++i) {
			Triangle t1(row0[i].get(),row0[i+1].get(),row1[i].get());
			total_area += t1.area();
			Triangle t2(row1[i].get(),row1[i+1].get(),row0[i+1].get());
			total_area += t2.area();
		}
	}

    cout << "Sphere.area Num vec " << vec_count << " max " << max_count << " alloc " << alloc_count << endl;
	return total_area;
}

void calc_area(double r, int n_th, int n_phi) {
	Sphere sphere(r,n_th,n_phi);
	double a = sphere.area();
	cout << "aprox " << a << endl;
	cout << "standard " << 4.0 * pi * r * r << endl;
    cout << "calc_area Num vec " << vec_count << " max " << max_count << " alloc " << alloc_count << endl;
}

int main(int argc, char* argv[]) {
	double r = 1.0;
    int n_th = 20;
    int n_phi = 20;
    if(argc==4) {
        r = atof(argv[1]);
        n_th = atoi(argv[2]);
        n_phi = atoi(argv[3]);
    }
    cout << r << " " << n_th << " " << n_phi << endl;
	cout.setf(std::ios_base::fixed);
	cout.precision(3);

    calc_area(r,n_th,n_phi);
    cout << "main Num vec " << vec_count << " max " << max_count << " alloc " << alloc_count << endl;
    
}
