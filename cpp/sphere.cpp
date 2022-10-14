#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <math.h>
using std::cout;
using std::endl;
#define PRINT_ALLOC 1
#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

const double pi = M_PI;

int vec_count=0;
int max_count=0;
int alloc_count=0;

class Vec3D {
public:
	double x;
	double y;
	double z;
	
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

	Vec3D* sub(Vec3D* other) {
		return new Vec3D(x - other->x, y - other->y, z - other->z);
	}

	double dot(Vec3D* v) {
		return x * v->x + y * v->y + z * v->z;
	}

	Vec3D* cross(Vec3D* v) {
		return new Vec3D(y * v->z - z * v->y, z * v->x - x * v->z, x * v->y - y * v->x  );
	}
	double lensq() {
		return x*x + y*y + z*z;
	}
};

class Triangle {
	Vec3D* u;
	Vec3D* v;
	Vec3D* w;
public:
	Triangle(Vec3D* ui, Vec3D* vi, Vec3D* wi) {
		u = ui; v = vi; w = wi;
	}
	double area() {
		Vec3D* v_u = v->sub(u);
		Vec3D* w_u = w->sub(u);
		Vec3D* norm = v_u->cross(w_u);
		double l = sqrt(norm->lensq()) / 2.0;
		delete v_u;
		delete w_u;
		delete norm;
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
	Vec3D* row0[phi_step+1];
	Vec3D* row1[phi_step+1];
	for(int i=0; i<= phi_step; ++i) {
		row0[i] = new Vec3D(
				rad * sin(0) * cos((2 * pi * i) / phi_step),
				rad * sin(0) * sin((2 * pi * i) / phi_step),
				rad * cos(0));
	}
	cout << endl;
	for(int j=1; j<= theta_step; ++j ) {

		for(int i=0; i<= phi_step; ++i) {
			row1[i] = new Vec3D(
					rad * sin(pi * j / theta_step) * cos(2 * pi * i / phi_step),
					rad * sin(pi * j / theta_step) * sin(2 * pi * i / phi_step),
					rad * cos(pi * j / theta_step));
		}

		for(int i=0; i< phi_step; ++i) {
			Triangle t1(row0[i],row0[i+1],row1[i]);
			total_area += t1.area();
			Triangle t2(row1[i],row1[i+1],row0[i+1]);
			total_area += t2.area();
			//cout << "Tri areas " << t1.area() << " " << t2.area() << endl;
		}

		for(int i=0; i<= phi_step; ++i) {
			delete row0[i];
			row0[i] = row1[i];
		}
	}
	for(int i=0; i<= phi_step; ++i) {
		delete row0[i];
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

	cout << "Num vec " << vec_count << " max " << max_count << " alloc " << alloc_count << endl;
}
