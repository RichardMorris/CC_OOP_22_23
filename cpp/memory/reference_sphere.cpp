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


const double pi = M_PI;

int vec_count=0;
int max_count=0;
int alloc_count=0;
class Vec3D {
private:
	double x;
	double y;
	double z;

public:
	Vec3D() : x{0}, y{0}, z{0} {
        cout << "Vec3D construct ("  << x << " " << y << " " << z << ") addr " << &x << endl;
        ++vec_count; ++alloc_count;
        if(vec_count>max_count) max_count=vec_count;
	}

	Vec3D(double x_in,double y_in, double z_in) : x{x_in}, y{y_in}, z{z_in} {
        cout << "Vec3D construct ("  << x << " " << y << " " << z << ") addr " << &x << endl;
        ++vec_count; ++alloc_count;
        if(vec_count>max_count) max_count=vec_count;
	}

    Vec3D(const Vec3D& v) : x{v.x}, y{v.y}, z{v.z} {
        cout << "Vec3D copy ("  << x << " " << y << " " << z << ") addr " << &x << endl;
        ++vec_count; ++alloc_count;
        if(vec_count>max_count) max_count=vec_count;
    }

    ~Vec3D() {
        cout << "Vec3D desruct ("  << x << " " << y << " " << z << ") addr " << &x << endl;
        --vec_count;
    }
    
    void operator=(const Vec3D &v) {
        x = v.x; y = v.y; z = v.z;
        cout << "Vec3D = ("  << x << " " << y << " " << z << ") addr " << &x << endl;
    }

    void set(double xi, double yi, double zi) {
        x = xi; y = yi; z = zi;
        cout << "Vec3D set ("  << x << " " << y << " " << z << ") addr " << &x << endl;
    }

	void set_from_difference(const Vec3D &u, const Vec3D &v) {
	    set(u.x - v.x, u.y - v.y, u.z - v.z);
	}

	double dot(const Vec3D &v) const {
		return x * v.x + y * v.y + z * v.z;
	}

	void set_from_cross(const Vec3D &u, const Vec3D &v) {
		set(u.y * v.z - u.z * v.y,
		            u.z * v.x - u.x * v.z,
            		u.x * v.y - u.y * v.x);
 	}
	
	double lensq() const {
		return x*x + y*y + z*z;
	}
};

class Triangle {
	Vec3D &u;
	Vec3D &v;
	Vec3D &w;
	
public:
	Triangle(Vec3D &ui, Vec3D &vi, Vec3D &wi) :
	    u{ui}, v{vi}, w{wi}	{}

	~Triangle() {
//	    cout << "Triangle destruct\n";
	}

	double area2() {
    	static Vec3D v_u; // only created once in lifetime of program
	    static Vec3D w_u; // so reused often. Could break in multi threaded
		
		v_u.set_from_difference(v,u);
		w_u.set_from_difference(w,u);
		double a = v_u.lensq();
		double b = w_u.lensq();
		double c = v_u.dot(w_u);
		double l = sqrt(a * b - c * c) / 2.0;
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
	Vec3D row0[phi_step+1];
	Vec3D row1[phi_step+1];
	for(int i=0; i<= phi_step; ++i) {
		row0[i].set(
				rad * sin(0) * cos((2 * pi * i) / phi_step),
				rad * sin(0) * sin((2 * pi * i) / phi_step),
				rad * cos(0));
	}
//	cout << "row 0 " << vec_count << endl;
	for(int row=1; row<= theta_step; ++row ) {

		for(int i=0; i<= phi_step; ++i) {
			row1[i].set(
					rad * sin(pi * row / theta_step) * cos(2 * pi * i / phi_step),
					rad * sin(pi * row / theta_step) * sin(2 * pi * i / phi_step),
					rad * cos(pi * row / theta_step));
		}
//  	    cout << "row " << row << " " << vec_count << endl;

		for(int i=0; i< phi_step; ++i) {
			Triangle t1(row0[i],row0[i+1],row1[i]);
			total_area += t1.area2();
			Triangle t2(row1[i],row1[i+1],row0[i+1]);
			total_area += t2.area2();
		}

//  	    cout << "row " << row << " " << vec_count << endl;
		for(int i=0; i<= phi_step; ++i) {
			row0[i] = row1[i];
		}
//  	    cout << "row " << row << " " << vec_count << endl;
	}
	return total_area;
}


void calc_area(double r, int n_th, int n_phi) {
	Sphere sphere(r,n_th,n_phi);
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
	cout.setf(std::ios_base::fixed);
	cout.precision(3);

    calc_area(r, n_th, n_phi);

    cout << "Num vec " << vec_count << " max " << max_count << " alloc " << alloc_count << endl;
}
