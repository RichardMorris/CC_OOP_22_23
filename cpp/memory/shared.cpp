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
        cout << "Vec3D construct ("  << x << " " << y << " " << z << ") addr " << &x << endl;
        ++vec_count;
	}

    Vec3D(const Vec3D& v) : x{v.x}, y{v.y}, z{v.z} {
        cout << "Vec3D copy ("  << x << " " << y << " " << z << ") addr " << &x << endl;
        ++vec_count;
    }

    ~Vec3D() {
        cout << "Vec3D desruct ("  << x << " " << y << " " << z << ") addr " << &x << endl;
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
        cout << "Line destruct\n";
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
	    cout << "Triangle destruct\n";
	}

	double area() {
		shared_ptr<Vec3D> v_u = v->sub(u);
		shared_ptr<Vec3D> w_u = w->sub(u);
		shared_ptr<Vec3D> norm = v_u->cross(w_u);
		double l = sqrt(norm->lensq()) / 2.0;
		return l;
	}
};

class DirectedEdge {
public:
    shared_ptr<Vec3D> v;
    shared_ptr<DirectedEdge> next;
    
    DirectedEdge(shared_ptr<Vec3D> vi) : v{vi} {}
};

// Illustates a problem with shared pointers
// where the memory may never be free up
void cycle() {
    shared_ptr<Vec3D> u{new Vec3D(1,0,0)};
    shared_ptr<Vec3D> v{new Vec3D(0,1,0)};
    shared_ptr<Vec3D> w{new Vec3D(0,0,1)};


    std::shared_ptr<DirectedEdge> p=std::make_shared<DirectedEdge>(u);
    std::shared_ptr<DirectedEdge> q=std::make_shared<DirectedEdge>(v);
    std::shared_ptr<DirectedEdge> r=std::make_shared<DirectedEdge>(w);
    p->next = q; q->next =r; r->next=p;
    cout << "Num vec " << vec_count << endl;
}

//
void heron() {
    shared_ptr<Vec3D> u{new Vec3D(1,0,0)};
    shared_ptr<Vec3D> v{new Vec3D(0,1,0)};
    shared_ptr<Vec3D> w{new Vec3D(0,0,1)};

    {// Vec3D w(1,2,3);
        Triangle t(u,v,w);
        double area = t.area();
        cout << "Vector area " << area << endl;
    } // Triangle freed when goes out of scope
    
    Line *l1 = new Line(u,v);
    double a = l1->len();
    Line *l2 =  new Line(v,w);
    double b = l2->len();
    Line *l3 = new Line(w,u);
    double c = l3->len();
    cout << "Num vec " << vec_count << endl;
    u.reset(); // releases a handle to the smart-pointer
    v.reset(); // won't be freed yet as the lines still references smart-pointer
    w.reset(); //
    
    delete l1;
    delete l2; // second vector deconstructed here
    double s = (a+b+c)/2;
    cout << "a " << a << " s " << s << " heron " << sqrt(s*(s-a)*(s-b)*(s-c)) << endl;
    
    delete l3; // 1st and 3rd vector freed here
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

    cout << r << " " << n_th << " " << n_phi << endl;
    heron();
    cout << "Num vec " << vec_count << endl;
    cycle();
    cout << "Num vec " << vec_count << endl;
}
