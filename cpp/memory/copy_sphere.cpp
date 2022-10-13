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
public:
	double x;
	double y;
	double z;

	Vec3D() : x{0}, y{0}, z{0} {
//        cout << "Vec3D construct ("  << x << " " << y << " " << z << ") addr " << &x << endl;
        ++vec_count; ++alloc_count;
        if(vec_count>max_count) max_count=vec_count;
	}

	Vec3D(double x_in,double y_in, double z_in) : x{x_in}, y{y_in}, z{z_in} {
//        cout << "Vec3D construct ("  << x << " " << y << " " << z << ") addr " << &x << endl;
        ++vec_count; ++alloc_count;
        if(vec_count>max_count) max_count=vec_count;
	}

    Vec3D(const Vec3D& v) : x{v.x}, y{v.y}, z{v.z} {
//        cout << "Vec3D copy ("  << x << " " << y << " " << z << ") addr " << &x << endl;
        ++vec_count;
    }

    ~Vec3D() {
//        cout << "Vec3D desruct ("  << x << " " << y << " " << z << ") addr " << &x << endl;
        --vec_count;
    }
    
    void operator=(const Vec3D &v) {
        x = v.x; y = v.y; z = v.z;
//        cout << "Vec3D = ("  << x << " " << y << " " << z << ") addr " << &x << endl;
    }

    void set(double xi, double yi, double zi) {
        x = xi; y = yi; z = zi;
    }
	Vec3D sub(const Vec3D v) const {
	    Vec3D res{x - v.x, y - v.y, z - v.z};
	    Vec3D neg{v.x - x, v.y - y, v.z - z};
	    if(x > v.x) {
		    return res;
	    } else {
		    return neg;
	    }
	}

	double dot(const Vec3D v) const {
		return x * v.x + y * v.y + z * v.z;
	}

	Vec3D cross(const Vec3D v) const {
		Vec3D res(y * v.z - z * v.y,
		            z * v.x - x * v.z,
            		x * v.y - y * v.x);
        return res;
	}
	
	double lensq() const {
		return x*x + y*y + z*z;
	}
};

class Line {
    const Vec3D &u;
    const Vec3D &v;
    
public:
    Line(const Vec3D &ui, const Vec3D &vi) : u{ui}, v{vi} {}
    
    ~Line() {
        cout << "Line destruct\n";
    }
    
    double len() const {
        Vec3D w = u.sub(v);
        return sqrt(w.lensq());
    }
};

class Triangle {
	Vec3D u;
	Vec3D v;
	Vec3D w;
public:
	Triangle(Vec3D ui, Vec3D vi, Vec3D wi) :
	    u{ui}, v{vi}, w{wi}	{}

	~Triangle() {
//	    cout << "Triangle destruct\n";
	}

	double area() {
		Vec3D v_u = v.sub(u);
		Vec3D w_u = w.sub(u);
		Vec3D norm = v_u.cross(w_u);
		double l = sqrt(norm.lensq()) / 2.0;
		return l;
	}

	double area2() {
		Vec3D v_u = v.sub(u);
		Vec3D w_u = w.sub(u);
		double a = v_u.lensq();
		double b = w_u.lensq();
		double c = v_u.dot(w_u);
		double l = sqrt(a * b - c * c) / 2.0;
		return l;
	}

    
	double area3() {
		Vec3D v_u = v.sub(u);
		Vec3D w_u = w.sub(u);
		Vec3D w_v = w.sub(v);
		double asq = v_u.lensq();
		double bsq = w_u.lensq();
		double csq = w_v.lensq();
		double sumsq = asq + bsq + csq;
		double l = sqrt( sumsq * sumsq- 2*( asq*asq + bsq*bsq + csq*csq));
		if(l != l) {
		    cout << asq << " " << bsq << " " << csq << endl;
		    l = 0;
		}
		return l/4;
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
	cout.setf(std::ios_base::fixed);
	cout.precision(3);
	double a = sphere.area();
	cout << "aprox " << a << endl;
	cout << "standard " << 4.0 * pi * r * r << endl;
}


void heron() {
    Vec3D u{1,0,0};
    Vec3D v{0,1,0};
    Vec3D w{0,0,1};

    Line l1(u, v);
    double a = l1.len();
    Line l2(v, w);
    double b = l2.len();
    Line l3(w, u);
    double c = l3.len();
    cout << "Num vec " << vec_count << endl;
    double s = (a+b+c)/2;
    cout << "a " << a << " s " << s << " heron " << sqrt(s*(s-a)*(s-b)*(s-c)) << endl;
    
    Triangle t{u,v,w};
    cout << t.area() << " " << t.area2() << " " << t.area3() << endl;
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

//    heron();
    calc_area(r, n_th, n_phi);

    cout << "Num vec " << vec_count << " max " << max_count << " alloc " << alloc_count << endl;
}
