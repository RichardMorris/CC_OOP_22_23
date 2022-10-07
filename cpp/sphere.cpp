#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <math.h>
using std::cout;
using std::endl;

const double pi = M_PI;

class Vec {
public:
	double x;
	double y;
	double z;
	Vec(double x_in,double y_in, double z_in) {
		x = x_in; y = y_in; z = z_in;
	}

	Vec* sub(Vec* other) {
		return new Vec(x - other->x, y - other->y, z - other->z);
	}

	double dot(Vec* v) {
		return x * v->x + y * v->y + z * v->z;
	}

	Vec* cross(Vec* v) {
		return new Vec(y * v->z - z * v->y, z * v->x - x * v->z, x * v->y - y * v->x  );
	}
	double lensq() {
		return x*x + y*y + z*z;
	}
};

class Triangle {
	Vec* u;
	Vec* v;
	Vec* w;
public:
	Triangle(Vec* ui, Vec* vi, Vec* wi) {
		u = ui; v = vi; w = wi;
	}
	double area() {
		Vec* v_u = v->sub(u);
		Vec* w_u = w->sub(u);
		Vec* norm = v_u->cross(w_u);
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
	Vec* row0[phi_step+1];
	Vec* row1[phi_step+1];
	for(int i=0; i<= phi_step; ++i) {
		row0[i] = new Vec(
				rad * sin(0) * cos((2 * pi * i) / phi_step),
				rad * sin(0) * sin((2 * pi * i) / phi_step),
				rad * cos(0));
	}
	cout << endl;
	for(int j=1; j<= theta_step; ++j ) {

		for(int i=0; i<= phi_step; ++i) {
			row1[i] = new Vec(
					rad * sin(pi * j / theta_step) * cos(2 * pi * i / phi_step),
					rad * sin(pi * j / theta_step) * sin(2 * pi * i / phi_step),
					rad * cos(pi * j / theta_step));
//			cout << row0[i]->x << " " << row0[i]->y << " " << row0[i]->z << "\t";
//			cout << row1[i]->x << " " << row1[i]->y << " " << row1[i]->z << endl;
		}
//		cout << endl;

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
	return total_area;
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
	Sphere sphere(r,n_th,n_phi);
	cout.setf(std::ios_base::fixed);
	cout.precision(3);
	cout << "aprox " << sphere.area() << endl;
	cout << "standard " << 4.0 * pi * r * r << endl;
	cout << "ip " << pi * pi * r * r << endl;
}
