#include <iostream>
#include <cmath>
#include <memory>
#include <stdlib.h>
#include <math.h>
using std::cout;
using std::endl;
using std::unique_ptr;
using std::make_unique;
using std::shared_ptr;
using std::make_shared;


const double pi = M_PI;

int vec_count = 0;
class Vec3D {
public:
	const double x;
	const double y;
	const double z;
	Vec3D(double x_in,double y_in, double z_in) : x{x_in}, y{y_in}, z{z_in} {
//        cout << "Vec3D construct ("  << x << " " << y << " " << z << ") addr " << &x << endl;
        ++vec_count;
	}

    Vec3D(const Vec3D& v) : x{v.x}, y{v.y}, z{v.z} {
//        cout << "Vec3D copy ("  << x << " " << y << " " << z << ") addr " << &x << endl;
        ++vec_count;
    }

    ~Vec3D() {
//        cout << "Vec3D destruct ("  << x << " " << y << " " << z << ") addr " << &x << endl;
        --vec_count;
    }

	shared_ptr<Vec3D> sub(shared_ptr<Vec3D> other) {
		return make_shared<Vec3D>(x - other->x, y - other->y, z - other->z);
	}

	double dot(const Vec3D* v) {
		return x * v->x + y * v->y + z * v->z;
	}

	shared_ptr<Vec3D> cross(shared_ptr<Vec3D> v) {
		return make_shared<Vec3D>(y * v->z - z * v->y, z * v->x - x * v->z, x * v->y - y * v->x  );
	}
	double lensq() {
		return x*x + y*y + z*z;
	}
};

class Line {
    shared_ptr<Vec3D> u;
    shared_ptr<Vec3D> v;
    
public:
    Line(const shared_ptr<Vec3D> ui, const shared_ptr<Vec3D> vi) : u{ui}, v{vi} {}
    
    ~Line() {
//        cout << "Line destruct\n";
    }
    
    double len() {
        shared_ptr<Vec3D> w = u->sub(v);
        return sqrt(w->lensq());
    }
};

class Triangle {
	shared_ptr<Vec3D> u;
	shared_ptr<Vec3D> v;
	shared_ptr<Vec3D> w;
public:
	Triangle(shared_ptr<Vec3D> ui, shared_ptr<Vec3D> vi, shared_ptr<Vec3D> wi) :
	    u{ui}, v{vi}, w{wi}	{}
	
	~Triangle() {
//	    cout << "Triangle destruct\n";
	}

	double area() {
		shared_ptr<Vec3D> v_u = v->sub(u);
		shared_ptr<Vec3D> w_u = w->sub(u);
		shared_ptr<Vec3D> norm = v_u->cross(w_u);
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
	shared_ptr<Vec3D> row0[phi_step+1];
	shared_ptr<Vec3D> row1[phi_step+1];
	for(int i=0; i<= phi_step; ++i) {
		row0[i] = make_shared<Vec3D>(
				rad * sin(0) * cos((2 * pi * i) / phi_step),
				rad * sin(0) * sin((2 * pi * i) / phi_step),
				rad * cos(0));
	}
	cout << "row 0 " << vec_count << endl;
	for(int row=1; row<= theta_step; ++row ) {

		for(int i=0; i<= phi_step; ++i) {
			row1[i] = make_shared<Vec3D>(
					rad * sin(pi * row / theta_step) * cos(2 * pi * i / phi_step),
					rad * sin(pi * row / theta_step) * sin(2 * pi * i / phi_step),
					rad * cos(pi * row / theta_step));
		}
  	    cout << "row " << row << " " << vec_count << endl;

		for(int i=0; i< phi_step; ++i) {
			Triangle t1(row0[i],row0[i+1],row1[i]);
			total_area += t1.area();
			Triangle t2(row1[i],row1[i+1],row0[i+1]);
			total_area += t2.area();
		}

  	    cout << "row " << row << " " << vec_count << endl;
		for(int i=0; i<= phi_step; ++i) {
			row0[i] = row1[i];
		}
  	    cout << "row " << row << " " << vec_count << endl;
	}
	return total_area;
}


void calc_area(double r, int n_th, int n_phi) {
	Sphere sphere(r,n_th,n_phi);
	cout.setf(std::ios_base::fixed);
	cout.precision(3);
	double a = sphere.area();
	cout << "aprox " << a << endl;
	cout << "standard " << 4.0 * pi * r * r << endl;
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
    cout << "Num vec " << vec_count << endl;
    calc_area(r,n_th,n_phi);
    cout << "Num vec " << vec_count << endl;
}
